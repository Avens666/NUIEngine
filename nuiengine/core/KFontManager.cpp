//  **************************************
//  File:        KFontManager.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description: This code is part of NUI Engine (NUI Graphics Lib)
//  Comments:
//  Rev:         2
//  Created:     2017/4/11
//  Last edit:   2017/4/28
//  Author:      Chen Zhi
//  E-mail:      cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
//  ***************************************
#include "KFontManager.h"
#include "KTextDrawable.h"
#include "boost/unordered_map.hpp"
#include "CharSet.h"
#include "FilePath.h"

#define SWAPWORD(x)		MAKEWORD(HIBYTE(x), LOBYTE(x))
#define SWAPLONG(x)		MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))

typedef short SHORT;
typedef unsigned short USHORT;
typedef	unsigned long ULONG;
typedef DWORD Fixed; //Re-defined
typedef SHORT FWORD;
typedef INT64 LONGDATETIME;
typedef USHORT UFWORD;

kn_string EncodingTable[11]={_T("Symbol"),_T("Unicode BMP (UCS-2)"),_T("ShiftJIS"),_T("PRC"),_T("Big5"),
							_T("Wansung"),_T("Johab"),_T("Reserved"),_T("Reserved"),_T("Reserved"),
							_T("Unicode UCS-4")};

typedef struct _tagTT_OFFSET_TABLE{
	USHORT uMajorVersion;
	USHORT uMinorVersion;
	USHORT uNumOfTables;
	USHORT uSearchRange;
	USHORT uEntrySelector;
	USHORT uRangeShift;
}TT_OFFSET_TABLE;
typedef struct _tagTT_TABLE_DIRECTORY{
	char	szTag[4];			//table name
	ULONG	uCheckSum;			//Check sum
	ULONG	uOffset;			//Offset from beginning of file
	ULONG	uLength;			//length of the table in bytes
}TT_TABLE_DIRECTORY;

typedef struct _tagTT_NAME_TABLE_HEADER{
	USHORT	uFSelector;			//format selector. Always 0
	USHORT	uRecordCount;			//Name Records count
	USHORT	uStorageOffset;		//Offset for strings storage, from start of the table
}TT_NAME_TABLE_HEADER;

typedef struct _tagTT_NAME_RECORD{
	USHORT	uPlatformID;
	USHORT	uEncodingID;
	USHORT	uLanguageID;
	USHORT	uNameID;
	USHORT	uStringLength;
	USHORT	uStringOffset;	//from start of storage area
}TT_NAME_RECORD;

typedef struct _tagTT_HEAD_RECORD{
	Fixed	versionNumber;
	Fixed	fontRevision;
	ULONG	checkSumAdjustment;
	ULONG	magicNumber;
	USHORT	flags;
	USHORT	unitsPerEm;
	UINT64	created;
	UINT64	modified;
	SHORT	xMin;
	SHORT	yMin;
	SHORT	xMax;
	SHORT	yMax;
	USHORT	macStyle;
	USHORT	lowestRecPPEM;
	SHORT	fontDirectionHint;
	SHORT	indexToLocFormat;
	SHORT	glyphDataFormat;
}TT_HEAD_RECORD;

typedef struct _tagTT_HHEA_RECORD{
	Fixed	Table_version_number;
	FWORD	Ascender;
	FWORD	Descender;
	FWORD	LineGap;
	UFWORD	advanceWidthMax;
	FWORD	minLeftSideBearing;
	FWORD	minRightSideBearing;
	FWORD	xMaxExtent;
	SHORT	caretSlopeRise;
	SHORT	caretSlopeRun;
	SHORT	caretOffset;
	SHORT	reserved1;
	SHORT	reserved2;
	SHORT	reserved3;
	SHORT	reserved4;
	SHORT	metricDataFormat;
	USHORT	numberOfHMetrics;
}TT_HHEA_RECORD;

typedef struct _tagTT_MAXP_RECORD{
	Fixed	Table_version_number;
	USHORT	numGlyphs;
}TT_MAXP_RECORD;

#define EnglishSampleText _T("THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.1234567890")
#define ChineseSampleText _T("Windows 中文字型范例 1234567890")

kn_string MByteToWChar(const string& str )
{
	int len = 0;
	len = str.length();
	int  unicodeLen = ::MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,NULL,0);
	wchar_t *  pUnicode;
	pUnicode = new  wchar_t[unicodeLen+1];
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,(LPWSTR)pUnicode,unicodeLen);
	kn_string  rt;
	rt = ( wchar_t* )pUnicode;
	delete  pUnicode;
	return  rt; 
}

kn_string GetFakeUnicodeData(TCHAR* pContent,int dataLength)
{
	char* chContent;
	chContent = new char[1+dataLength / 2];
	memset(chContent,0,1+dataLength / 2);
	for (int i=0;i<dataLength / 2;i++)
	{
		chContent[i]=HIBYTE(pContent[i]);
	}
	kn_string result = MByteToWChar(chContent);
	delete chContent;
	return result;
}

kn_string GetUnicodeData(TCHAR* pContent,int dataLength)
{
	for (int i=0;i<dataLength / 2;i++)
	{
		pContent[i]=SWAPWORD(pContent[i]);
	}

	kn_string result=pContent;

	return result;
}

void GetFontPropertyFromFile(kn_string strFilePath,stFontProperty & fontProp)
{ 
#ifdef WIN32
	HANDLE hFile = CreateFile(strFilePath.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	DWORD dwRead = 0;
	DWORD dwFileSize = GetFileSize(hFile,NULL); 
	int nK = dwFileSize/1024;
	char cBuff[32] = {0};
	itoa(nK,cBuff,10);
	string strSize = cBuff;
	kn_string wstrSize = MByteToWChar(strSize) + _T("KB "); 
	fontProp.strFileSize=_T("  ")+wstrSize;
	fontProp.strFontFilePath=strFilePath;
	
	TT_OFFSET_TABLE ttOffsetTable;
	ReadFile(hFile,&ttOffsetTable, sizeof(TT_OFFSET_TABLE),&dwRead,NULL);
	ttOffsetTable.uNumOfTables = SWAPWORD(ttOffsetTable.uNumOfTables);
	ttOffsetTable.uMajorVersion = SWAPWORD(ttOffsetTable.uMajorVersion);
	ttOffsetTable.uMinorVersion = SWAPWORD(ttOffsetTable.uMinorVersion);

	TT_TABLE_DIRECTORY findTableDirectory,nameTableDir,headTableDir,hheaTableDir,maxpTableDir;
	BOOL bFoundNameTable = FALSE, bFoundHeadTable=FALSE, bFoundHheaTable=FALSE,bFoundMaxpTable=FALSE;

	std::vector<char> vtBuf;

	//find offset for all tables "cmap" "name" ...
	for(int i = 0; i < ttOffsetTable.uNumOfTables; i++)
	{
		ReadFile(hFile,&findTableDirectory, sizeof(TT_TABLE_DIRECTORY),&dwRead,NULL);

		vtBuf.resize(5);
		memset(&vtBuf[0],0,vtBuf.size());
		strncpy(&vtBuf[0],findTableDirectory.szTag, 4);
		if(strcmp(_strlwr(&vtBuf[0]),"name") == 0)
		{
			bFoundNameTable = TRUE;
			findTableDirectory.uLength = SWAPLONG(findTableDirectory.uLength);
			findTableDirectory.uOffset = SWAPLONG(findTableDirectory.uOffset);
			nameTableDir = findTableDirectory;
			continue;
		}
		if(strcmp(_strlwr(&vtBuf[0]),"head") == 0)
		{
			bFoundHeadTable = TRUE;
			findTableDirectory.uLength = SWAPLONG(findTableDirectory.uLength);
			findTableDirectory.uOffset = SWAPLONG(findTableDirectory.uOffset);
			headTableDir = findTableDirectory;
			continue;
		}
		if(strcmp(_strlwr(&vtBuf[0]),"hhea") == 0)
		{
			bFoundHheaTable = TRUE;
			findTableDirectory.uLength = SWAPLONG(findTableDirectory.uLength);
			findTableDirectory.uOffset = SWAPLONG(findTableDirectory.uOffset);
			hheaTableDir = findTableDirectory;
			continue;
		}
		if(strcmp(_strlwr(&vtBuf[0]),"maxp") == 0)
		{
			bFoundMaxpTable = TRUE;
			findTableDirectory.uLength = SWAPLONG(findTableDirectory.uLength);
			findTableDirectory.uOffset = SWAPLONG(findTableDirectory.uOffset);
			maxpTableDir = findTableDirectory;
			continue;
		}
	}
	//find "maxp"table
	if (bFoundMaxpTable)
	{
		SetFilePointer(hFile,maxpTableDir.uOffset,NULL,FILE_BEGIN);
		TT_MAXP_RECORD maxpTable;
		ReadFile(hFile,&maxpTable,sizeof(TT_MAXP_RECORD),&dwRead,NULL);
		USHORT numGlyphs = SWAPWORD(maxpTable.numGlyphs);
		TCHAR buf[128];
		_stprintf(buf,_T("%d"),numGlyphs);
		fontProp.strGlyphCount = buf; 
		fontProp.strGlyphCount = _T("字模数 ")+fontProp.strGlyphCount+_T(" ");
	}
	//find "hhea" table
	if (bFoundHheaTable)
	{
		SetFilePointer(hFile,hheaTableDir.uOffset,NULL,FILE_BEGIN);
		TT_HHEA_RECORD hheaTable;
		ReadFile(hFile,&hheaTable,sizeof(TT_HHEA_RECORD),&dwRead,NULL);
		FWORD ascender = SWAPWORD(hheaTable.Ascender);
		TCHAR buf[128];
		_stprintf(buf,_T("%d"),ascender);

		fontProp.strTypoAscender=buf;
		fontProp.strTypoAscender = _T("TypoAscender ")+fontProp.strTypoAscender+_T(" ");
		memset(buf,0,128*sizeof(TCHAR));

		FWORD descender = SWAPWORD(hheaTable.Descender);
		_stprintf(buf,_T("%d"),descender);
		fontProp.strTypoDescender=buf;
		fontProp.strTypoDescender = _T("TypoDescender ")+fontProp.strTypoDescender+_T(" ");
		memset(buf,0,128*sizeof(TCHAR));

		FWORD linegap = SWAPWORD(hheaTable.LineGap);
		_stprintf(buf,_T("%d"),linegap);
		fontProp.strLineGap=buf;
		fontProp.strLineGap = _T("LineGap ")+fontProp.strLineGap+_T(" ");
		memset(buf,0,128*sizeof(TCHAR));
	}
	//find "head" table
	if (bFoundHeadTable)
	{
		SetFilePointer(hFile,headTableDir.uOffset,NULL,FILE_BEGIN);
		TT_HEAD_RECORD headTable;
		ReadFile(hFile,&headTable,sizeof(TT_HEAD_RECORD),&dwRead,NULL);
		USHORT unitPerEm = SWAPWORD(headTable.unitsPerEm);
		TCHAR buf[128];
		_stprintf(buf,_T("%d"),unitPerEm);
		fontProp.strUnitsPerEm = buf;
		fontProp.strUnitsPerEm = _T("UnitsPerEM ")+fontProp.strUnitsPerEm+_T(" ");
		memset(buf,0,128*sizeof(TCHAR));
		USHORT lowestRectPPEM= SWAPWORD(headTable.lowestRecPPEM);
		_stprintf(buf,_T("%d"),lowestRectPPEM);
		fontProp.strSmallestRectPixel=buf;
		fontProp.strSmallestRectPixel = _T("SmallestRect ")+fontProp.strSmallestRectPixel+_T(" ");
	}
	//find "naming" table
	if(bFoundNameTable)
	{ 
		SetFilePointer(hFile,nameTableDir.uOffset,NULL,FILE_BEGIN);
		TT_NAME_TABLE_HEADER nameTable; 
		ReadFile(hFile,&nameTable,sizeof(TT_NAME_TABLE_HEADER),&dwRead,NULL);
		nameTable.uRecordCount = SWAPWORD(nameTable.uRecordCount);
		nameTable.uStorageOffset = SWAPWORD(nameTable.uStorageOffset);

		TT_NAME_RECORD ttRecord;
		bFoundNameTable = FALSE;

		for(int i = 0; i < nameTable.uRecordCount; i++)
		{
			ReadFile(hFile,&ttRecord,sizeof(TT_NAME_RECORD),&dwRead,NULL);
			ttRecord.uPlatformID = SWAPWORD(ttRecord.uPlatformID);
			ttRecord.uEncodingID = SWAPWORD(ttRecord.uEncodingID);
			ttRecord.uLanguageID = SWAPWORD(ttRecord.uLanguageID);
			ttRecord.uNameID = SWAPWORD(ttRecord.uNameID);
			ttRecord.uStringLength = SWAPWORD(ttRecord.uStringLength);
			ttRecord.uStringOffset = SWAPWORD(ttRecord.uStringOffset);

			//save position for roll back 
			DWORD dwPos = SetFilePointer(hFile,0,NULL,FILE_CURRENT);  
			SetFilePointer(hFile,nameTableDir.uOffset + ttRecord.uStringOffset + nameTable.uStorageOffset,NULL,FILE_BEGIN);  

			if (ttRecord.uPlatformID==0x03)
			{
				TCHAR* pContent = NULL;
				if (ttRecord.uStringLength%2==1)
				{
					ttRecord.uStringLength+=1;
				}
				pContent = new TCHAR[1+ttRecord.uStringLength/2];
				memset(pContent,0,2+ttRecord.uStringLength);
				ReadFile(hFile,pContent,ttRecord.uStringLength,&dwRead,NULL);  

				//english name
				if(ttRecord.uNameID == 1&&ttRecord.uLanguageID == 0x0409)
				{
					fontProp.strEnglishName = GetFakeUnicodeData(pContent,ttRecord.uStringLength);
					fontProp.strEnglishSampleText=EnglishSampleText;

					fontProp.strPlatform = _T("Windows ");
					kn_string::size_type pos = fontProp.strLanguage.find(_T("English"));
					if (pos==kn_string::npos)
					{
						fontProp.strLanguage.append(_T(" English"));
					}
					fontProp.strEncoding = EncodingTable[ttRecord.uEncodingID];
				}
				//sub family name like italic, oblique
				else if(ttRecord.uNameID == 2&&ttRecord.uLanguageID == 0x0409)
				{
					fontProp.strStyle = GetFakeUnicodeData(pContent,ttRecord.uStringLength);
					fontProp.strStyle = fontProp.strStyle+_T(" ");
				}
				//uniqueID
				else if(ttRecord.uNameID == 3&&ttRecord.uLanguageID == 0x0409)
				{
					fontProp.strUniqueIdentifier = GetFakeUnicodeData(pContent,ttRecord.uStringLength);
					fontProp.strUniqueIdentifier = _T(" ")+fontProp.strUniqueIdentifier+_T(" ");
				}
				//english copyright
				else if(ttRecord.uNameID == 0 && ttRecord.uLanguageID == 0x0409)
				{
					fontProp.strEnglishCopyright = GetFakeUnicodeData(pContent,ttRecord.uStringLength);
				}
				//english trademark
				else if(ttRecord.uNameID == 7 && ttRecord.uLanguageID == 0x0409)
				{
					fontProp.strEnglishTrademark=GetFakeUnicodeData(pContent,ttRecord.uStringLength);
				}
				//version
				else if(ttRecord.uNameID == 5 && ttRecord.uLanguageID == 0x0409)
				{
					fontProp.strVersion =GetFakeUnicodeData(pContent,ttRecord.uStringLength);
				}
				//chianese name
				else if(ttRecord.uNameID == 1&&ttRecord.uLanguageID == 0x0804)
				{
					fontProp.strChineseName = GetUnicodeData(pContent,ttRecord.uStringLength);
					fontProp.strChineseSampleText=ChineseSampleText;

					kn_string::size_type pos = fontProp.strLanguage.find(_T("中文"));
					if (pos==kn_string::npos)
					{
						fontProp.strLanguage.append(_T(" 中文"));
					}
				}
				//chinese copyright
				else if(ttRecord.uNameID == 0 &&ttRecord.uLanguageID == 0x0804)
				{
					fontProp.strChineseCopyright = GetUnicodeData(pContent,ttRecord.uStringLength);
				}
				//chinese trademark
				else if(ttRecord.uNameID == 7 &&ttRecord.uLanguageID == 0x0804)
				{
					fontProp.strChineseTrademark= GetUnicodeData(pContent,ttRecord.uStringLength);
				}
				delete[] pContent;
			}
			SetFilePointer(hFile,dwPos,NULL,FILE_BEGIN);
		}   
	}
	CloseHandle(hFile);

#endif // WIN32
	return ;
}

KFontManager::KFontManager()
{

}

KFontManager::~KFontManager()
{
	for (mapFontNameMap::iterator iter=m_FontNameMap.begin();iter!=m_FontNameMap.end();iter++)
	{ 
		(*iter).second->unref();
	}
	for (mapFontNameMap::iterator iter=m_FontBoldNameMap.begin();iter!=m_FontBoldNameMap.end();iter++)
	{ 
		(*iter).second->unref();
	}
	
	for (mapFontFileMap::iterator iter=m_FontFileMap.begin();iter!=m_FontFileMap.end();iter++)
	{ 
		(*iter).second->unref();
		
	}
	for (std::vector<RETypeface*>::iterator iter=m_FontVector.begin();iter!=m_FontVector.end();iter++)
	{
		(*iter)->unref();
	}
	for (mapFontPropMap::iterator iter=m_FontPropMap.begin();iter!=m_FontPropMap.end();iter++)
	{ 
		delete ((*iter).second);
	}

	m_FontNameMap.clear();
	m_FontBoldNameMap.clear();
	m_FontFileMap.clear();
	m_FontVector.clear();
	m_FontPropMap.clear();
}


RETypeface* KFontManager::GetFontFromName( char* szName, SkTypeface::Style s )
{
#ifdef KOTEI_LINUX
	return GetFontFromFile(_T("./resource/msyh.ttf"));
#endif

	if (sizeof(szName) ==0)
	{
		return NULL;
	}
	string strName = szName;
	if (s == SkTypeface::kBold)
	{//粗体使用m_FontBoldNameMap，m_FontNameMap只有名称没有样式
		if (m_FontBoldNameMap.find(strName)==m_FontBoldNameMap.end() )
		{
			RETypeface* pfontType = RETypeface::CreateFromName(szName, s);
			m_FontBoldNameMap.insert(std::pair<string,RETypeface*>(strName, pfontType));
		}
		return m_FontBoldNameMap[strName];

	}	
	else 
	{
		if (m_FontNameMap.find(strName)==m_FontNameMap.end() )
		{
			RETypeface* pfontType = RETypeface::CreateFromName(szName, s);
			m_FontNameMap.insert(std::pair<string,RETypeface*>(strName, pfontType));
		}
	}
	


	return m_FontNameMap[strName];
	
}

RETypeface* KFontManager::GetFontFromFile( kn_string strFilePath )
{
	if (strFilePath.empty() || strFilePath==_T(""))
	{
		return NULL;
	}
	
	kn_string strFileBak = strFilePath;
	strFilePath = GetCurrentPath();
	strFilePath += strFileBak;	
	
	
	if (m_FontFileMap.find(strFilePath)==m_FontFileMap.end())
	{
		char chFilePath[MAX_PATH] ={0};
		WideCharToMultiByte(CP_ACP, 0, strFilePath.c_str(), -1, chFilePath, MAX_PATH, NULL, NULL);
		RETypeface* pfontType = RETypeface::CreateFromFile(chFilePath);
		m_FontFileMap.insert(std::pair<kn_string,RETypeface*>(strFilePath,pfontType));
		GetFontPropFromFile(strFilePath);
	}
	return m_FontFileMap[strFilePath];
}

RETypeface* KFontManager::GetFontByID( int nFontID )
{
	if (nFontID<m_FontVector.size() && nFontID>=0)
	{
		return m_FontVector.at(nFontID);
	}
	else
	{
		return NULL;
	}
}

int KFontManager::AddFont( RETypeface* pTypeface )
{
	m_FontVector.push_back(pTypeface);
	return (m_FontVector.size()-1);
}

stFontProperty* KFontManager::GetFontPropFromFile( kn_string strFilePath )
{
	if (m_FontPropMap.find(strFilePath)==m_FontPropMap.end())
	{
		stFontProperty* pFontPropPTR = new stFontProperty;
		GetFontPropertyFromFile(strFilePath,*pFontPropPTR);
		m_FontPropMap.insert(std::pair<kn_string,stFontProperty*>(strFilePath,pFontPropPTR));
	}

	return m_FontPropMap[strFilePath];
}


static KFontManager* g_fontManager=NULL;
KFontManager* GetFontManagerSingleton()
{
	if (g_fontManager==NULL)
	{
		g_fontManager = new KFontManager;
	}
	return g_fontManager;
}

extern void DeleteFontManagerSingleton()
{
	if (g_fontManager)
	{
		delete g_fontManager;
	}
	g_fontManager=NULL;
}
