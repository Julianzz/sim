/*
 * rules.h
 *
 *  Created on: 2011-8-23
 *      Author: stefanyang
 *
 *	常用的一些基础函数和规则函数
 *	从原有老版本中继承过来的代码，部分函数已经弃用
 */

#ifndef RULES_H_
#define RULES_H_

#include <string>
#include "common.h"
#include "data.h"
#include "typeDefine.h"

// 打印一些简单的stl结构
void out_vector(const std::vector<std::string> &v);
void out_set(const _set &v);
void out_set(const set<int> &v);
void out_set(const set<unsigned long long> &v);
void out_hashset(const _hashset &v);
void out_map(const map<string, bool> &data);
void out_map(const map<string, string> &data);
void out_map(const map<string, int> &data);
void out_map(const map<int, string> &data);

// if two string is equal or include other return true
bool wordIsSame(const std::string &a, const std::string &b);
bool wordIsSame(const std::string &a, const std::string &b, int length);

// if the two set has one 'strIsSame' element return true
// example: ['111', '222'] ['1111', '333'] return true because of '1111' strIsSame '111'
bool listIsSame(_vector &a, _vector &b);

// find element both in a and b, and erase from b, add them to result
// example: ['111', '222'] ['11', '444'] 
// a ['111', '222']   b ['444']  result ['11']
// because '111' in a incluse '11' in b 
int findSameInList(_vector &a, _vector &b, _vector &result);

// merge two vector to the resutl vector
// exmaple: src:[111, 222], result:[t, p], the process result is: result=[t111,t222,p111, p222]
int mergeList(_vector &src, _vector &result);

// change chinese string to pinyin vector
// example: 银行 return ['#yin#xing#', '#yin#hang#']
int strToPY(const std::string &src, _vector &result, const _mapVec &pinyin);

// change single word list to pinyin vector
// example: ["银", "行"] return  ['#yin#xing#', '#yin#hang#']
int vecToPY(_vector &src, _vector &result, const _mapVec &pinyin);

// if two string's pinyin is same or include other return true
// example: 名茶行 茗茶 return true
bool pyIsSame(const std::string &a, const std::string &b, const _mapVec &pinyin,
		long int diffLen = 1);

// check two chinese words vector
// delete the pinyin same word in both a and b
// add same in same vector
//int pyVecIsSame(_vector &a, _vector &b, _vector &same, _mapVec &pinyin);
//int pyVecIsSame(_vector &a, _vector &b, CacheResult &ar, CacheResult &br,  _mapVec &pinyin);
int pyVecIsSame(_vector &a, _vector &b, _vector &ar, _vector &br,
		_mapVec &pinyin, _hashset &dpmark, long int diffLen = 1);
int pyVecIsSame(_vector &a, _vector &b, _vector &ar, _vector &br,
		_mapVec &pinyin, _hashset &dpmark, std::string &aObjName,
		std::string &bObjName, long int diffLen = 1);
int pyVecIsSame(_vector &a, _vector &b, _vector &ar, _vector &br,
		const _mapVec &pinyin, long int diffLen = 0);

// SBC to DBC
// return result in original vec
// dict: table that used for mapping, key is SBC, value is DBC
// values: ['测', '，', '试', '1', 'a']
int S2D(_vector& values, _map &dict);

// remove bracket from string
// return result in original vec
// if only_bracket is true, only remove bracket else remove content in bracket
//int removeBracket(_vector& values, bool only_bracket = false);
int removeBracket(_vector &values, CacheResult &result);
int removeBracket(_vector &values, OriCacheResult &result);

// remove punc from string
int removePunc(_vector &values, bool needBra = false);

// format english word size
// return result in original vec
// default to lower size
int formatSize(_vector &values, bool to_upper = false);

// single word vector to string
// example: ['a', '2', 'b'] to "a2b"
//int vec2str(_vector &values, std::string &result);
int vec2str(_vector &values, std::string &result, bool del = true);

// filte special word from src vector
//int filte(_vector &src, CacheResult &result, _hashset &filteTable);
//int filte(_vector &src, _vector &result, _hashset &filteTable);
int filte(_vector &src, _vector &result, const _set &filteTable);

int filteIfNotNull(_vector &src, _vector &result, const _set &filteTable);

// filte word the include in other poi name's brackets
//int filte(_vector &aDiff, CacheResult &a, CacheResult &b);
int filte(_vector &aDiff, _vector &a, _vector &bInBracket);

// use pinyin to filte word that inclue in other poi name's brackets content
//int filte(_vector &aDiff, CacheResult &a, CacheResult &b, Resource &resource);
//int filte(_vector &aDiff, _vector &a, _vector &bInBracket, _mapVec &pinyin);
int filte(_vector &aDiff, _vector &a, const _vector &bInBracket,const  _mapVec &pinyin);
int filteIfNotNull(_vector &aDiff, _vector &a, const _vector &bInBracket,const  _mapVec &pinyin);

// filte word that end with some special single word
int filteEndWith(_vector &src, _vector &result, _hashset &endList,
		_hashset &undel);

// use delim to divid src into two part, and use wordsplit for each part
int lcsSplit(std::string &src, std::string &delim, _vector &result,
		WordSplit &ws);
//int lcsSplit(CacheResult &src, std::string &delim, Resource &resource);

// merge continuous single word to one word
// example: ['a', 'b', 'abc', 'q', 'c', 'w']
// ---> ['ab', 'abc', 'qcw']
int mergeWord(_vector &src, _vector &result);
int mergeWord(_vector &src, _vector &result, const Resource &resource);
//int mergeNumAlp(const std::string &srcStr, _vector &src, _vector &tempVec, _set &result, _hashset &measure, bool isInBacket);
int mergeNumAlp(const std::string &srcStr, _vector &src, _vector &tempVec,
		_vector &tempResultVec, _set &result, const _hashset &measure,
		bool isInBracket);

// trans some special word to format content
int transWord(_vector &src, _vector &result, _map &transMap);
int transWord(_vector &src, const _map &transMap);

// get trans name and put it into result
int getTransName(_set &src, _set &result, _map &transMap);

// get trans type
int getTransType(_vector &src, _set &result, _map &transMap);

// when find chain name, get the right left word list
int getLeftList(_vector &src, _set &result, _map &transMap);

// find import obj name from src, that in objSet
int findObjName(_vector &src, std::string &objName, double &objMaxDistance,
		_mapStrDbl &objSet);
int findObjName(CacheResult &src, Resource &resource);
int findObjName(OriCacheResult &src, Resource &resource);

// if find in filter, erase from src 
int filte(_vector &src, const _hashset &filter);

int filte(_vector &src, _vector &filter);
void filte(_vector &src, string &filter);
int filte(_vector &src, _vector &filter, Resource &resource, int &count);
void filte(_vector &src, string &filter, const _mapSet &typeword, int &count);

// 过滤完结果放到result中，被过滤的放到filted中
int filte(_vector &src, _vector &result, _set &filted, const _hashset &resource);

int filteDistrict(_vector &src, std::string &addr, const _hashset &xzqh_set);
int filteDistrict(_vector &src, std::string &addr, _hashset &xzqh_set,
		Resource &resource, int &count);
void filteDistrict(_vector &src, std::string &addr, const _hashset &xzqh_set,
		const _mapSet &typeword, int &count);

// find word in markSet, if find result = 1
int findMark(_vector &src, int &result, _hashset &markSet);

// find word in markSet, if find input in result set
int findMark(_vector &src, _set &result, _hashset &markSet);

// find mark word in both two vec, and find thier relation, return in result
// oneNullDiff: 只有一条数据的括号内容中包含特征词时，是否认为不同，true表示认为不同
// oneDiff: 指当两个数据都包含括号，且括号中的只有一边有特征词，是否认为相同，true表示认为不同
int findMark(_vector &a, _vector &b, SimScore &score, int &result,
		_hashset &markSet, bool oneNullDiff, bool oneDiff);

// filte if count et Count
int filteCount(_vector &src, _vector &result, _hashset &filter, size_t Count);

// filte if word end with special word
int filteMarkDistrict(_vector &src, _vector &result, _hashset &filter);

// check string only include alpha, number and measure word
bool isNumMeasure(std::string &src, _hashset &filter);

// count single zh word
int singleZhCount(_vector &src, size_t &count);

// check the last tailLen word in src, if find type word return type
int getTailType(_vector &src, string &type, size_t tailLen, _map &transMap);

// check whether the two word are same if remove single no mean end word
bool wordInclude(std::string &a, std::string &b, const _hashset &endList);

// find wordInclude word in vec, and put it to same vec
int filteInclude(_vector &a, _vector &b, _vector &ar, _vector &br,
		const _hashset &endList);

int transNum(_vector &src);
int transZhNum(_vector &src, _hashset &number, _hashset &number2,
		int length = 1);
int transZhNum(std::string &src, std::string &result, _hashset &number,
		_hashset &number2);
int transZhNum(std::string &src, std::string &result, _hashset &number,
		_hashset &number2, int length);
int transZhNum(_vector &src, _vector &result, _hashset &number,
		_hashset &number2);
int transZhNum(_set &src, _hashset &number, _hashset &number2, int length);

int delfix(std::string &src, std::string &result, std::string &fix,
		const std::string &type);

// weather the tail of two string is same, if same, return true
// length: tail length
bool tailIsSame(const std::string &str1, const std::string &str2, int length);

int splitType(const std::string &src, _vector &result);
int splitType2(const std::string &src, _vector &result);

// find tail name
int findTailType(const std::string &srcStr, SoSoAttr &result, const _map &sosoTypeMap);
int findTailType(const std::string &srcStr, std::string &soso_type,
		const _map &sosoTypeMap);

int findSoSoType(CacheResult &src, _map &sosoTypeMap);
int findSoSoType(OriCacheResult &src, _map &sosoTypeMap);

int findAreaTail(CacheResult &result, _hashset &areaTailSet);
int findAreaTail(OriCacheResult &result, _hashset &areaTailSet);

// filte long english or pinyin word such as: thethirdaffiliatedhospitalsunyatsenuniversity
int filteLongAlp(_vector &src);

int filteDoorInBrac(_vector &src, _set &doorSet, const Resource &resource);
int filteDoor(_vector &src, _set &doorSet, const Resource &resource);
int filteDoor2(_vector &src, _set &doorSet, const Resource &resource);
int filteDoor(_set &src, _set &doorSet);

bool checkSetInclude(const _set &a_set, const _set &b_set);

int countNotType(_vector &src, int &count, Resource &resource);
int countNotType(_vector &src, int &count, const _mapSet &typeword);

int replaceSpecWord(std::string &src);

int set_inter(_set &aSet, _set &bSet, _vector &resultVec);
int set_diff(_set &aSet, _set &bSet, _vector &resultVec);

int addrIsSame(std::string &str_a, std::string &str_b);

int transWord(set<string> &src_set, set<string> &result_set, _map &transMap);
int transWord(_mapStrDbl &objMap, _mapStrDbl &resultMap, _map &transMap);
//int transWord(_hashset &src_set, _hashset &result_set, _map &transMap);
int transWord(_map &objMap, _map &resultMap, _map &transMap);
int transWord(_mapSet &oldMap, _mapSet &newMap, _map &transMap);

int findMaxMin(_mapStrDbl &src_map, double &max, double &min);

int addPrefix(_vector &src_vec, const _hashset &word_set);

// 规范行政机构的地理名称，将安定镇规范为安定
int formatGov(_vector &src, Resource &resource);
int formatGov(_vector &src, OriCacheResult &result, Resource &resource);
int formatGov(std::string &src, Resource &resource);
int formatGov(std::string &src, OriCacheResult &result, Resource &resource);

bool isSpecType(CacheResult &a, CacheResult &b, std::string str_type);

// 合并末尾为办，委等单字
int mergeTailSglWord(OriCacheResult &result);

bool isType(std::string str_type);

#endif
