#ifndef _JNI_REGIST_STATIC_METHOD_H_
#define _JNI_REGIST_STATIC_METHOD_H_

#include <jni.h>
#include <string>
#include <android/log.h>
#include "NE_pubhead.h"
using namespace std;

/*
 * 通用的注册Java静态方法的接口, 可变参数CallRequestedMothod
 * */
//JNIRegistStaticMethod 参数 JNI实现

class JNIRegistStaticMethod
{
public:

	JNIRegistStaticMethod();

	int RegisterCallback(JNIEnv* env, const char* pszClassName, const char* szMethodName, const char* szMethodSign);

	void CallRequestedMothod();

	void CallRequestedMothod(int p1, int p2);

	void CallRequestedMothod(int p1);

	int CallRequestedMothodRet();

	kn_string CallRequestedMothodObjectRet(short* points, int iArraySize);

	int CallRequestedMothodIntRet(int p1, int p2, int p3, const char* szp4);

	int UnregisterCallback(JNIEnv* env);

private:

	void EndCallback(JNIEnv * env);
	JNIEnv * BeginCallback();
private:
	bool m_bRegistered;
	bool m_isAttached;
	jclass m_class;
	
	jmethodID m_methodCallback;

	string m_strClassName;
	string m_strMethodName;
	string m_strMethodSign;
};

#endif	//	_JNI_REUESTRENDER_FROM_LOCATOR_H_
