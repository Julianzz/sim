/**
 * 字符串处理工具函数
 * @author jerryzhang@tencent.com
 * @since  2006.04.12 
 */

#include "StringUtility.h"
#include <algorithm>
#include <errno.h>

using namespace std;

/** 功能: 删除在strValue中两头的无效字符 */
string StringUtility::TrimBothSides(const string& strValue, 
									const std::string& strTarget)
{
	if (strValue.size() <= 0)
		return "";
	
	size_t dwBeginPos, dwEndPos;
	size_t dwStrLen = strValue.length();
	
	for (dwBeginPos = 0 ; dwBeginPos < dwStrLen; dwBeginPos++)
	{
		// 如果该字符不在要删除的字符串中
		if (-1 == strTarget.find(strValue.at(dwBeginPos))) 
			break;
	}
	
	for (dwEndPos = dwStrLen - 1 ; dwEndPos > 0 ; dwEndPos--)
	{
		// 如果该字符不在要删除的字符串中
		if (-1 == strTarget.find(strValue.at(dwEndPos))) 
			break;
	}
	
	if (dwBeginPos > dwEndPos)
		return "";
	
	return strValue.substr(dwBeginPos, dwEndPos - dwBeginPos + 1);
}

/** 功能: 删除在strValue中两头的无效字符 */
string StringUtility::Trim(const string& strValue, 
									const std::string& strTarget)
{
	return TrimBothSides(strValue,strTarget);	
}
/** 功能: 删除在strValue中左边的无效字符 */
string StringUtility::TrimLeft(const string& strValue, 
							   const std::string& strTarget)
{
	if (strValue.size() <= 0)
		return "";
	
	size_t dwBeginPos;
	size_t dwStrLen = strValue.length();
	for (dwBeginPos = 0 ; dwBeginPos < dwStrLen; dwBeginPos++)
	{
		// 如果该字符不在要删除的字符串中
		if (-1 == strTarget.find(strValue.at(dwBeginPos))) 
			break;
	}
	
	if (dwBeginPos == dwStrLen - 1)
		return "";
	
	return strValue.substr(dwBeginPos);
}

/** 功能: 删除在strValue中右边的无效字符 */
string StringUtility::TrimRight(const string& strValue, 
								const std::string& strTarget)
{
	if (strValue.size() <= 0)
		return "";
	
	size_t dwEndPos;
	size_t dwStrLen = strValue.length();
	for (dwEndPos = dwStrLen - 1 ; dwEndPos > 0 ; dwEndPos--)
	{
		// 如果该字符不在要删除的字符串中
		if (-1 == strTarget.find(strValue.at(dwEndPos))) 
			break;
	}
	
	if (dwEndPos == 0)
		return "";
	
	return strValue.substr(0, dwEndPos + 1);
}

/** 功能: 删除在中文字符串两头的中文空格 */
std::string StringUtility::TrimChineseSpace(const std::string& strValue)
{
    if (strValue.size() < 2)
        return strValue;

    int dwHead = 0;
    int dwTail = (int)strValue.size() - 2;

    // 确定字符串前面的中文空格的个数
    while ((dwHead <= (int)strValue.size() - 2) && 0xA1 == (unsigned char)strValue[dwHead] && 0xA1 == (unsigned char)strValue[dwHead+1])
        dwHead += 2;

    // 确定字符串前面的中文空格的个数
    while (dwTail >= 0 && 0xA1 == (unsigned char)strValue[dwTail] && 0xA1 == (unsigned char)strValue[dwTail+1])
        dwTail -= 2;

	if (dwTail < dwHead)
		return "";
	else
	    return strValue.substr(dwHead, dwTail - dwHead + 2);
}
/** 功能: 删除在中英文字符串两头的中英文空格 */
std::string StringUtility::TrimBothChEnSpace(const std::string& strValue)
{
	if (strValue.size() < 2)
		return strValue;

	size_t dwHead = 0;
	size_t dwTail = strValue.size()-1;
	// 确定字符串前面的中文空格的个数
	while (dwHead <= strValue.size()-2)
	{
		if (strValue[dwHead] == ' ')
		{
			dwHead ++;
		}
		else if (strValue[dwHead] == (char)0xa1 && strValue[dwHead+1] == (char)0xa1)
		{
			dwHead += 2;
		}
		else
		{
			break;
		}
	}
	// 确定字符串后面的中英文空格的个数
	while (dwTail > 0)
	{
		if (strValue[dwTail] == ' ')
		{
			dwTail --;
		}
		else if (strValue[dwTail-1] == (char)0xa1 && strValue[dwTail] == (char)0xa1)
		{
			dwTail -= 2;
		}
		else
		{
			break;
		}        
	}
	if (dwTail < dwHead)
		return "";
	return strValue.substr(dwHead, dwTail - dwHead + 2);
}
/** 功能: 把一个字符串划分成多个字符串
 *  参数: 
 *  输入参数 const std::string& strMain         主字符串
 *  输入参数 const  std::string& strSpliter     字符串分界符号
 *  输出参数 std::vector<std::string>& strList 分解后的结果
 */
void StringUtility::SplitString(const std::string& strMain, 
				 const  std::string strSpliter, 
				 std::vector<std::string>& strList,
                 bool bReserveNullString)
{
	strList.clear();

	if (strMain.empty() || strSpliter.empty())
		return;
	
	// 从下面开始都使用处理后的string
	string strMainTemp = strMain;
	
	// 要划分的字符串或分界符为空，说明调用该函数不正确
	if (strMainTemp.empty() || strSpliter.empty())
		return;
	
	size_t nPos = strMainTemp.find(strSpliter);
    string strTemp;

    while (string::npos != nPos)
	{
        strTemp = strMainTemp.substr(0,nPos);
        if (!strTemp.empty())
            strList.push_back(strTemp);

        // 如果设置了保留空字段
        if (bReserveNullString && strTemp.empty())
            strList.push_back(strTemp);

		strMainTemp = strMainTemp.substr(nPos+strSpliter.size());
		nPos = strMainTemp.find(strSpliter);
	}
	
	strMainTemp = TrimBothSides(strMainTemp, " \r\n\t");

    // 如果设置了保留空字段
    if (bReserveNullString)
	{
		// 如果是要求保留空字段，那么不论最后的是否为空都应该保留
        strList.push_back(strMainTemp);
	}
	else
	{
		// 最后一项要求不为空才加入集合
		if(!strMainTemp.empty())
			strList.push_back(strMainTemp);
	}
}

/** 功能:  简单的分词函数：把一个字符串按分隔符号划分成多个字符串 */
void StringUtility::SplitString(const std::string& strMain, 
				 std::vector<std::string>& strList)
{
	strList.clear();
	if (strMain.empty())
		return;

    string strTemp = strMain;
    string::size_type dwBeginPos;
    string::size_type dwEndPos;
	do
	{
        dwBeginPos = 0;
        while(dwBeginPos < strTemp.size() && IsSeparator(strTemp[dwBeginPos]))
            dwBeginPos++;

        dwEndPos = dwBeginPos;
        while(dwEndPos < strTemp.size() && !IsSeparator(strTemp[dwEndPos]))
            dwEndPos++;

        if (dwEndPos > dwBeginPos)
        {
            strList.push_back(strTemp.substr(dwBeginPos, dwEndPos - dwBeginPos));
            strTemp = strTemp.substr(dwEndPos);
        }
        else
        {
            break;
        }
	}
    while ( strTemp.size() > 0);
}

/** 把字符串A中的子串B替换为C */
string StringUtility::ReplaceStr(const string& strValue, 
								 const string& oldStr, 
								 const string& newStr)
{
	string strRet = strValue;

	// 找到第一个Target
	size_t dwPos = strValue.find(oldStr);  
	size_t dwNextPos;

    while (string::npos != dwPos)
	{
		dwNextPos = dwPos + oldStr.size();
		if (strRet.size() >= dwNextPos)
		{
			strRet.erase(dwPos, oldStr.size());
			strRet.insert(dwPos, newStr);
		}

		dwPos = strRet.find(oldStr, dwPos+newStr.size());
	}
	return strRet;
}

/** 把一个集合中的字符转换为对应的另一个集合中字符
 *  例如：把 <变为[, > 变为]
 *        strNew = ReplaceCharSet(strOld, "<>", "[]");
 */
string StringUtility::ReplaceCharSet(const string& strOld, 
							 string strFromSet, 
							 string strToSet)
{
	std::string strResult;
	for (string::size_type i = 0; i < strOld.size(); i++)
	{
		char ch = strOld[i];

		string::size_type dwPos = strFromSet.find(ch);
		if (dwPos == string::npos)
		{
			// 没有找到，说明不需要替换，直接复制过去
			strResult += ch;
		}
		else if (dwPos < strToSet.size())
		{
			// 找到，替换
			strResult += strToSet[dwPos];
		}
	}
	return strResult;

}

/** 替换字符串中的某些子字符串 */
string StringUtility::ReplaceStrSet(const string& strRawData, 
								    map<string, string>& mSet)
{
	string strRet = strRawData;
	std::map<std::string, std::string>::iterator it;
	for (it = mSet.begin(); it != mSet.end(); it++)
	{
		strRet = ReplaceStr(strRet, it->first, it->second);
	}
	return strRet;
}

/** 把字符串A中的子符删除 */
std::string StringUtility::RemoveChar(string& strRaw, char ch)
{
	int dwRawLen = strRaw.size();
	int dwNewLen = 0;
	char* pcNew = new char[dwRawLen+1];
	string strNew;

	strNew.reserve(dwRawLen);
	for(size_t i = 0; i < dwRawLen; i++)
	{
		if (strRaw[i] > 0)
		{
			if (ch != strRaw[i])
				pcNew[dwNewLen++] = strRaw[i];
		}
		else
		{
			if (i+1 < dwRawLen)
			{
				pcNew[dwNewLen++] = strRaw[i];
				pcNew[dwNewLen++] = strRaw[i+1];
				i++;
			}
			else
			{
				// 这是最后有半个汉字的情况
				pcNew[dwNewLen++] = strRaw[i];
			}
		}
	}
	pcNew[dwNewLen] = '\0';

	strNew.assign(pcNew);
	delete pcNew;

	return strNew;
}

/** 把字符串A中的某个子字符串删除 */
std::string StringUtility::RemoveStr(std::string& strRawData, std::string strSubStr, bool bNeedSeparator)
{
	string::size_type dwPos;
	string strTemp = strRawData;
	string strRet;
	
	do
	{
		dwPos = strTemp.find(strSubStr);
        if (dwPos == string::npos)
		{
			strRet += strTemp;
			break;
		}
        else
		{
            if (bNeedSeparator)
            {
                bool bHasPreSeparator = false;
                bool bHasPostSeparator = false;

                if (0 == dwPos)
                    bHasPreSeparator = true;
                else if (dwPos > 0 && IsSeparator(strTemp[dwPos-1]))
                    bHasPreSeparator = true;

                if (dwPos+strSubStr.size() >= strTemp.size())
                    bHasPostSeparator = true;
                else if (IsSeparator(strTemp[dwPos+strSubStr.size()]))
                    bHasPostSeparator = true;

                if (bHasPreSeparator && bHasPostSeparator)
                {
                    // 前后都有分界符，那么认为是可以删除的
			        strRet += strTemp.substr(0, dwPos);
			        strTemp = strTemp.substr(dwPos+strSubStr.size());
                }
                else
                {
                    // 如果前或后没有分界符，那么认为是不可以删除的
			        strRet += strTemp.substr(0, dwPos+strSubStr.size());
			        strTemp = strTemp.substr(dwPos+strSubStr.size());                    
                }
            }
            else
            {
			    strRet += strTemp.substr(0, dwPos);
			    strTemp = strTemp.substr(dwPos+strSubStr.size());
            }
		}
	}
	while (!strTemp.empty());
	return strRet;

}

/** 删除字符串中的某些子字符串 */
string StringUtility::RemoveStrSet(const string& strRawData, 
									                         set<string>& sStrSet,
                                                             bool bNeedSeparator)
{
    std::set<std::string>::iterator it;
    string strTmp = strRawData;


    it = sStrSet.end();
    for(it--; it != sStrSet.begin(); it--)
        strTmp = RemoveStr(strTmp, *it, bNeedSeparator);

    if (it == sStrSet.begin())
        strTmp = RemoveStr(strTmp, *it, bNeedSeparator);
    return strTmp;
}

/** 删除一个字符串中的一段子字符串，包括strBegin和strEnd也会被删除 */
string StringUtility::DeleteSubstr(const std::string& strRawData, 
								   std::string strBegin, 
								   std::string strEnd)
{
	string::size_type dwPosBegin;
	string::size_type dwPosEnd;
	string strTemp = strRawData;
	string strRet;
	
	do
	{
		dwPosBegin = strTemp.find(strBegin);
		dwPosEnd = strTemp.find(strEnd);

        if ((dwPosBegin != string::npos) && (dwPosEnd != string::npos))
		{
            if (dwPosEnd > dwPosBegin)
            {
                // 开始和结束字符串都找到的情况
                if (dwPosBegin > 0)
                    strRet += strTemp.substr(0, dwPosBegin);
                if (dwPosEnd+strEnd.size() < strTemp.size())
                {
                    size_t dwNewLen = strTemp.size()-dwPosEnd-strEnd.size();
                    strTemp = strTemp.substr(dwPosEnd+strEnd.size(), dwNewLen);
                }
                else
                    strTemp = "";
                continue;
            }
            else
            {
                strRet += strTemp.substr(0, dwPosBegin);
                strTemp = strTemp.substr(dwPosBegin);
            }
		}
        else
        {
            strRet += strTemp;
            break;
        }
	}
	while (!strTemp.empty());
	return strRet;
}

/** 从一个字符串中提取出关注的部分，例如"[*]" */
string StringUtility::ExtractSubstr( const string& strInput, string& strPattern)
{
    // 如果参数为空，或者参数为'*'，或者在参数中没有找到*这个子串，则返回源串
    if ((strPattern.empty())  || ("*" == strPattern))
        return strInput;

    // 找到 strInput 中的*号所在的位置
    string::size_type pos = strPattern.find("*");
    if (string::npos == pos)
        return strInput;

    string strLeft  = strPattern.substr(0, pos);
    string strRight = strPattern.substr(pos + 1, strPattern.length() - pos + 1);

    // 在输入串中找*左边子串的位置
    string::size_type leftPos = 0;
    string::size_type rightPos =  strInput.length() - 1;

    if (!strLeft.empty())
    {
        leftPos = strInput.find(strLeft);
        if (string::npos != leftPos)
        {        
            leftPos += strLeft.length();
        }
        else
        {
            leftPos = 0;
        }
    }

    if (!strRight.empty())
    {
        rightPos = strInput.rfind(strRight);
        if (string::npos != rightPos)
        {
            rightPos -= 1;
        }
        else
        {
            rightPos =  strInput.length() - 1;
        }
    }

    return strInput.substr(leftPos, rightPos - leftPos + 1);
}

/** 把字符串中一些需要转义的字符做转义处理 */
string StringUtility::QuoteWild(const char* strRaw)
{
	if (NULL == strRaw)
		return "";

	char* pcDest = new char[strlen(strRaw)*2];
  	int i = 0;
	int j = 0;

	while (strRaw[i]) 
	{
		switch((unsigned char)strRaw[i]) 
		{
			case '\n':
				pcDest[j++] = '\\';
				pcDest[j++] = 'n';
				break;
			case '\t':
				pcDest[j++] = '\\';
				pcDest[j++] = 't';
				break;
			case '\r':
				pcDest[j++] = '\\';
				pcDest[j++] = 'r';
				break;
			case '\b':
				pcDest[j++] = '\\';
				pcDest[j++] = 'b';
				break;
			case '\'':
				pcDest[j++] = '\\';
				pcDest[j++] = '\'';
				break;
			case '\"':
				pcDest[j++] = '\\';
				pcDest[j++] = '\"';
				break;
			case '\\':
				pcDest[j++] = '\\';
				pcDest[j++] = '\\';
				break;
			default:
				// 小于32是控制字符，都转换为空格(32)
				if ((unsigned char)strRaw[i] >= 32) 
					pcDest[j++] = strRaw[i];
				else 
					pcDest[j++] =32;
		}
		i++;
	}
	pcDest[j] = 0;

	string strRet;
	strRet.assign(pcDest);
	delete [] pcDest;
	
	return strRet;
}

/** 字符串摘要：取前面若干个字符，主要是防止出现半个汉字和英文以及数字的截断　*/
std::string StringUtility::AbstractString(std::string& strOldStr, int dwLen, std::string& strLeftStr)
{
    if (dwLen <= 0)
    {
        strLeftStr = strOldStr;
        return "";
    }
    else if (strOldStr.empty())
    {
        strLeftStr = "";
        return "";
    }
    else if ((string::size_type)dwLen >= strOldStr.size())
    {
        strLeftStr = "";
        return strOldStr;
    }
  
    int dwOffset = 0;
    while(dwOffset <= dwLen)
    {
        if(strOldStr[dwOffset] < 0)
        {
            // 处理中文
            if (dwOffset+2 <= dwLen)
                dwOffset += 2;
            else  // 容纳不小
                break;
        }
        else // 下面是ascii字符处理
        {
            if (IsAlnum(strOldStr[dwOffset]))
            {
                // 检查该行是否可以显示下该数字或字母
                int dwWordLen = 1;
                while(IsAlnum(strOldStr[dwOffset+dwWordLen]))
                    dwWordLen++;

                // 该词的长度大于摘要的长度，那么不得不截断处理
                if (dwWordLen > dwLen)
                {
                    dwOffset = dwLen;
                    break;
                }

                if (dwOffset+dwWordLen <= dwLen)
                    dwOffset += dwWordLen;
                else
                    break;
            }
            else
            {
                // 非控制字符\数字\字母之外的可以分开
                if (dwOffset+1 <= dwLen)
                    dwOffset++;
                else
                    break;
            } // end else
        }
    } // end while

    // 集中在这里返回是为了避免多个出口
    strLeftStr = strOldStr.substr(dwOffset);
    return strOldStr.substr(0, dwOffset);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 字符串内容判断

bool StringUtility::CmpNoCase(char c1, char c2)
{
	return toupper(c1) == toupper(c2);
}

/** 在字符串中查找子符串，不区分大小写 */
string::size_type StringUtility::FindNoCase(string strMain, string strSub, size_t dwFrom)
{
	string::iterator it;
	it = search(strMain.begin()+dwFrom, strMain.end(), strSub.begin(),strSub.end(), CmpNoCase);
	if( it == strMain.end())
		return string::npos;
	else
		return (it - strMain.begin());
}
/*搜索子字符串，主字符串中可能含有‘\0’字符，也可能不以0字符结束*/
char * StringUtility::FindNoCaseEnStrWithLength(const char * pcBegin,const char * pcEnd,const char * pcSubStr)
{
	if (pcBegin == NULL || pcEnd == NULL || pcSubStr == NULL || pcEnd <= pcBegin)
	{
		return NULL;
	}
	char * pcCur = (char *)pcBegin;
	int dwSubLen = (int)strlen(pcSubStr);
	if (dwSubLen > pcEnd - pcBegin)
	{
		return NULL;
	}
	while (pcCur <= pcEnd)
	{
		if (*pcCur < 0)
		{
			pcCur += 2;
		}
		else 
		{
			if(0 ==STRNCASECMP(pcCur,pcSubStr,dwSubLen))
			{
				if (pcCur > pcEnd)
				{
					return NULL;
				}
				return pcCur;
			}
		
			pcCur ++;
		}
	}
	return NULL;

}
/** 在字符串中查找子符串，不区分大小写 */
string::size_type StringUtility::RFindNoCase(string strMain, string strSub, size_t dwFrom)
{
    string::iterator it;
    it = find_end(strMain.begin(), strMain.begin()+dwFrom, strSub.begin(),strSub.end(), CmpNoCase);
    if( it == strMain.end())
        return string::npos;
    else
        return (it - strMain.begin());
}

/** 在中文字符串中开始查找一个宽字节字 */
char* StringUtility::WStrChr(const char* pcMain, const char* pcWChar)
{
    if(NULL == pcMain || NULL == pcWChar || pcWChar[0] >= 0 || pcWChar[1] >= 0)
        return NULL;

    char* pcTempMain = (char*)pcMain;
    char  c1, c2;

    do
    {
        c1 = *pcTempMain;

        // 对于宽字符，那么应该是跳过2个字符
        if (c1 < 0)
        {
            if (NULL == *(pcTempMain+1))
                return NULL;
            c2 = *(pcTempMain+1);
            if (c1 != pcWChar[0] || c2 != pcWChar[1])
            {
                pcTempMain += 2;
                continue;
            }
            else
                break; // 找到的情况
        }
        else
        {
            pcTempMain++;
            if (*pcTempMain != NULL)
                continue;
            else
                return NULL;
        }
    } while(NULL != *pcTempMain);
    return pcTempMain;
}

/** 在中文字符串中开始反向查找一个宽字节字 */
char* StringUtility::RWStrChr(const char* pcMain, const char* pcWChar)
{
    if(NULL == pcMain || NULL == pcWChar || pcWChar[0] >= 0 || pcWChar[1] >= 0)
        return NULL;

    size_t dwStrLen = strlen(pcMain);
    if (dwStrLen < 2)
        return NULL;
    char* pcTempMain = (char*)(pcMain+dwStrLen-2);
    char  c1, c2;

    do
    {
        c1 = *pcTempMain;

        // 对于宽字符，那么应该是跳过2个字符
        if (c1 < 0)
        {
            if (*(pcTempMain+1) >= 0)
            {
                pcTempMain--;
                continue;
            }
            c2 = *(pcTempMain+1);
            if (c1 != pcWChar[0] || c2 != pcWChar[1])
            {
                pcTempMain -= 2;
                continue;
            }
            else
                return pcTempMain;
        }
        else
        {
            pcTempMain--;
        }
    } while(pcTempMain >= pcMain);
    return NULL;
}

/** 在中文字符串中开始反向查找一个宽字节字 */
string::size_type StringUtility::RWStrChr(const string& strMain, const std::string strWChar)
{
    char* pcPos =  RWStrChr(strMain.c_str(), strWChar.c_str());
    if (NULL == pcPos)
        return string::npos;
    else
        return (string::size_type)(pcPos-(char*)strMain.c_str());
}

/** 在中文字符串中查找宽字节的子符串 */
char* StringUtility::WFind(const char* pcMain, const char* pcSub)
{
    if(NULL == pcMain || NULL == pcSub)
        return NULL;

    unsigned char *pSrc = (unsigned char *)pcMain;
    unsigned char *pDst = (unsigned char *)pcSub;
    unsigned char *pTmp;

    if (!*pDst)
        return (char*)pcMain;

    while (true)
    {
        while (NULL != *pSrc && *pSrc != *pDst)
        {
            if (*pSrc < 0x80)
                pSrc ++;
            else
                pSrc += 2;
        }

        if (!*pSrc)
            return NULL;

        pTmp = pSrc;

        while (*pDst && *pSrc == *pDst)
        {
            pSrc++;
            pDst++;
        };

        if (!*pDst)
            return (char *)pTmp;
        else
        {
            if (*pTmp > 0x80)
                pSrc = pTmp + 2;
            else
                pSrc = pTmp + 1;
            pDst = (unsigned char *)pcSub;
        }
    }

    return NULL;
}

/** 检测字符串中是否包含了某些子字符串 */
bool StringUtility::IsSubStrContained(string& strRawData, set<string>& sSubStr)
{
	set<string>::iterator it;
	for (it = sSubStr.begin(); it != sSubStr.end(); it++)
	{
		if (-1 != strRawData.find(*it))
			return true;
	}

	return false;
}

/** 返回当前位置开始的下一个字符 */
string StringUtility::GetNextChar(string& str, size_t dwCurPos)
{
	if(str.size() < dwCurPos+1)
		return "";
	if(str.at(dwCurPos) < 0)
	{
		return str.substr(dwCurPos, 2);
	}
	else
	{
		return str.substr(dwCurPos, 1);
	}
}

/** 获得下一个有效的结束位置, 一般用于找html tag的结束标签 */
char* StringUtility::GetNextValidEndPos(char* pcCurPos, char* pcEnd, char cEndSign)
{
    char* pcTagEndPos = pcCurPos;
    bool   bIsInQuot = false;
    int      dwEndCnt = 0;
    char* apcTagEndPos[4];
    #define MAX_END_LINE_CNT 3

    do 
    {
        if (*pcTagEndPos == cEndSign)
        {
            // 开始找结束">"， 一直遇到一个不在引号里面的">"
            if (!bIsInQuot)
            {
                pcTagEndPos++;
                break;
            }
            else
            {
                if (dwEndCnt < MAX_END_LINE_CNT)
                    apcTagEndPos[dwEndCnt++] = pcTagEndPos+1;
                else 
                    return apcTagEndPos[dwEndCnt-1];
            }
        }
        // 遇到非转义引号的情况
        else if ((*pcTagEndPos == '\"' || *pcTagEndPos == '\'') && (*(pcTagEndPos-1) != '\\'))
        {
            if (!bIsInQuot)
            {
                dwEndCnt = 0;
                bIsInQuot = true;
            }
            else
                bIsInQuot = false;
        }
        else if (*pcTagEndPos == '\n')
        {
            // 处理非法的情况，比如：<a href="./default.htm"" class="b">北京日报</a>
            if (dwEndCnt < MAX_END_LINE_CNT)
                apcTagEndPos[dwEndCnt++] = pcTagEndPos+1;
            else 
                return apcTagEndPos[dwEndCnt-1];
        }
        pcTagEndPos++;
    }
    while (pcTagEndPos != pcEnd && *pcTagEndPos != '\0');

    return pcTagEndPos;
}

/** 获得一个字符串的字符个数, 中文等双字节字符只算1个 */
int StringUtility::GetCharCount(string& strWord)
{
	int dwCharCount = 0;
	for (size_t i = 0; i < strWord.size(); ++i, ++dwCharCount)
	{
		if (strWord[i] < 0) 
		{
			++i;
		}
	}
	return dwCharCount;
}

/** 获得一个字符串中某个字符的个数 */
int StringUtility::GetCharCount(string& strWord, char c)
{
	int dwCharCount = 0;
	for (size_t i = 0; i < strWord.size(); ++i)
	{
		if (strWord[i] > 0 && strWord[i] == c) 
		{
            ++dwCharCount;
		}
	}
	return dwCharCount;
}

/** 统计一个字符串中数字和字符等个数 */
int StringUtility::StatStr(std::string& strWord, int& dwAlpha, int& dwDigit)
{
    dwAlpha = 0;
    dwDigit = 0;

    for (size_t i = 0; i < strWord.size(); ++i)
    { 
        if ((strWord[i] >= 'a' && strWord[i] <= 'z') || (strWord[i] >= 'A' && strWord[i] <= 'Z'))
            dwAlpha++;
        else if (strWord[i] >= '0' && strWord[i] <= '9')
            dwDigit++;
    }
    return 0;
}

/** 获得一个字符串中某个子字符串的个数 */
int StringUtility::GetSubStrCnt(std::string& strMain, std::string strSubStr, bool bNoCase)
{
    size_t dwPos = 0;
    int dwSubStrCnt = 0;

    dwPos -= strSubStr.size();
    do 
    {
        if (bNoCase)
            dwPos = StringUtility::FindNoCase(strMain, strSubStr, dwPos+strSubStr.size());
        else
            dwPos = strMain.find(strSubStr, dwPos+strSubStr.size());	
        if (string::npos != dwPos)
            dwSubStrCnt++;
        else
            break;
    } while(true);
    return dwSubStrCnt; 
}

/** 获得一个字符串中中文字符的个数 */
int StringUtility::GetChineseCharCount(string& strWord)
{
	int dwCharCount = 0;
	for (size_t i = 0; i < strWord.size(); )
	{
		if (strWord[i] < 0) 
		{
            i += 2;
            ++dwCharCount;
		}
        else
        {
            ++i;
        }
	}
	return dwCharCount;
}

/** 是否是数字IP */
bool StringUtility::IsDigitIp(std::string strIp)
{
	vector<string> strList;
	int dwTmp;
		
	SplitString(strIp, ".", strList);
    if (4 != strList.size())
    	return false;
    
    for(int i = 0; i < 4; i++)
    {
    	if (strList[i].size() > 3 || strList[i].size() == 0 )
    		return false;
    	
		for (size_t j = 0; j < strList[i].size(); j++)
		
		{			
			if (!(strList[i][j] >= '0' && strList[i][j] <= '9'))				
				return false;
    	}
    	dwTmp = Str2Int(strList[i]);
    	if (dwTmp > 255)
    		return false;
    }
	return true;	
}

/**
* 判断一个字符串是不是只是包含空格字符,检查的空格字符包括' ', '\t'
* @param   pStr 要检查的字符串
* @return  bool 如果被检查字符串只是包含空格字符,就返回PORT_TRUE,
*          否则返回PORT_FALSE
*/
bool StringUtility::IsWhiteString(char const *pStr)
{
    while((*pStr == ' ')|| (*pStr == '\t'))
        pStr ++;
    if(*pStr == '\0')
        return true;
    else
        return false;
}

/** 判断一个字符串是否是完全由英文字符串组成 */
bool StringUtility::IsEnglishString(const char* pStr, bool bLowLineAsAlpha)
{
	char* p = (char*)pStr;
    while (*p != '\0')
	{
        if (bLowLineAsAlpha)
        {
            if( !ISALPHA(*p) && ('_' != *p))
                return false;
        }
        else
        {
            if( !IsAlpha(*p))
                return false;
        }
		p++;	
	}

    return true;  
}

/** 判断一个字符串是否全是数字组成 */
bool StringUtility::IsDigitString(const char* pStr)
{
	char* p = (char*)pStr;
    while (*p != '\0')
	{
        if( !IsDigit(*p))
            return false;
		p++;	
	}

    return true;  
}

/** 判断一个字符串是否全是16进制数字组成 */
bool StringUtility::IsHexDigitString(const char* pStr)
{
	char* p = (char*)pStr;
	while (*p != '\0')
	{
		if (!((*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F') || (*p >= '0' && *p <= '9')))
		{
			return false;
		}
		p++;	
	}
	return true;  
}
/** 计算GB2312汉字的序号,返回值是0到6767之间的数 */
int StringUtility::GetGB2312HanziIndex(string& str)
{
	return ((unsigned char)str[0] - 176)*94 + (unsigned char)str[1] - 161;
}

/** 判断一个字符串的前两个字节是否是GB2312汉字 */
bool StringUtility::IsGB2312Hanzi(string& str)
{
	if(str.size() < 2)
		return false;
	int dwIndex = GetGB2312HanziIndex(str);
	if (dwIndex >=0 && dwIndex < 6768)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/** 判断一个字符串是否完全由GB2312的汉字组成 */
bool StringUtility::IsOnlyComposedOfGB2312Hanzi(string& strWord)
{
	//如果字符串的长度不是2的倍数,则返回false
	if (strWord.size()%2 != 0)
	{
		return false;
	}

    string strTmp;

	//逐个判断每两个字节是否组成一个汉字
	for (size_t i = 0; i+1 < strWord.size(); i=i+2)
	{
        strTmp = strWord.substr(i);
		if (!IsGB2312Hanzi(strTmp)) 
		{
			return false;
		}
	}
	return true;
}

/** 判断字符是否是数字，windows下的isdigit()有bug */
bool StringUtility::IsDigit(char c)
{
    if (c >= '0' && c <= '9')
        return true;
    else
        return false;
}

/** 判断字符是否是空格,windows下的isspace()有bug */
bool StringUtility::IsSpace(char c)
{
    if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
        return true;
    else
        return false;
}

/** 判断字符是否是数字或字母,windows下的isalnum()有bug */
bool StringUtility::IsAlnum(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        return true;
    else
        return false;
}

/** 判断字符是否是字母，windows下的isalpha()有bug */
bool StringUtility::IsAlpha(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    else
        return false;
}

/** 判断字符是否是分界符 */
bool StringUtility::IsSeparator(char c)
{
    string strSeparator(" \t\r\n-_[](){}:.,=*&^%$#@!~?<>/|'\"");
    if (string::npos == strSeparator.find(c))
        return false;
    else
        return true;
}

/** 判断字符串是否是正常的URL */
bool StringUtility::IsValidUrl(char* pcUrl)
{
	if (NULL == pcUrl)
		return false;

	char* p = pcUrl;
	while (*p != '\0')
	{
		if (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')
			return false;
		else if ((*p == '<' || *p == '>') && (*(p-1) != '\\'))
			return false;
		else if (*p == '*' || *p == '|')
			return false;
		else if (*p == '/' && (STRNCASECMP(p+1, "Javascript:", 11) == 0))
			return false;
		else if ((*p == 'j' || *p == 'J') && (STRNCASECMP(p+1, "avascript:", 10) == 0))
			return false;

		p++;
	}
	return true;
}

/** 把字符转为小写, 不用系统函数tolower是因为它有bug */
char StringUtility::CharToLower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 32;
	else
		return c;
}

/** 把字符转为大写, 不用系统函数toupper是因为它有bug */
char StringUtility::CharToUpper(char c)
{
	if (c >= 'a' && c <= 'z')
		return c - 32;
	else
		return c;
}

/** 把字符串转为小写 */
std::string StringUtility::ToLower(const char *pszValue)
{
    std::string  strRet = pszValue;
    size_t dwLen = strRet.size();
    for (unsigned long i = 0 ; i < dwLen; i ++)
    {
		if (strRet[i] < 0)
		{
			i++;
		}
		else 
		{
            strRet[i] = CharToLower(pszValue[i]);
		}

    }
	
    return strRet;
}

/** 把字符串转为大写 */
std::string StringUtility::ToUpper(const char *pszValue)
{
    std::string  strRet = pszValue;
    size_t dwLen = strRet.size();
    for (unsigned long i = 0 ; i < dwLen; i ++)
    {
		if (pszValue[i] < 0)
		{
			i++;
		}
		else 
		{
			strRet[i] = CharToUpper(pszValue[i]);
		}

    }
	
    return strRet;
}

/** 把一个int值转换成为字符串*/
string StringUtility::Int2Str(int dwValue)
{
    char szBuf[16];
    sprintf(szBuf,"%d",dwValue);
    return string(szBuf);
}

/** 把一个unsigned int值转换成为字符串*/
string StringUtility::Int2Str(unsigned int  dwValue)
{
    char szBuf[16];
    sprintf(szBuf,"%u",dwValue);
    return string(szBuf);
}

/**把字符串转为unsigned long long型数*/
unsigned long long StringUtility::Str2ULongLong(const std::string& str)
{
	unsigned long long dwRet = 0;
	if(str[0] == '\0')
	{
		return dwRet;
	}
	//printf("%d:%s$\n",str.length(), str.c_str());
	stringstream ss;
	ss << str;
	ss >> dwRet;
	ss.str("");
	//cout<<"Size of stream = "<<ss.str().length()<<endl;
	return dwRet;
}

/** 把一个long long值转换成为字符串*/
string StringUtility::LongLong2Str(long long ddwValue)
{
    char szResult[64];
    szResult[0] = '\0';

#ifdef WIN32
    _i64toa(ddwValue, szResult, 10); 
#else
    sprintf (szResult, "%llu", ddwValue);
#endif
    return string(szResult);
}

/** 把一个long long值转换成为字符串*/
string StringUtility::ULongLong2Str(unsigned long long ddwValue)
{
    char szResult[64];
    szResult[0] = '\0';

#ifdef WIN32
    _i64toa(ddwValue, szResult, 10); 
#else
    sprintf (szResult, "%llu", ddwValue);
#endif
    return string(szResult);
}

/** 把浮点数转为字符串 */
string StringUtility::Double2Str(double lfValue)
{
    char szBuf[80];
    sprintf(szBuf, "%lf", lfValue);
    return string(szBuf);
}

/** 以十六进制的格式打印，调试时候使用，不能打印40k以上的 */
void StringUtility::BinPrint(char * title, unsigned char * p,int len)
{
    char szBuf[1024*16];

    memset(szBuf, 0, sizeof(szBuf));

    ToHxString(szBuf, title, p, len);
    printf("%s", szBuf);
}

/** 以十六进制的格式打印，调试时候使用 */
void StringUtility::ToHxString(char* pcBinStr, char * title, unsigned char * p,int len)
{
    if (NULL == pcBinStr || NULL == title || NULL == p)
        return;

    int i;
    int j = 0;

    sprintf(pcBinStr, "\n--------------------------------\n");

    if (NULL != title)
    {
        sprintf(pcBinStr+strlen(pcBinStr), "%s len = %d", title, len);
        sprintf(pcBinStr+strlen(pcBinStr),"\n--------------------------------\n");
    }

    for(i = 0; i < len; i++)
    {
        j++;
        if(5 == j)  
        {
            sprintf(pcBinStr+strlen(pcBinStr),"| ");
            j = 1;
        }
        sprintf(pcBinStr+strlen(pcBinStr), "%02x ", p[i]);
    }
    sprintf(pcBinStr+strlen(pcBinStr),"\n--------------------------------\n\n");
}

/** 自动以可显示的格式打印，调试时候使用 */
void StringUtility::PrintWithAutoArrange(char* pText, int dwLineWidth)
{
    string strText(pText);
    string strLeft;
    string strLine;
    int dwRepeatTimes = 0; // 用来防止死循环的计数器

    do
    {
        dwRepeatTimes++;
        strLine = AbstractString(strText, dwLineWidth, strLeft);
        strText = strLeft;
        cout << strLine << endl;
    }
    while (strText.size()>0 && dwRepeatTimes < 10000);
}

/** 以十六进制的格式打印，调试时候使用 */
void StringUtility::Str2HexStr(char* pcHexStr, unsigned char * p,int len)
{
    int i;
    int j = 0;
    char buf[1024*512];

    memset(buf, 0, sizeof(buf));

    sprintf(buf, "\r\n--------------------------------\r\n");

    for(i = 0; i < len; i++)
    {
        j++;
        if(5 == j)  
        {
            sprintf(buf+strlen(buf),"| ");
            j = 1;
        }
        if(i % 20 == 0)  
        {
            sprintf(buf+strlen(buf),END_LINE);
        }

        sprintf(buf+strlen(buf), "%02x ", p[i]);
    }
    sprintf(buf+strlen(buf),"\r\n--------------------------------\r\n");
    sprintf(pcHexStr, "%s", buf);
}

/** 把一个set输出为一个字符串 */
string StringUtility::OutputSet(set<string>& strSet)
{
	string strRet;
	set<string>::iterator it;
	for (it = strSet.begin(); it != strSet.end(); it++)
	{
		strRet += *it;
		strRet += END_LINE;
	}
	return strRet;
}

/** 把一个Vector输出为一个字符串 */
string StringUtility::OutputVector(vector<string>& strVec)
{
	string strRet;
	vector<string>::iterator it;
	for (it = strVec.begin(); it != strVec.end(); it++)
	{
		strRet += *it;
		strRet += END_LINE;
	}
	return strRet;
}

/** 把一个map输出为一个字符串 */
string StringUtility::OutputMap(map<string, string>& strMap)
{
	string strRet;
	map<string, string>::iterator it;
	for (it = strMap.begin(); it != strMap.end(); it++)
	{
		strRet += it->first + string(" = ") + it->second;
		strRet += END_LINE;
	}
	return strRet;
}

/** 把一个map输出为一个字符串 */
string StringUtility::OutputMap(map<string,  int>& strMap)
{
	string strRet;
	map<string, int>::iterator it;
	for (it = strMap.begin(); it != strMap.end(); it++)
	{
		strRet += it->first + string(" = ");
        strRet += Int2Str(it->second);
		strRet += END_LINE;
	}
	return strRet;
}

/** 把一个map输出为一个字符串 */
std::string StringUtility::OutputMultiMap(std::multimap<std::string, std::string>& strMap)
{
	string strRet;
	multimap<string, string>::iterator it;
	for (it = strMap.begin(); it != strMap.end(); it++)
	{
		strRet += it->first + string(" = ") + it->second;
		strRet += END_LINE;
	}
	return strRet;
}

/** 把一个字符转换成16进制的形式 */
std::string StringUtility::Char2Hex(unsigned char ch)
{
	std::string str("%");
	char high;
	char low;

	high = ch / 16;
	if(high > 9)
		high = 'A' + high - 10;
	else
		high = '0' + high;

	low  = ch % 16;
	if(low > 9)
		low = 'A' + low - 10;
	else
		low = '0' + low;

	str += high;
	str += low;

	return str;
}

/** 判断一个字符是否在一个字符串中 */
bool StringUtility::IsCharInStr(unsigned char ch, const std::string& strTarget)
{
	for (size_t t = 0; t < strTarget.size(); t++)
	{
		if (ch == strTarget[t])
			return true;
	}

	return false;
}

/** 把字符串转为整型数 */
int StringUtility::Str2Int(const std::string& str)
{
	int dwRet = 0;

	for (size_t i = 0; i < str.size() && IsDigit(str[i]); ++i)
		dwRet = dwRet*10 + str[i] - '0'; 

	return dwRet;
}

/** 把字符串转为整型数 */
unsigned int StringUtility::Str2UInt(const std::string& str)
{
	unsigned int dwRet = 0;

	for (size_t i = 0; i < str.size(); ++i)
		dwRet = dwRet*10 + str[i] - '0'; 

	return dwRet;
}
/** 把字符串转为LongLong型数 */
long long StringUtility::Str2LongLong(const std::string& str)
{
	long long llRet = 0;


	if (str.size() == 0)
	{
		return 0;
	}
	else
	{
		int j = 0;
		while (!IsDigit(str[j]) && str[j] == '-' && j < (int)str.size())
		{
			j++;
		}
		if (j >= (int)str.size())
		{
			return 0;
		}
		if (str[j] == '-')
		{
			llRet = -1;
		}
		else
		{
			llRet= str[j] - '0';
		}
		for (size_t i = j; i < str.size()-1 && IsDigit(str[i]); ++i)
			llRet = llRet*10 + str[i] - '0'; 
		
	}
	return llRet;
}
/** 把字符串转为浮点数 */
double StringUtility::Str2Double(const std::string& str)
{
	// atof在windows和linux上定义相同
	return atof(str.c_str());
}

/** 把16进制的字符串转为整型数 */
int StringUtility::HexStr2long(const char* pcHex)
{
	int dwRet = 0;
	int dwTmp;
	size_t dwLen = strlen(pcHex);

	for (size_t i = 0; i < dwLen; ++i)
	{
		if (pcHex[i] >= 'A' && pcHex[i] <= 'F')
			dwTmp = pcHex[i] - 'A' + 10;
		else if (pcHex[i] >= 'a' && pcHex[i] <= 'f')
			dwTmp = pcHex[i] - 'a' + 10;
		else
			dwTmp = pcHex[i] - '0';
		
		dwRet = dwRet*16 + dwTmp; 
	}
	return dwRet;
}

/** 把一个字符串中的连续空格转为单个空格 */
string StringUtility::SingleBlank(const string &strRaw) 
{
	unsigned int dwCount = 0;
	bool bIsFirstSpace = true;
	const char *ptr = strRaw.c_str();

	string strRet(strRaw.length(), ' ');

	// 跳过字符串开始的空格
	while ((*ptr > 0) && ISSPACE(*ptr)) 
        ++ptr;
	
	while (*ptr)
	{
		if ((*ptr > 0) && ISSPACE(*ptr))
		{
			if (bIsFirstSpace)
			{
				bIsFirstSpace = false;
				strRet[dwCount++] = ' ';
			}
		}
		else
		{
			bIsFirstSpace = true;
			strRet[dwCount++] = *ptr;
		}
		
		++ptr;
	}

	// 删除字符串结束的空格
	string::size_type dwPos;
	dwPos = strRet.find_last_not_of(' ', dwCount);
	if (dwPos != string::npos)
		strRet.erase(dwPos+1);
	else
	{
		dwPos = 0;
		strRet.erase(dwPos);
	}

	return strRet;
}

/** 删除html或xml格式的注释 <!-- --> */
string StringUtility::StripComments(const string& strRawFile) 
{
	string strNewFile;
	strNewFile.reserve(strRawFile.size());

	const char *ptr = strRawFile.c_str();
	const char *end = ptr + strRawFile.length();

	bool bIsInsideComment = false;
	while(1) 
	{
		if(!bIsInsideComment) 
		{
			if(ptr  + 3 < end) 
			{
				if(*ptr == '<' && *(ptr+1) == '!' && *(ptr+2) =='-' && *(ptr + 3) == '-' ) 
				{
					bIsInsideComment = true;
				}
			}
		} 
		else 
		{
			if(ptr + 2 < end) 
			{
				if(*ptr == '-' && *(ptr+1) == '-' && *(ptr+2) == '>' ) 
				{
					bIsInsideComment = false;
					ptr += 3;
				}
			}
		}
		if(ptr == end) 
			break;
		if(!bIsInsideComment) 
			strNewFile += *ptr;
		ptr++;
	}

	strNewFile.resize(strNewFile.size());

	return strNewFile;
}

/** 计算一个字符串的hash值 */
unsigned int StringUtility::GetStrHashValue(const char* pcStr)
{    
    unsigned int dwMagic = 31;    
    const unsigned char* p = (const unsigned char*) pcStr;    
    unsigned int h = 0;    
    for (; *p != '\0'; ++p) 
    {        
        h = h * dwMagic + *p;    
    }    
    return h;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 字符串模糊匹配实现（就是简单的正则表达式以及范围匹配功能）

/** 检查一个字符串是否能匹配到一个通配符号，外部使用接口
 *  MatchWildcard("he*o","hello"): true
 *  MatchWildcard("he*p","hello"): false
 *  MatchWildcard("he??o","hello"): true
 *  MatchWildcard("he?o","hello"): false
 *  MatchWildcard("[a-z][0-9]","h7"): true
 *  MatchWildcard("172.16.*","172.16.68.29"): true
 */
bool StringUtility::MatchWildcard(const string& strWild, const string& strMatch)
{
  return _MatchRemainder(strWild, strWild.begin(), strMatch, strMatch.begin());
}

// 计算一个字符是否在一个集合中，例如'8'属于"0-9"， 内部函数
bool StringUtility::_MatchSet(const string& strPattern, char strMatch)
{
	// 实际的字符集，例如:strPattern为"a-z", 那么strRealCharSet是"abcd...z"
	string strRealCharSet; 
	string::const_iterator i;
	for (i = strPattern.begin(); i != strPattern.end(); ++i)
	{
		switch(*i)
		{
		case '-':
		{
			if (i == strPattern.begin())
				strRealCharSet += *i;
			else if (i+1 == strPattern.end())
		  		return false;
		    else
			{
				// 第一个字符已经在字符集中，所以首先删除(集合可能是空的)
				strRealCharSet.erase(strRealCharSet.end()-1);
				char last = *++i;
				for (char ch = *(i-2); ch <= last; ch++)
				{
				  strRealCharSet += ch;
				}
			}
			break;
		}
		case '\\':
			if (i+1 == strPattern.end()) 
				return false;
			strRealCharSet += *++i;
			break;
		default:
			strRealCharSet += *i;
			break;
		}
	}
	string::size_type dwResult = strRealCharSet.find(strMatch);
	return dwResult != string::npos;
}

// 匹配剩下的通配符，递归调用, 内部函数
bool StringUtility::_MatchRemainder(const string& strWild, 
							string::const_iterator itWild, 
							const string& strMatch, 
							string::const_iterator itMatch)
{
#ifdef _DEBUG_
	cout << "MatchRemainder called at " << *itMatch << " with wildcard " << *itWild << endl;
#endif
	while (itWild != strWild.end() && itMatch != strMatch.end())
	{
#ifdef _DEBUG_
		cout << "trying to strMatch " << *itMatch << " with wildcard " << *itWild << endl;
#endif
		switch(*itWild)
		{
		case '*':
		{
			++itWild;
			for (string::const_iterator i = itMatch; i != strMatch.end(); ++i)
			{
				// 处理"*"在通配符的结束处的情况，这种情况下就没有剩下的了
				if (itWild == strWild.end())
				{
				  if (i == strMatch.end()-1)
					return true;
				}
				else if (_MatchRemainder(strWild, itWild, strMatch, i))
				{
				  return true;
				}
			} // end for
			return false;
		} // end case '*'
		case '[':
		{
			// 找到集合的结束处
			bool bFound = false;
			string::const_iterator it = itWild + 1;
			for (; !bFound && it != strWild.end(); ++it)
			{
				switch(*it)
				{
				case ']':
				{
					// 拿字符*itMatch到集合"[...]"中去匹配（括号被去掉）
					if (!_MatchSet(strWild.substr(itWild - strWild.begin() + 1, it - itWild - 1), *itMatch))
						return false;
					bFound = true;
					break;
				}
				case '\\':

					// 转义字符不能在结尾
					if (it == strWild.end()-1)
						return false;
					++it;
					break;
				default:
					break;
				}
			} // end for
			if (!bFound)
				return false;
			++itMatch;
			itWild = it;
			break;
		} // end case '['
		case '?':
			++itWild;
			++itMatch;
			break;
		case '\\':
			if (itWild == strWild.end()-1)
				return false;
			++itWild;
			if (*itWild != *itMatch)
				return false;
			++itWild;
			++itMatch;
			break;
		default:
			if (*itWild != *itMatch)
				return false;
			++itWild;
			++itMatch;
			break;
		} // end switch
	} // end while

	return (itWild == strWild.end()) && (itMatch == strMatch.end());
}

/** 功能: 计算两个字符串的相似度
*    输入参数 const std::string& strA      字符串1
*    输入参数 const  std::string& strB     字符串2
*/
int StringUtility::CalcSimilar(const string& strA, const string& strB)
{
    static string strStopWord("，。的地得一是了");
    set<string> sWord;
    int dwWordCount = 0;
    string strWord;
    size_t dwHitCount = 0;
    size_t dwAlnumBeginPos;

    for (size_t i = 0; i < strA.size(); ++i)
    {
        if (strA[i] < 0) 
        {
            strWord = strA.substr(i, 2);
            ++i;
            if (string::npos == strStopWord.find(strWord))
                sWord.insert(strWord);
        }
        else if (StringUtility::IsAlnum(strA[i] ))
        {
            dwAlnumBeginPos = i;

            while (i+1 < strA.size() && StringUtility::IsAlnum(strA[i+1]))
                i++;
            strWord = strA.substr(dwAlnumBeginPos, i-dwAlnumBeginPos+1);
			strWord = ToLower(strWord.c_str());
            if (string::npos == strStopWord.find(strWord))
                sWord.insert(strWord);
        }
    } // end for
	
	string strBTemp = ToLower(strB.c_str());
    for (set<string>::iterator it = sWord.begin(); it != sWord.end(); it++)
    {		
        if (string::npos != strBTemp.find(*it))
            dwHitCount++;            
    } // end for

    if (0 == sWord.size())
        return 0;
    else
        return (int)(dwHitCount*100/sWord.size());
}

/** 功能: 计算两个字符串的相似度(高性能版本)
*    输入参数 const std::string& strA      字符串1
*    输入参数 const  std::string& strB     字符串2
*/
int StringUtility::CalcSimilarEx(const char* pcStrA, int dwLenA, const char* pcStrB, int dwLenB)
{
	if (NULL == pcStrA || NULL == pcStrB)
		return 0;

	//static string strStopWord("，。的地得一是了");
	//添加了省市区县
	char szChBitmap[1024*64];
	char szEnBitmap[1024*64];
	int dwWordCount = 0;
	size_t dwHitCount = 0;
	char* p = (char*)pcStrA;
	const char* pcEndA = pcStrA + dwLenA;
	const char* pcEndB = pcStrB + dwLenB;
	unsigned short wChWord;
	unsigned short wEnWord;

	memset(szChBitmap, 0, sizeof(szChBitmap));
	memset(szEnBitmap, 0, sizeof(szEnBitmap));

	while (p < pcEndA)
	{
		if (*p < 0) 
		{
			wChWord = *(unsigned short*)p;
			p += 2;
			if (wChWord != 0xC4B5 && wChWord != 0xA1CA && wChWord != 0xD0CA && wChWord != 0xD8CF && wChWord != 0xF8C7 && wChWord != 0xACA3 && wChWord != 0xA3A1 && wChWord != 0xD8B5 && wChWord != 0xC3B5 
				&& wChWord != 0xBBD2 && wChWord != 0xC7CA && wChWord != 0xCBC1)
			{
				if (szChBitmap[wChWord] == 0)
				{
					szChBitmap[wChWord] = 1;
					dwWordCount++;
				}
				
			}
		}
		else
		{		
			if (ISALPHA(*p) || ISDIGIT(*p))
			{
				wEnWord = 0;
				do 
				{
					wEnWord *= 16777619; 
					if (*p >= 'A' && *p <= 'Z')
						wEnWord ^= (unsigned short)((unsigned char)(*p + 32)); 
					else
						wEnWord ^= (unsigned short)((unsigned char)(*p)); 
					p++;
				} while(p < pcEndA && (ISALPHA(*p) || ISDIGIT(*p)));
				if (szEnBitmap[wEnWord] == 0)
				{
					szEnBitmap[wEnWord] = 1;
					dwWordCount++;
				}				
			}
			else
			{
				p++;
			}
		}
	} // end while (p < pcEndA)

	p =(char*) pcStrB;
	while (p < pcEndB)
	{
		if (*p < 0) 
		{
			wChWord = *(unsigned short*)p;
			p += 2;
			if (wChWord != 0xC4B5 && wChWord != 0xA1CA && wChWord != 0xD0CA && wChWord != 0xD8CF && wChWord != 0xF8C7 && wChWord != 0xACA3 && wChWord != 0xA3A1 && wChWord != 0xD8B5 && wChWord != 0xC3B5 
				&& wChWord != 0xBBD2 && wChWord != 0xC7CA && wChWord != 0xCBC1)
			{
				if (szChBitmap[wChWord] == 1)
				{
					szChBitmap[wChWord] = 0;
					dwHitCount++;
				}
			}
		}
		else
		{		
			if (ISALPHA(*p) || ISDIGIT(*p))
			{
				wEnWord = 0;
				do 
				{
					wEnWord *= 16777619; 
					if (*p >= 'A' && *p <= 'Z')
						wEnWord ^= (unsigned short)((unsigned char)(*p + 32)); 
					else
						wEnWord ^= (unsigned short)((unsigned char)(*p)); 
					p++;
				} while(p < pcEndB && (ISALPHA(*p) || ISDIGIT(*p)));

				if (szEnBitmap[wEnWord] == 1)
				{
					szEnBitmap[wEnWord] = 0;
					dwHitCount++;
				}
			}
			else
			{
				p++;
			}
		}
	} // end while (p < pcEndB)

	if (0 == dwWordCount)
		return 0;
	else
		return (int)dwHitCount*100/dwWordCount;
}
/** 本文件结束 */
