/*
 * common.h
 *
 *  Created on: 2011-9-24
 *      Author: stefanyang
 *
 *  定义包含基础功能的常用函数
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <vector>
#include <set>
using namespace std;

//=========================================================================
// 字符判断集合
//=========================================================================
#define ISALPHA(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define ISDIGIT(c) (c >= '0' && c <= '9')
#define ISSPACE(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')

//=========================================================================
// 根据特征字符串做字符串分割函数集合
//=========================================================================
// 参数说明：
// src: string need to split
// delimit: delimiter that can be any string
// v: result vector
// null_subst: if element is null, return null_subst
int split(const std::string &src, const std::string &delimit,
		std::vector<std::string> &v, std::string null_subst = "");

int split(const std::string &src, const std::string &delimit,
		std::set<std::string> &v, std::string null_subst = "");

// src: string need to split
// delimit: delimiter that can be only a char
// v: result vector
// trim: if null element is not need return, set trim true
// null_subst: if element is null, return null_subst
int split2(const std::string &src, const std::string &tok,
		std::vector<std::string> &v, bool trim = false, std::string null_subst =
				"");

// split a string that include chinese word to a word vector
// example: 
// src: 北京2008年奥运会english馆
// result: ['北', '京', '2008', '年', '奥', '运', '会', 'english', '馆']
int split2word(const std::string &src, std::vector<std::string> &result);

// split a string that include chinese word to a word vector
// example: 
// src: 北京2008年奥运会english馆
// result: ['北', '京', '2', '0', '0', '8', '年', '奥', '运', '会', 'e', 'n', 'g', 'l', 'i', 's', 'h', '馆']
int split2word2(const std::string &src, std::vector<std::string> &result);

// bigram split
void spliteBigram(const std::string &src, std::vector<std::string> &vec_words);

//=========================================================================
// 字符串替换函数集合
//=========================================================================
int replace(std::string &str, const std::string &oldValue,
		const std::string &newValue, std::string &newStr);
string& replaceStr(string& str, const string& old_value,
		const string& new_value);

//=========================================================================
// 最长相同字符串识别
//=========================================================================
int LCS(std::vector<std::string> &s1, std::vector<std::string> &s2,
		std::vector<std::string> &result);

//=========================================================================
// get string len, also include gb chinese word
//=========================================================================
size_t len(const std::string &a);

//=========================================================================
// convert string to double
//=========================================================================
double Str2Double(const std::string& str);

//=========================================================================
// 检查src是否以endStr结尾
//=========================================================================
bool endswith(const std::string &src, const std::string &endStr);

//=========================================================================
// 检查src是否以endStr开始
//=========================================================================
bool startswith(const std::string &src, const std::string &startStr);

//=========================================================================
// 检查src是否以dataSet中的某一个开始
//=========================================================================
bool startsWithSet(const std::string &src, const set<string> &dataSet);

//=========================================================================
// 检查src是否以dataSet中的某一个结尾
//=========================================================================
bool endsWithSet(const std::string &src, const set<string> &dataSet);

//=========================================================================
// 字符串是否是数字，是返回true
//=========================================================================
bool isDigit(std::string &src);

//=========================================================================
// 字符串是否是字母，是返回true
//=========================================================================
bool isAlpha(std::string &src);

//=========================================================================
// 字符串是否是数字或字母，是返回true
//=========================================================================
bool isAlpDig(std::string &src);

//=========================================================================
// 编辑距离计算函数
//=========================================================================
size_t levDis(const std::string &s1, const std::string &s2);

#endif
