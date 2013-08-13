#include "contrib.h"
#include <cstdio>
#include <string>

#define HALF_CHAR_NUMS 93

string g_strEnglish[HALF_CHAR_NUMS] = {
	 "!", "\"", "#", "��", "%", "&","\'", "(", ")", "*", "+", ",", "-", ".", "/",  
"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?",
"@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
"P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_",
"'", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
"p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}"};

// ��������
string strChineseDigit[] =  {"һ", "��", "��", "��", "��", "��", "��", "��", "��", "��",
	"��", "Ҽ", "��", "��", "��", "��", "½", "��", "��", "��"};

// �������ֵ�λ
string strChineseUnit[] =  {"ʮ","��", "ǧ", "��", "��", "ʰ", "��", "Ǫ", "�f", "�|"};




string Int2Str(int dwValue)
{
    char szBuf[16];
    sprintf(szBuf,"%d",dwValue);
    return string(szBuf);
}

/***************************************************************
 * �����������ж�һ���ַ����Ƿ������ĵ�����
 * ���������
 * ���������
 * ����ֵ  ��
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
 * �����������ж�һ���ַ����Ƿ������ĵ�������λ
 * ���������
 * ���������
 * ����ֵ  ��
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
 * �����������ַ����еĴ�д����תΪСд����, ֻ�ܴ����д������
 * ���������
 * ���������
 * ����ֵ  ��
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
		// ���ڲ���Ҫת������������
		if (strRawData[dwPos] < 0)
		{
			strTmp =  strRawData[dwPos++];
			strTmp += strRawData[dwPos++];

			bIsChineseDigit = IsChineseDigit(strTmp);			
			if (!bIsChineseDigit)
			{
				// �ѷ���������֮ǰ������תΪӢ������
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
				if ((strTmp == "һ") || (strTmp == "Ҽ"))
					dwTmp = 1;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 2;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 3;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 4;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 5;
				else if ((strTmp == "��") || (strTmp == "½"))
					dwTmp = 6;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 7;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 8;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 9;
				else 
					dwTmp = 0;
				dwResult*=10;
				dwResult += dwTmp;
			}			
		}
		else
		{
			// �ѷ���������֮ǰ������תΪӢ������
			if (-1 != dwTmp)
				dwResult += dwTmp;	
			if (0 != dwResult)
				strRet += Int2Str(dwResult);

			dwTmp = -1;
			dwResult = 0;

			//��ASCII�ַ�
			strRet += strRawData[dwPos++];
		}
	}
	dwPos = 0;

	bool bTenStartUnit = true;
	bool bIsStartDigit = false;
	while(bIsHasUnit && dwPos < strRawData.size())
	{
		// ���ڲ���Ҫת������������
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
					if (strTmp != "ʮ" && strTmp != "ʰ")
					{
						bTenStartUnit = false;
					}
				}
				bIsStartDigit = true;
			}

			if (!bIsChineseDigit && !bIsChineseUnit)
			{
				// �ѷ���������֮ǰ������תΪӢ������
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
				if ((strTmp == "һ") || (strTmp == "Ҽ"))
					dwTmp = 1;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 2;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 3;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 4;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 5;
				else if ((strTmp == "��") || (strTmp == "½"))
					dwTmp = 6;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 7;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 8;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwTmp = 9;
				else 
					dwTmp = 0;				
			}
			else if (bIsChineseUnit)
			{
				int dwCurUnit = 0;
				
				// �Ȱѵ�λ�����
				if ((strTmp == "ʮ") || (strTmp == "ʰ"))
					dwCurUnit = 10;
				else if ((strTmp == "��") || (strTmp == "��"))
					dwCurUnit = 100;
				else if ((strTmp == "ǧ") || (strTmp == "Ǫ"))
					dwCurUnit = 1000;
				else if ((strTmp == "��") || (strTmp == "�f"))
					dwCurUnit = 10000;
				else if ((strTmp == "��") || (strTmp == "�|"))
					dwCurUnit = 100000000;

				if (bTenStartUnit && dwCurUnit == 10)
				{
					dwResult = 10;
					dwPreUnit = dwCurUnit;
					dwTmp = -1;

				}
				// ������λ���ǰ������ּ���ȥ
				else if (-1 != dwTmp)
				{
					dwResult += dwTmp * dwCurUnit;
					
					dwPreUnit = dwCurUnit;

					// ������е�λ�����ֺ󣬸�λҪ�声
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
			// �ѷ���������֮ǰ������תΪӢ������
			if (-1 != dwTmp)
				dwResult += dwTmp;	
			if (0 != dwResult)
				strRet += Int2Str(dwResult);

			dwTmp = -1;
			dwResult = 0;

			//��ASCII�ַ�
			strRet += strRawData[dwPos++];
		}
	}

	// �������û�е�λ�ĸ�λ����
	if (-1 != dwTmp && bIsHasUnit)
		dwResult += dwTmp;
	if (0 != dwResult)
		strRet += Int2Str(dwResult);

	return strRet;
}

/***************************************************************
 * �����������ַ����е�ȫ������תΪ�������
 * ���������
 * ���������
 * ����ֵ  ��
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
			// ���ڲ���Ҫת������������
			if (strTmp[dwPos] < 0)
			{
				strRet += strTmp[dwPos++];
				strRet += strTmp[dwPos++];
			}
			else
			{
				//��ASCII�ַ�
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
