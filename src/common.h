/*
 * common.h
 *
 *  Created on: 2011-9-24
 *      Author: stefanyang
 *
 *  ��������������ܵĳ��ú���
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <vector>
#include <set>
using namespace std;

//=========================================================================
// �ַ��жϼ���
//=========================================================================
#define ISALPHA(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define ISDIGIT(c) (c >= '0' && c <= '9')
#define ISSPACE(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')

//=========================================================================
// ���������ַ������ַ����ָ������
//=========================================================================
// ����˵����
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
// src: ����2008����˻�english��
// result: ['��', '��', '2008', '��', '��', '��', '��', 'english', '��']
int split2word(const std::string &src, std::vector<std::string> &result);

// split a string that include chinese word to a word vector
// example: 
// src: ����2008����˻�english��
// result: ['��', '��', '2', '0', '0', '8', '��', '��', '��', '��', 'e', 'n', 'g', 'l', 'i', 's', 'h', '��']
int split2word2(const std::string &src, std::vector<std::string> &result);

// bigram split
void spliteBigram(const std::string &src, std::vector<std::string> &vec_words);

//=========================================================================
// �ַ����滻��������
//=========================================================================
int replace(std::string &str, const std::string &oldValue,
		const std::string &newValue, std::string &newStr);
string& replaceStr(string& str, const string& old_value,
		const string& new_value);

//=========================================================================
// ���ͬ�ַ���ʶ��
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
// ���src�Ƿ���endStr��β
//=========================================================================
bool endswith(const std::string &src, const std::string &endStr);

//=========================================================================
// ���src�Ƿ���endStr��ʼ
//=========================================================================
bool startswith(const std::string &src, const std::string &startStr);

//=========================================================================
// ���src�Ƿ���dataSet�е�ĳһ����ʼ
//=========================================================================
bool startsWithSet(const std::string &src, const set<string> &dataSet);

//=========================================================================
// ���src�Ƿ���dataSet�е�ĳһ����β
//=========================================================================
bool endsWithSet(const std::string &src, const set<string> &dataSet);

//=========================================================================
// �ַ����Ƿ������֣��Ƿ���true
//=========================================================================
bool isDigit(std::string &src);

//=========================================================================
// �ַ����Ƿ�����ĸ���Ƿ���true
//=========================================================================
bool isAlpha(std::string &src);

//=========================================================================
// �ַ����Ƿ������ֻ���ĸ���Ƿ���true
//=========================================================================
bool isAlpDig(std::string &src);

//=========================================================================
// �༭������㺯��
//=========================================================================
size_t levDis(const std::string &s1, const std::string &s2);

#endif
