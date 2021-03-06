#ifndef CONTRIB_H_
#define CONTRIB_H_

#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

/***************************************************************
 * 功能描述：判断一个字符串是否是中文的数量
 * 输入参数：
 * 输出参数：
 * 返回值  ：
 ***************************************************************/
bool IsChineseDigit(std::string& strData);

/***************************************************************
 * 功能描述：判断一个字符串是否是中文的数量单位
 * 输入参数：
 * 输出参数：
 * 返回值  ：
 ***************************************************************/
bool IsChineseUnit(std::string& strData);

/***************************************************************
 * 功能描述：字符串中的大写数字转为小写数字, 只能处理大写的整数
 * 输入参数：
 * 输出参数：
 * 返回值  ：
 ***************************************************************/
std::string Decapitalized(std::string& strRawData);

/***************************************************************
 * 功能描述：字符串中的全角数字转为半角数字
 * 输入参数：
 * 输出参数：
 * 返回值  ：
 ***************************************************************/
std::string ToEnglishDigit(std::string& strRawData);

#endif
