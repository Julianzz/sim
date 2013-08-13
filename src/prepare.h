/*
 * prepare.h
 *
 *  Created on: 2012-10-10
 *      Author: stefanyang
 *
 *  poi相似度计算涉及的所有预处理函数
 *
 *  预处理主要包括：
 *  1. poi主要字段的规范化
 *		如：全角转半角，标点符号的删除，简繁体转换等
 *  2. poi进行特征识别，包括区域、行政级别、连锁店等
 *  3. 周边检索结构体的初始化
 *  4. idf词表的准备
 *  5. 无效电话（高频电话认为是无用电话）的识别
 */

#ifndef PREPARE_H_
#define PREPARE_H_

#include <fstream>
#include "typeDefine.h"
#include "data.h"
#include "tools.h"
#include "typeDefine.h"

//=================================================
// 多线程参数结构体
//=================================================
struct PrepareFromCsvParam {
	ifstream *fin; 							// 输入文件迭代器
	PreProcMap *pre_proc_map; 				// 预处理结果保存的map
	const Resource *resource; 				// 预处理中会用的所有词表资源
	const CsvIndexCfg *csv_index_cfg; 		// 预处理中涉及的所有配置
	pthread_mutex_t *prepare_read_mutex; 	// 读文件锁
	pthread_mutex_t *prepare_setmap_mutex;	// 修改结果map锁
	const Tools *tools;						// 分词、简繁转化等工具函数
};

//struct PrepareCsvDirParam {
//	ifstream *fin;						// 输入文件迭代器
//	PreProcMap *pre_proc_map;			// 预处理结果保存map
//	const Resource *resource;			// 预处理中会用的所有词表资源
//	const CsvIndexCfg *csv_index_cfg;	// 预处理中涉及的所有配置
//	const Tools *tools;					// 分词、简繁转化等工具函数
//};

//=================================================
// 判断数据是否有效
//=================================================
// 判断预处理结果是否是可用数据
bool isBadPreProcData(PreProcData *pre_proc_data);
// 判断配置参数结构体是否是有效数据
bool isBadCsvIndexCfg(const CsvIndexCfg *csv_index_cfg);

//=================================================
// 预处理poi数据
//
// ===== poi数据的预处理包括 =====
// 区域（东区、南区等）识别
// 门识别（东门，南门等）
// 连锁店识别
// 号码识别并规范
// 行政级别的识别
// 简称、合并名识别
// 同义词替换
// 子点特征识别
// 后缀类型识别
// 分店识别
// 等等
//
// ===== 预处理数据主要有几方面的考虑 =====
// 1. 将所有数据先预处理然后进行相似度计算，有效减少了重复处理，对速度有大幅提升
// 2. 动态生成词表的idf，这样可以根据待处理的数据得到针对性的特征词计量，提高聚合精度
// 3. 对全量数据的电话预先处理，有效减少不同poi点公用一个电话带来的聚合错误
//=================================================
// 生成record结构体
void getRecFromCsv(const vector<string> &vec_data, Record &record,
		const CsvIndexCfg *csv_index_cfg);
void getRecFromCsv(const string &str_data, Record &record,
		const CsvIndexCfg *csv_index_cfg);

// 生成预处理结果
void makePreProcFromRec(const Record *record, PreProcData *pre_proc_data,
		const Resource *resource, const Tools *tools);
void makePreProcFromCsv(const string &str_data, PreProcData *pre_proc_data,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg);
void makePreProcFromCsv(const string &str_data, PreProcData *pre_proc_data,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg, Record *record);

// 从文件生成预处理结果
void _initMapFromCsvFileSgl(ifstream &fin, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg);
void _initMapFromCsvFileMulti(ifstream &fin, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg, pthread_mutex_t &prepare_read_mutex,
		pthread_mutex_t &prepare_setmap_mutex);
void initMapFromCsvFile(const char *path, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg);
//void initMapFromCsvFileDir(const char *data_dir, PreProcMap *pre_proc_map,
//		const Resource *resource, const Tools *tools,
//		const CsvIndexCfg *csv_index_cfg);

// 从文件生成预处理结果并更新数据库
void _updateDBFromCsvFileSgl(ifstream &fin, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg);
void _updateDBFromCsvFileMulti(ifstream &fin, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg, pthread_mutex_t &prepare_read_mutex,
		pthread_mutex_t &prepare_setmap_mutex);
void updateDBFromCsvFile(const string &path, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg);

// 从数据库生成预处理结果
//void initMapFromDB();

//=================================================
// 统计函数
//=================================================
// 统计名称中词的idf值
void statIdf(const PreProcData &pre_proc_data,
		map<string, double> &stat_idf_map);
// 统计电话的出现次数
void statTel(const PreProcData &pre_proc_data, map<string, int> &stat_tel_map);

// 计算idf
void makeIdf(unsigned long number, map<string, double> &stat_idf_map
		, double &max_idf, double &min_idf);
// 获取无效电话列表
void makeUselessTel(map<string, int> &stat_tel_map, int threshold);

//=================================================
// 周边搜索相关函数
//
// 周边搜索的主要方法说明：
// 		将中国的地图划分为大小相等的很多个格子，并将所有poi点按照名称分词和坐标，
// 映射到不同的格子中。当需要做周边搜索时，先按照关键词获取该点所在的格子以及周边
// 的8个格子的所有数据，然后根据坐标距离，做过滤，最后得到需要的数据集合
//
// 名称切分方法说明：
// 		早期名称切分使用bigram的方式，这种方式召回率有保证，但是召回的无用数据很多
// 且对于很多同音异形的别字短词很难召回。优化后，现在使用的分词方式为，短词名称使用bigram切分
// 和拼音同时建索引，长词则采用中文分词+拼音的方式
//=================================================
int getLatKey(double lat);
int getLngKey(double lng);

void setGridMap(const GridKey &key, const unsigned long long value,
		GridMap &grid_map);
void setGridMap(const PreProcData *pre_proc_data, const Resource *resource,
		GridMap *grid_map);

void initGridMap(const PreProcMap *pre_proc_map, const Resource *resource,
		GridMap *grid_map);

void getAroundGridMap(const PreProcData &pre_proc_data, const GridKey &key, const GridMap &grid_map,
		set<unsigned long long> &result_set, int level);
void getAroundGridMap(const PreProcData &pre_proc_data, const GridMap &grid_map,
		set<unsigned long long> &result_set, int level);

#endif /* PREPARE_H_ */
