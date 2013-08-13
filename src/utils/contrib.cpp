#include "contrib.h"
#include <cstdio>
#include <string>

#define HALF_CHAR_NUMS 93

string g_strEnglish[HALF_CHAR_NUMS] = {
	 "!", "\"", "#", "￥", "%", "&","\'", "(", ")", "*", "+", ",", "-", ".", "/",  
"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?",
"@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
"P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_",
"'", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
"p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}"};

// 中文数字
string strChineseDigit[] =  {"一", "二", "三", "四", "五", "六", "七", "八", "九", "〇",
	"零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖"};

// 中文数字单位
string strChineseUnit[] =  {"十","百", "千", "万", "亿", "拾", "佰", "仟", "萬", "億"};




string Int2Str(int dwValue)
{
    char szBuf[16];
    sprintf(szBuf,"%d",dwValue);
    return string(szBuf);
}

/***************************************************************
 * 功能描述：判断一个字符串是否是中文的数量
 * 输入参数：
 * 输出参数：
 * 返回值  ：
 ***************************************************************/
bool IsChineseDigit(std::string& strData)
{
	for (int i = 0; i < 19; i++)
	{
		if (strData == strChineseDigit[i])
			return true;
	}
	return false;
}

/***************************************************************
 * 功能描述：判断一个字符串是否是中文的数量单位
 * 输入参数：
 * 输出参数：
 * 返回值  ：
 ***************************************************************/
bool IsChineseUnit(std::string& strData)
{
	for (int i = 0; i < 10; i++)
	{
		if (strData == strChineseUnit[i])
			return true;
	}
	return false;
}

/***************************************************************
 * 功能描述：字符串中的大写数字转为小写数字, 只能处理大写的整数
 * 输入参数：
 * 输出参数：
 * 返回值  ：
 ***************************************************************/
std::string Decapitalized(std::string& strRawData)
{
	string::size_type    dwPos = 0;
	string strRet;
	int    dwResult = 0;
	int    dwTmp = 0;
	bool   bIsChineseDigit;
	bool   bIsChineseUnit;
	int    dwPreUnit = 0;
	string strTmp;
	bool bIsHasUnit = false;
	for (int i = 0;i < strRawData.size();i++)
	{
		if(strRawData[i] < 0)
		{
			strTmp =  strRawData[i++];
			strTmp += strRawData[i];
			if (IsChineseUnit(strTmp))
			{
				bIsHasUnit = true;
				break;
			}			
		}
	}
	while(!bIsHasUnit && dwPos < strRawData.size())
	{
		// 对于不需要转换的其他中文
		if (strRawData[dwPos] < 0)
		{
			strTmp =  strRawData[dwPos++];
			strTmp += strRawData[dwPos++];

			bIsChineseDigit = IsChineseDigit(strTmp);			
			if (!bIsChineseDigit)
			{
				// 把非中文数字之前的数字转为英文数字
				if (-1 != dwTmp)
					dwResult += dwTmp;
				if (0 != dwResult)
					strRet += Int2Str(dwResult);

				strRet += strTmp;
				dwResult = 0;
				dwTmp = -1;
				continue;
			}
			else
			{
				if ((strTmp == "一") || (strTmp == "壹"))
					dwTmp = 1;
				else if ((strTmp == "二") || (strTmp == "贰"))
					dwTmp = 2;
				else if ((strTmp == "三") || (strTmp == "叁"))
					dwTmp = 3;
				else if ((strTmp == "四") || (strTmp == "肆"))
					dwTmp = 4;
				else if ((strTmp == "五") || (strTmp == "伍"))
					dwTmp = 5;
				else if ((strTmp == "六") || (strTmp == "陆"))
					dwTmp = 6;
				else if ((strTmp == "七") || (strTmp == "柒"))
					dwTmp = 7;
				else if ((strTmp == "八") || (strTmp == "捌"))
					dwTmp = 8;
				else if ((strTmp == "九") || (strTmp == "玖"))
					dwTmp = 9;
				else 
					dwTmp = 0;
				dwResult*=10;
				dwResult += dwTmp;
			}			
		}
		else
		{
			// 把非中文数字之前的数字转为英文数字
			if (-1 != dwTmp)
				dwResult += dwTmp;	
			if (0 != dwResult)
				strRet += Int2Str(dwResult);

			dwTmp = -1;
			dwResult = 0;

			//　ASCII字符
			strRet += strRawData[dwPos++];
		}
	}
	dwPos = 0;

	bool bTenStartUnit = true;
	bool bIsStartDigit = false;
	while(bIsHasUnit && dwPos < strRawData.size())
	{
		// 对于不需要转换的其他中文
		if (strRawData[dwPos] < 0)
		{
			strTmp =  strRawData[dwPos++];
			strTmp += strRawData[dwPos++];

			bIsChineseDigit = IsChineseDigit(strTmp);
			bIsChineseUnit  = IsChineseUnit(strTmp);
			if (bIsChineseDigit  || bIsChineseUnit)
			{
				if(!bIsStartDigit)
				{
					if (strTmp != "十" && strTmp != "拾")
					{
						bTenStartUnit = false;
					}
				}
				bIsStartDigit = true;
			}

			if (!bIsChineseDigit && !bIsChineseUnit)
			{
				// 把非中文数字之前的数字转为英文数字
				if (-1 != dwTmp)
					dwResult += dwTmp;
				if (0 != dwResult)
					strRet += Int2Str(dwResult);

				strRet += strTmp;
				dwResult = 0;
				dwTmp = -1;
				continue;
			}
			else if (bIsChineseDigit)
			{
				if ((strTmp == "一") || (strTmp == "壹"))
					dwTmp = 1;
				else if ((strTmp == "二") || (strTmp == "贰"))
					dwTmp = 2;
				else if ((strTmp == "三") || (strTmp == "叁"))
					dwTmp = 3;
				else if ((strTmp == "四") || (strTmp == "肆"))
					dwTmp = 4;
				else if ((strTmp == "五") || (strTmp == "伍"))
					dwTmp = 5;
				else if ((strTmp == "六") || (strTmp == "陆"))
					dwTmp = 6;
				else if ((strTmp == "七") || (strTmp == "柒"))
					dwTmp = 7;
				else if ((strTmp == "八") || (strTmp == "捌"))
					dwTmp = 8;
				else if ((strTmp == "九") || (strTmp == "玖"))
					dwTmp = 9;
				else 
					dwTmp = 0;				
			}
			else if (bIsChineseUnit)
			{
				int dwCurUnit = 0;
				
				// 先把单位算出来
				if ((strTmp == "十") || (strTmp == "拾"))
					dwCurUnit = 10;
				else if ((strTmp == "百") || (strTmp == "佰"))
					dwCurUnit = 100;
				else if ((strTmp == "千") || (strTmp == "仟"))
					dwCurUnit = 1000;
				else if ((strTmp == "万") || (strTmp == "萬"))
					dwCurUnit = 10000;
				else if ((strTmp == "亿") || (strTmp == "億"))
					dwCurUnit = 100000000;

				if (bTenStartUnit && dwCurUnit == 10)
				{
					dwResult = 10;
					dwPreUnit = dwCurUnit;
					dwTmp = -1;

				}
				// 遇到单位则把前面的数字加上去
				else if (-1 != dwTmp)
				{
					dwResult += dwTmp * dwCurUnit;
					
					dwPreUnit = dwCurUnit;

					// 计算过有单位的数字后，个位要清０
					dwTmp = -1;

					continue;
				}
				else if (dwPreUnit > 0)
				{
					dwResult *= dwCurUnit;
					dwPreUnit = dwCurUnit;
				}
			}
		}
		else
		{
			// 把非中文数字之前的数字转为英文数字
			if (-1 != dwTmp)
				dwResult += dwTmp;	
			if (0 != dwResult)
				strRet += Int2Str(dwResult);

			dwTmp = -1;
			dwResult = 0;

			//　ASCII字符
			strRet += strRawData[dwPos++];
		}
	}

	// 处理后面没有单位的个位数字
	if (-1 != dwTmp && bIsHasUnit)
		dwResult += dwTmp;
	if (0 != dwResult)
		strRet += Int2Str(dwResult);

	return strRet;
}

/***************************************************************
 * 功能描述：字符串中的全角数字转为半角数字
 * 输入参数：
 * 输出参数：
 * 返回值  ：
 ***************************************************************/
std::string ToEnglishDigit(std::string& strRawData)
{
	string strTmp = strRawData;
	string strRet;
	string::size_type dwPos = 0;

	while(dwPos < strTmp.size())
	{
		if ((unsigned char)strTmp[dwPos] != (unsigned char)0xA3)
		{
			// 对于不需要转换的其他中文
			if (strTmp[dwPos] < 0)
			{
				strRet += strTmp[dwPos++];
				strRet += strTmp[dwPos++];
			}
			else
			{
				//　ASCII字符
				strRet += strTmp[dwPos++];
			}
		}
		else
		{
			// 
			int dwIndex = (unsigned char)strTmp[dwPos+1] - (unsigned char)0xA1;
            if (dwIndex > 0 && dwIndex < HALF_CHAR_NUMS)
            {
			    strRet += g_strEnglish[dwIndex];
			    dwPos += 2;
            }
            else
            {
                strRet += strTmp[dwPos++];
            }
		}
	}
	return strRet;
}
