#include "JNIRegistStaticMethod.h"
#include <stdlib.h>

extern JavaVM * gJavaVM;	//	全局变量, 在JNI_OnLoad时获取

#define JR_MUTEX

JNIRegistStaticMethod::JNIRegistStaticMethod()
{
	m_bRegistered = false;
	m_isAttached = false;
	m_class = NULL;
	m_methodCallback = NULL;
}

int JNIRegistStaticMethod::UnregisterCallback(JNIEnv * env)
{
	JR_MUTEX
	m_bRegistered = false;
	if(m_class)
	{
		env->DeleteGlobalRef(m_class);
		m_class = NULL;
	}
	return 0;
}

//	避免抛出异常挂掉
void AvoidException(JNIEnv* env)
{
	if(env->ExceptionCheck())
	{
		//	通过System.out.err 输出
		env->ExceptionDescribe();
		//	清除异常
		env->ExceptionClear();
	}
}

int JNIRegistStaticMethod::RegisterCallback(JNIEnv* env, const char* pszClassName, const char* szMethodName, const char* szMethodSign)
{
	JR_MUTEX	//	register 和 Call不能同时进行
	m_bRegistered = false;
	if(m_class)
	{
		//	避免重复Register
		env->DeleteGlobalRef(m_class);
		m_class = NULL;
	}
	//	获取类名 "android/navi/ui/mainmap/MainMapActivity"
	jclass jcLocal = env->FindClass(pszClassName);
	if(jcLocal == NULL)
	{
		LOGE("FindClass %s error", pszClassName);
		AvoidException(env);
		return -1;
	}
	m_class = (jclass)env->NewGlobalRef(jcLocal);
	LOGI ("RegisterCallback jcLocal = %p, m_class = %p", jcLocal, m_class);
	env->DeleteLocalRef(jcLocal);
	if(m_class == NULL)
	{
		LOGE("NewGlobalRef %s error", pszClassName);
		AvoidException(env);
		return -3;
	}
	// 获取方法的签名 javap -s -public  MainActivity
	//	"requestRenderFromLocator", "()V"
	// 获取方法
	m_methodCallback = env->GetStaticMethodID(m_class, szMethodName, szMethodSign);
	if (m_methodCallback == NULL)
	{
		LOGE("GetMethodID %s - %s error", szMethodName, szMethodSign);
		AvoidException(env);
		return -2;
	}
	LOGI("RegisterCallback  ok");
	m_bRegistered = true;
	m_strClassName = pszClassName;
	m_strMethodName = szMethodName;
	m_strMethodSign = szMethodSign;
	return 0;
}

// 获取JNIEnv
JNIEnv *JNIRegistStaticMethod::BeginCallback()
{
	if (!m_bRegistered)
	{
		return NULL;
	}
	JavaVM *jvm = gJavaVM;
	JNIEnv *env = NULL;
	m_isAttached = false;
	int status = jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
	if (status != JNI_OK)
	{
		//	LOGI("CallRequestRenderFromLocator jvm->GetEnv = %p, iStatus = %d",	env, status);
		status = jvm->AttachCurrentThread(&env, NULL);
		//LOGI("CallRequestRenderFromLocator AttachCurrentThread  = %d, Env = %p", status, env);
		if (status != JNI_OK)
		{
			LOGE("CallRequestRenderFromLocator: failed to AttachCurrentThread = %d", status);
			return NULL;
		}
		m_isAttached = true;
	}
	return env;
}

void JNIRegistStaticMethod::EndCallback(JNIEnv * env)
{
	//LOGI ("CallRequestRenderFromLocator end");
	AvoidException(env);
	if(m_isAttached)
	{
		gJavaVM->DetachCurrentThread();
	}
}

void JNIRegistStaticMethod::CallRequestedMothod()
{
	JR_MUTEX
	JNIEnv * env = BeginCallback();
	if(env != NULL)
	{
		env->CallStaticVoidMethod(m_class, m_methodCallback);
		EndCallback(env);
	}
	//LOGI ("JNIRegistStaticMethod::CallRequestedMothod");
}

void JNIRegistStaticMethod::CallRequestedMothod(int p1, int p2)
{
	JR_MUTEX
	JNIEnv * env = BeginCallback();
	if(env != NULL)
	{
		env->CallStaticVoidMethod(m_class, m_methodCallback, p1, p2);
		EndCallback(env);
	}
	//LOGI ("JNIRegistStaticMethod::CallRequestedMothod   p1(%d) p2(%d)", p1, p2);
}


void JNIRegistStaticMethod::CallRequestedMothod(int p1)
{
	JR_MUTEX
	JNIEnv * env = BeginCallback();
	if(env != NULL)
	{
		env->CallStaticVoidMethod(m_class, m_methodCallback, p1);
		EndCallback(env);
	}
	//LOGI ("JNIRegistStaticMethod::CallRequestedMothod   p1(%d)", p1);
}

int JNIRegistStaticMethod::CallRequestedMothodRet()
{
	JR_MUTEX
	JNIEnv * env = BeginCallback();
	int ret = 0;
	if(env != NULL)
	{
		ret = env->CallStaticIntMethod(m_class, m_methodCallback);
		EndCallback(env);
	}
	//LOGI ("JNIRegistStaticMethod::CallRequestedMothod   ret(%d)", ret);
	return ret;
}

kn_string JNIRegistStaticMethod::CallRequestedMothodObjectRet(short* points, int iArraySize)
{
	JR_MUTEX
	JNIEnv * env = BeginCallback();
	kn_string strRet;
	if(env != NULL)
	{
		jshortArray jShortArray= env->NewShortArray(iArraySize);
		jshort* pJavaBuf = env->GetShortArrayElements(jShortArray, NULL);
		memcpy(pJavaBuf, points, iArraySize * sizeof(short));
		jobject ret = env->CallStaticObjectMethod(m_class, m_methodCallback, jShortArray, iArraySize);
		env->ReleaseShortArrayElements(jShortArray, pJavaBuf, 0);
		jsize iStringLen = env->GetStringLength((jstring)ret);
		if(iStringLen > 0)
		{
			const jchar* pWChars = env->GetStringChars((jstring)ret,  NULL);
			kn_char* pszTemp = new kn_char [iStringLen + 1];
			pszTemp[iStringLen] = 0;
			for(int i = 0; i < iStringLen; i++)
			{
				pszTemp[i] = 	pWChars[i];
			}
			strRet = pszTemp;
			delete [] pszTemp;
			env->ReleaseStringChars((jstring)ret, pWChars);
		}
		EndCallback(env);
	}
	//LOGI ("JNIRegistStaticMethod::CallRequestedMothod   ret(%d)", ret);
	return strRet;
}

int JNIRegistStaticMethod::CallRequestedMothodIntRet(int p1, int p2, int p3, const char* szp4)
{
	JR_MUTEX
	JNIEnv * env = BeginCallback();
	int iRet = 0;
	if(env != NULL)
	{
		jstring jstr = env->NewStringUTF(szp4);
		iRet = env->CallStaticIntMethod(m_class, m_methodCallback, p1, p2, p3, jstr);
		env->DeleteLocalRef(jstr);
		EndCallback(env);
	}
	return iRet;
}
