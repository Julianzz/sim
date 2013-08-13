/**
 * �ַ����������ߺ���
 * @author jerryzhang@tencent.com
 * @since  2006.04.12
 */

#ifndef     __STRING_UTILITY_H__
#define     __STRING_UTILITY_H__


#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>


// ���ִ�Сд���ַ����ȽϺ���
#ifdef WIN32
    #ifndef STRNCASECMP
    #define STRNCASECMP			_strnicmp
    #endif
    #ifndef STRCASECMP
    #define STRCASECMP			_stricmp
    #endif
	#define snprintf            _snprintf 
#else 
    #ifndef STRNCASECMP
    #define STRNCASECMP			strncasecmp
    #endif
    #ifndef STRCASECMP
    #define STRCASECMP			strcasecmp
    #endif
#endif

#ifdef WIN32
    #define END_LINE "\r\n"
#else
    #define END_LINE "\n"
#endif

#ifndef ISALPHA_DIGIT_SPACE

#define ISALPHA_DIGIT_SPACE
#define ISALPHA(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define ISDIGIT(c) (c >= '0' && c <= '9')
#define ISSPACE(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#endif

class StringUtility
{
public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ַ������ݴ���

    /** ����: ɾ����strValue����ͷ����Ч�ַ� */
    static std::string TrimBothSides(const std::string& strValue, 
									 const std::string& strTarget = " \t\r\n");
    /** ����: ɾ����strValue����ͷ����Ч�ַ� */
    static std::string Trim(const std::string& strValue, 
									 const std::string& strTarget = " \t\r\n");
    /** ����: ɾ����strValue����ߵ���Ч�ַ� */
    static std::string TrimLeft(const std::string& strValue, 
								const std::string& strTarget = " \t\r\n");

    /** ����: ɾ����strValue���ұߵ���Ч�ַ� */
    static std::string TrimRight(const std::string& strValue, 
								 const std::string& strTarget = " \t\r\n");

    /** ����: ɾ���������ַ�����ͷ�����Ŀո� */
    static std::string TrimChineseSpace(const std::string& strValue);

    /** ����: ɾ�����ַ������ߵ�������Ӣ�Ŀո��ַ��� */
    static std::string TrimBothChEnSpace(const std::string& strValue);


    /** ����: ��һ���ַ������ֳɶ���ַ��� */
    static void SplitString(const std::string& strMain, 
				    const  std::string strSpliter, 
				    std::vector<std::string>& strList,
                    bool bReserveNullString = false);

    /** ����:  �򵥵ķִʺ�������һ���ַ������ָ����Ż��ֳɶ���ַ��� */
    static void SplitString(const std::string& strMain, 
				    std::vector<std::string>& strList);

	/** ����: ��һ���ַ������ֳɶ���ַ��� */
	static void Split(const std::string& strMain, 
		const  std::string strSpliter, 
		std::vector<std::string>& strList,
		bool bReserveNullString = false)
	{
		SplitString(strMain, strSpliter, strList,bReserveNullString );
	}

	/** ����:  �򵥵ķִʺ�������һ���ַ������ָ����Ż��ֳɶ���ַ��� */
	static void Split(const std::string& strMain, 
		std::vector<std::string>& strList)
	{
		SplitString(strMain, strList);
	}


    /** ���ַ���A�е��Ӵ�B�滻ΪC */
    static std::string ReplaceStr(const std::string& strValue, 
						   const std::string& oldStr, 
						   const std::string& newStr);

	/** ��һ�������е��ַ�ת��Ϊ��Ӧ����һ���������ַ�
	 *  ���磺�� <��Ϊ[, > ��Ϊ]
	 *        strNew = ReplaceCharSet(strOld, "<>", "[]");
	 */
	static std::string ReplaceCharSet(const std::string& strOld, 
								 std::string strFromSet, 
								 std::string strToSet);

	/** �滻�ַ����е�ĳЩ���ַ��� */
	static std::string ReplaceStrSet(const std::string& strRawData, 
									 std::map<std::string, std::string>& mSet);

    /** ���ַ���A�е��ӷ�ɾ�� */
    static std::string RemoveChar(std::string& strRawData, char ch);

    /** ���ַ���A�е�ĳ�����ַ���ɾ�� */
    static std::string RemoveStr(std::string& strRawData, std::string strSubStr, bool bNeedSeparator = false);

	/** ɾ���ַ����е�ĳЩ���ַ��� */
	static std::string RemoveStrSet(const std::string& strRawData, 
									 std::set<std::string>& sStrSet, bool bNeedSeparator = false);

	/** ɾ��һ���ַ����е�һ�����ַ��� */
	static std::string DeleteSubstr(const std::string& strRawData, std::string strBegin, std::string strEnd);

    /** ��һ���ַ�������ȡ����ע�Ĳ��֣�����"[*]" */
    static std::string ExtractSubstr(const std::string& strInput, std::string& strPattern);

	/** ���ַ�����һЩ��Ҫת����ַ���ת�崦�� */
	static std::string QuoteWild(const char* strRaw);

    /** �ַ���ժҪ��ȡǰ�����ɸ��ַ�����ֹ���ְ�����ֺ�Ӣ���Լ����ֵĽضϡ�*/
    static std::string AbstractString(std::string& strOldStr, int dwLen, std::string& strLeftStr); 

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ַ��������ж�

    /** �Ƚ��ַ��������ִ�Сд */
	static bool CmpNoCase(char c1, char c2);
	/*�������ַ��������ַ����п��ܺ��С�\0���ַ���Ҳ���ܲ���0�ַ�����*/
	static char * FindNoCaseEnStrWithLength(const char * pcBegin,const char * pcEnd,const char * pcSubStr);
	/** ���ַ����в����ӷ����������ִ�Сд */
	static std::string::size_type FindNoCase(std::string strMain, std::string strSub, size_t dwFrom = 0);

    /** ���ַ����в����ӷ����������ִ�Сд */
    static std::string::size_type RFindNoCase(std::string strMain, std::string strSub, size_t dwFrom);

    /** �������ַ����п�ʼ����һ�����ֽ��� */
    static char* WStrChr(const char* pcMain, const char* pcWChar);

    /** �������ַ����п�ʼ�������һ�����ֽ��� */
    static char* RWStrChr(const char* pcMain, const char* pcWChar);

    /** �������ַ����п�ʼ�������һ�����ֽ��� */
    static std::string::size_type RWStrChr(const std::string& strMain, const std::string strWChar);

    /** �������ַ����п�ʼ���ҿ��ֽڵ��ӷ��� */
    static char* WFind(const char* pcMain, const char* pcSub);

	/** ���һ���ַ������ַ����� */
    static int GetCharCount(std::string& strWord);

    /** ���һ���ַ�����ĳ���ַ��ĸ��� */
    static int GetCharCount(std::string& strWord, char c);

    /** ͳ��һ���ַ��������ֺ��ַ��ȸ��� */
    static int StatStr(std::string& strWord, int& dwAlpha, int& dwDigit);

    /** ���һ���ַ�����ĳ�����ַ����ĸ��� */
    static int GetSubStrCnt(std::string& strMain, std::string strSubStr, bool bNoCase = true);

    /** ���һ���ַ����������ַ��ĸ��� */
    static int GetChineseCharCount(std::string& strWord);

    /** ���ص�ǰλ�ÿ�ʼ����һ���ַ� */
    static std::string GetNextChar(std::string& str, size_t dwCurPos);

    /** �����һ����Ч�Ľ���λ��, һ��������html tag�Ľ�����ǩ */
    static char* GetNextValidEndPos(char* pcCurPos, char* pcEnd, char cEndSign = '>');

    /** ����ַ������Ƿ������ĳЩ���ַ��� */
    static bool IsSubStrContained(std::string& strRawData, std::set<std::string>& sSubStr);

    /** �Ƿ�������IP */
    static bool IsDigitIp(std::string strIp);

    /** �ж�һ���ַ����Ƿ��ǿհ��ַ� */
    static bool IsWhiteString(char const *pStr);

    /** �ж�һ���ַ����Ƿ�ȫ����Ϊ��ĸ,  ���һ��������ʾ�Ƿ�Ҫ��'_'������ĸ */
    static bool IsEnglishString(const char* pStr, bool bLowLineAsAlpha = false);

    /** �ж�һ���ַ����Ƿ�ȫ��������� */
    static bool IsDigitString(const char* pStr);

	/** �ж�һ���ַ����Ƿ�ȫ��16����������� */
	static bool IsHexDigitString(const char* pStr);

    /** ����GB2312���ֵ����,����ֵ��0��6767֮����� */
    static int GetGB2312HanziIndex(std::string& str);

    /** �ж�һ���ַ�����ǰ�����ֽ��Ƿ���GB2312���� */
    static bool IsGB2312Hanzi(std::string& str);

	/** �ж�һ���ַ����Ƿ���ȫ��GB2312�ĺ������ */
    static bool IsOnlyComposedOfGB2312Hanzi(std::string& strWord);

	/** �ж��ַ��Ƿ��ǿո�windows�µ�isspace()��bug */
    static bool IsSpace(char c);

	/** �ж��ַ��Ƿ������ֻ���ĸ��windows�µ�isalnum()��bug */
    static bool IsAlnum(char c);

	/** �ж��ַ��Ƿ������֣�windows�µ�isdigit()��bug */
    static bool IsDigit(char c);

	/** �ж��ַ��Ƿ�����ĸ��windows�µ�isalpha()��bug */
    static bool IsAlpha(char c);

	/** �ж��ַ��Ƿ��Ƿֽ�� */
    static bool IsSeparator(char c);

	/** �ж��ַ����Ƿ���������URL */
    static bool IsValidUrl(char* pcUrl);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ַ�����Сдת��

	/** ���ַ�תΪСд */
    static char CharToLower(char c);

	/** ���ַ�תΪ��д */
    static char CharToUpper(char c);

    /** ���ַ���תΪСд */
    static std::string ToLower(const char *pszValue);

    /** ���ַ���תΪ��д */
    static std::string ToUpper(const char *pszValue);

	///////////////////////////////////////////////////////////////////////
	// �ַ�������������������ʮ��������֮���ת��
	//str 2 long long
	static long long Str2LongLong(const std::string& str);

    /** ��һ��intֵת����Ϊ�ַ���*/
    static std::string Int2Str(int dwValue);

    /** ��һ��unsigned intֵת����Ϊ�ַ���*/
    static std::string Int2Str(unsigned int  dwValue);

    /** ���ַ���תΪ������ */
    static int Str2Int(const std::string& str);

    /** ���ַ���תΪ�޷��������� */
    static unsigned int Str2UInt(const std::string& str);

	/**���ַ���תΪunsigned long long����*/
	static unsigned long long Str2ULongLong(const std::string& str);

    /** ��һ��long longֵת����Ϊ�ַ���*/
    static std::string LongLong2Str(long long ddwValue);
    
    static std::string ULongLong2Str(unsigned long long ddwValue);

    /** ���ַ���תΪ������ */
    static double Str2Double(const std::string& str);

    /** �Ѹ�����תΪ�ַ��� */
    static std::string Double2Str(double);

    /** ��16���Ƶ��ַ���תΪ������ */
    static int HexStr2long(const char* pcHex);

    /** ��ʮ�����Ƶĸ�ʽ��ӡ������ʱ��ʹ�� */
    static void Str2HexStr(char* pcHexStr, unsigned char * p,int len);

    /** ��һ���ַ�ת����16���Ƶ���ʽ */
    static std::string Char2Hex(unsigned char ch);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����ʱ�õ����ַ�����ӡ

    /** ��ʮ�����Ƶĸ�ʽ��ӡ������ʱ��ʹ�� */
    static void ToHxString(char* pcBinStr, char * title, unsigned char * p,int len);

    /** ��ʮ�����Ƶĸ�ʽ��ӡ������ʱ��ʹ�� */
    static void BinPrint(char * title, unsigned char * p,int len);

    /** �Զ��Կ���ʾ�ĸ�ʽ��ӡ������ʱ��ʹ�� */
    static void PrintWithAutoArrange(char* pText, int dwLineWidth = 80);

    /** ��һ��set���Ϊһ���ַ��� */
    static std::string OutputSet(std::set<std::string>& strSet);

    /** ��һ��Vector���Ϊһ���ַ��� */
    static std::string OutputVector(std::vector<std::string>& strVec);

    /** ��һ��map���Ϊһ���ַ��� */
    static std::string OutputMap(std::map<std::string, std::string>& strMap);

    /** ��һ��map���Ϊһ���ַ��� */
    static std::string OutputMap(std::map<std::string, int>& strMap);

    /** ��һ��multimap���Ϊһ���ַ��� */
    static std::string OutputMultiMap(std::multimap<std::string, std::string>& strMap);

    /** �ж�һ���ַ��Ƿ���һ���ַ����� */
    static bool IsCharInStr(unsigned char ch, const std::string& strTarget);

    /** ��һ���ַ����е������ո�תΪ�����ո� */
    static std::string SingleBlank(const std::string& strRaw); 

	/** ɾ��html��xml��ʽ��ע�� <!-- -->, �ȼ���DeleteSubstr(str, "<!--", "-->"), ��������Ҫ�� */
	static std::string StripComments(const std::string& strRawFile);

    /** ����һ���ַ�����hashֵ */
    static unsigned int GetStrHashValue(const char* pcStr);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ַ���ģ��ƥ��ʵ�֣����Ǽ򵥵��������ʽ�Լ���Χƥ�书�ܣ�

    /** ���һ���ַ����Ƿ���ƥ�䵽һ��ͨ�����
    *  MatchWildcard("he*o","hello"): true
    *  MatchWildcard("he*p","hello"): false
    *  MatchWildcard("he??o","hello"): true
    *  MatchWildcard("he?o","hello"): false
    *  MatchWildcard("[a-z][0-9]","h7"): true
    *  MatchWildcard("172.16.*","172.16.68.29"): true
    */
    static bool MatchWildcard(const std::string& strWild, const std::string& strMatch);

    /** ����: ���������ַ��������ƶ�
    *    ������� const std::string& strA      �ַ���1
    *    ������� const  std::string& strB     �ַ���2
    */
    static int CalcSimilar(const std::string& strA, const std::string& strB);
	static int CalcSimilarEx(const char* pcStrA, int dwLenA, const char* pcStrB, int dwLenB);

private:

    // ����һ���ַ��Ƿ���һ�������У�����'8'����"0-9"
    static bool _MatchSet(const std::string& strPattern, char strMatch);

    // ƥ��ʣ�µ�ͨ������ݹ����
    static bool _MatchRemainder(const std::string& strWild, 
							    std::string::const_iterator itWild, 
							    const std::string& strMatch, 
							    std::string::const_iterator itMatch);

};

#endif

/** ���ļ����� */