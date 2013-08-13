/*
 * tools.h
 *
 *  Created on: 2012-10-23
 *      Author: stefanyang
 *
 *  一些公用的基础工具函数
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <cstdio>
#include <cstring>
#include <errno.h>
#include <string>
#include <iostream>
#include "utils/contrib.h"
#include "data.h"
#include "common.h"
#include "typeDefine.h"
#include "utils/wordsplit/wordsplit.h"
using namespace std;

//========================================
// 繁体转简体
//========================================
class Trad2Simp {
private:
	unsigned short** m_trad2simp_tbl;
	size_t m_map_arr_size;

public:
	bool init();
	bool destroy();
	bool func(char* pcDest, const char* pcSrc, const unsigned int dwSrcLen);
	string func(const string &str_src);

	Trad2Simp() {
		if (!init()) {
			cout << "trad2simp init err" << endl;
		}
	}

	~Trad2Simp() {
		destroy();
	}
};

//========================================
// tools 结构体
//========================================
class Tools {
public:
	Trad2Simp *m_trad2simp;
	WordSplit *m_wordsplit;

public:
	Tools() {
		m_trad2simp = new Trad2Simp();
		m_wordsplit = new WordSplit();
	}

	~Tools() {
		delete m_trad2simp;
		delete m_wordsplit;
	}
};

//========================================
// 函数形态的tools
//========================================
// 打印预处理结果
void print(PreProcData *pre_proc_data);
void print(const SimTempCache *sim_temp_cache);

// 全角转半角
string sbc2dbc(const string &str);

// 大写转小写
void lower(string &str);

// 去掉标点符号
void removePunc(string &values, bool needBra);

// 中文数字转阿拉伯数字
void transZhNum(string &str, const _hashset &number, const _hashset &number2,
		int length);
void transZhNum(_set &src, const _hashset &number, const _hashset &number2,
		int length);

// 去掉括号内的内容
void removeBracket(string &str, vector<string> &result);

// 去掉名称后面的**店后缀
//int findAreaTail(string &formatName, vector<string> &bracket_vec,
//		const _hashset &areaTailSet);
int findAreaTail(PreProcData *pre_proc_data, const Resource *resource);

// 获取地名中的行政后缀
void getGovTail(std::string &src, string &gov_tail, const _mapSet &typeword);

// 获取连锁店名
void findObjName(PreProcData *pre_proc_data, const Resource *resource);

// 名称只为类型词时，将括号内的内容添加到规范名称中
void addContentForTWName(PreProcData *pre_proc_data, const _mapSet &typeword);

// 合并数字,字母内容
void mergeNumAlp(PreProcData *pre_proc_data, const Resource *resource);

// 合并结尾特别的单字
void mergeTailSglWord(PreProcData *pre_proc_data);

// 去掉行政区划词
void filteDistrict(PreProcData *pre_proc_data, const Resource *resource,
		int &count);

// 规范行政区划后缀
void formatGov(string &name, string &gov_tail, const _mapSet &typeword);
void formatGov(_vector &src, string &gov_tail, const _mapSet &typeword);

// 查找相同的类型码或映射类型
bool findType(const string &base_main_type, const string &base_sub_type,
		const string &base_type, const string &inc_main_type,
		const string &inc_sub_type, const string &inc_type,
		const _mapSet &type_map, SimTempCache *sim_temp_cache);
bool findType(const PreProcData *base_data, const PreProcData *inc_data,
		const _mapSet &type_map, SimTempCache *sim_temp_cache);
bool isSameType(const string &base_type, const string &inc_type,
		const _mapSet &type_map);

// 一个词是否包含于一个vector中
bool isInclude(const string &str, const vector<string> &str_vec);

// 不可缺失词的识别
void findUnlackWord(PreProcData *pre_proc_data, const _mapSet &unlack_map);

// 从不同词部分查找是否有包含精确类关键词的词
//void findDiffAccurateKeyword(const vector<string> &data_vec,
//		const string &base_str, const string &inc_str, const Resource *resource,
//		vector<string> &result_vec);
void findDiffAccurateKeyword(const vector<string> &data_vec,
		const set<string> &word_set, const Resource *resource,
		vector<string> &result_vec);

// 过滤00词表中出现的词
void filte(vector<string> &data_vec, const Resource *resource);

// 将名称分词后，带店结尾的词添加到grid表中
void setGridKeyWord(const vector<string> &name_vec, set<string> &grid_set);

// 规范门类信息
void formatDoorSet(set<string> &door_set);

// 检查不同部分是否出现有非频繁非01词表词
bool hasNotFreqTypeWord(vector<string> &data_vec, const Resource *resource,
		double theshold, int &count, bool checkGovTail);

// 获取某路径下的所有文件名，并返回增加目录的路径表
int getPaths(const std::string &path, vector<string> &path_vec);

// 以块方式读取文件
bool getFileLine(ifstream &fin, string& strLine);

// 获取名称中不可缺失词
void findUnlackWord(PreProcData *pre_proc_data, const Resource *resource);

// 市旅游局 扩展为 北京市旅游局
void extendGov(PreProcData *pre_proc_data);

// 上海站 扩展为 上海火车站
void extendTrain(PreProcData *pre_proc_data);

// 获取数据的行政级别
void findGovLevel(PreProcData *pre_proc_data, const Resource *resource);

// 获取地支的数据
void findDizhi(PreProcData *pre_proc_data, const Resource *resource);

// 获取门类信息
void findDoor(PreProcData *pre_proc_data, const Resource *resource);

// 获取括号中的以店结尾的可能的分店信息
void findFendian(PreProcData *pre_proc_data, const Resource *resource);

// 获取后缀是否在词表中出现
void findTail(string &str, string &result, const set<string> &tailSet);

// 识别 2院 这种分词已经在一起的数量词
bool isNumWord(string &str, const set<string> &measure);
void findNumWord(PreProcData *pre_proc_data, const Resource *resource);

// 识别 雅宝大厦停车场 中的雅宝
void findParkPrefix(PreProcData *pre_proc_data, const Resource *resource);

// 计算idf的值之和
double sumIDF(const vector<string> &data_vec, const Resource *resource);

// 计算两个set是否相似
void getSetSim(const set<string> &base_set, const set<string> &inc_set,
		double &score, vector<string> &same_vec, vector<string> &diff_base_vec,
		vector<string> &diff_inc_vec, const Resource *resource);
void getSetSim(set<string> &base_set, set<string> &inc_set, double &score,
		const Resource *resource);

// 判断门类信息中是否包含方向信息
bool isSpecDoor(const set<string> &door_set, const Resource *resource);

// 将包含于filter_set中的词从data_vec中过滤掉
void filterSet(vector<string> &data_vec, set<string> &filter_set);

// 将包含于str中的词从data_vec中过滤掉
void filter(const string &str, vector<string> &data_vec,
		vector<string> &result_vec);

// 将 **中等放到数字容器中的词复制到name中
void getNumWord(set<string> &num_set, vector<string> &name_vec);

// 针对订餐小秘书的数据特征，做的特别处理
// 名称中包含空格，空格后面的内容当成括号内的内容
void formatName(string &name);

// 增加量词
// 处理空港科技大厦a 中的a，扩展为a座
void addSuffix(PreProcData *pre_proc_data);

// 检查结合是否有包含
bool checkInclude(const set<string> &base_unlack_set,
		const set<string> &inc_unlack_set, const set<string> &inc_name_set);
bool checkInclude(const set<string> &base_door_set,
		const set<string> &inc_door_set, const string &base_name);

// 对特殊的容易引起同义词歧义的词语进行替换
void transSpecWord(PreProcData *pre_proc_data);

// 将类型名称规范化
void formatTypeName(PreProcData *pre_proc_data, const Resource *resource);

bool isTypeConflict(const string &base_type, const string &inc_type,
		const Resource *resource);
bool isConflict(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource);

void formatNumName(string &name);
#endif /* TOOLS_H_ */
