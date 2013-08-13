/*
 * data.h
 *
 *  Created on: 2012-9-24
 *      Author: stefanyang
 *
 *  定义poi聚合中需要用的基础结构（类）
 */

#ifndef DATA_H_
#define DATA_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdio.h>
#include <string.h>
#include "utils/computeAddressSimilarity.h"
using namespace std;

//===============================================
// 聚合需要作为输入的基本字段定义
//===============================================
class Record {
public:
	bool m_is_null;				// 标记该记录是否为空, true表示为空，false表示不空
	int m_status;				// 该记录在数据库中的状态，0: 可用，1：不可用

	int m_addr_level;			// 地址级别
	unsigned long long m_id;	// 记录id
	double m_lat;				// 纬度
	double m_lng;				// 经度
	string m_name;				// 名称
	string m_addr;				// 地址
	string m_split_addr;		// 分割后的地址
	string m_type;				// 类型代码，如：241000
	string m_type_name;			// 类型名称
	string m_source;			// 来源
	string m_province;			// 省
	string m_city;				// 市
	string m_district;			// 区
	string m_tel;				// 电话
	string m_accurate_type;		// 子点关键词识别模块返回的值

public:
	void clear() {
		m_is_null = true;
		m_status = 0;

		m_id = 0;
		m_lat = 0.0;
		m_lng = 0.0;
		m_name = "";
		m_addr = "";
		m_split_addr = "";
		m_addr_level = 100;
		m_type = "";
		m_type_name = "";
		m_source = "";
		m_province = "";
		m_city = "";
		m_district = "";
		m_tel = "";
		m_accurate_type = "";
	}

	Record() {
		clear();
	}
};

//===============================================
// 每条poi数据预处理后产生的结果集
//===============================================
class PreProcData {
public:
	bool m_is_null;					// 标记该记录是否为空, true表示为空，false表示不空
	int m_status; 					// 该记录在数据库中的状态，0: 可用，1：不可用

	// raw
	unsigned long long m_id;		// 记录id

	double m_lat;					// 纬度
	double m_lng;					// 经度

	string m_name;					// 名称
	string m_addr;					// 地址
	string m_split_addr;			// 分割后的地址
	int m_addr_level;				// 地址级别
	string m_type;					// 类型编码
	string m_source;				// 类型名称
	string m_province;				// 省
	string m_city;					// 市
	string m_district;				// 区
	set<string> m_tel_set;			// 电话

	// preproc
	string m_accurate_type;			// 子点识别的类型
	string m_accurate_keyword;		// 子点识别的关键词
	vector<string> m_accurate_kw_vec;
	set<string> m_accurate_kw_set;
	string m_format_name; 			// 规范化后的名称
	string m_del_punc_name; 		// 规范化后包含括号的名称
	unsigned int m_del_punc_name_len;	// m_del_punc_name的字数，中文算一个字
	string m_bigram_name;			// 用于bigram切分的名称
	vector<string> m_raw_bracket_vec;	// 简单规范后的括号中的内容

	vector<string> m_name_vec;		// 名称分词后的结果
	vector<string> m_bracket_vec;	// 括号中内容分词的结果
	set<string> m_name_set;
	set<string> m_bracket_set;

	string m_main_type;				// 原始大类
	string m_sub_type;				// 原始中类
	string m_soso_type;				// 根据后缀识别的类型编码
	string m_soso_main_type;		// 后缀识别类型的大类编码
	string m_soso_sub_type;			// 后缀识别类型的中类编码
	string m_accurate_main_type;	// 子点识别的类型大类
	string m_accurate_sub_type;		// 子点识别的类型种类

	// 地址相似需要的预处理数据
	map<int, string> m_addrsim_xzqh_map;
	map<string, string> m_addrsim_road_map;
	set<string> m_addrsim_community_set;
	set<string> m_addrsim_poi_set;
	set<string> m_addrsim_hn_set;
	set<string> m_addrsim_home_set;
	set<string> m_addrsim_floor_set;

	set<string> m_num_name_set;		// 名称数量词识别结果
	set<string> m_door_set;			// 名称门类信息识别结果，如：“颐和园东门”中的“东门”
	set<string> m_dpmark_set;		// 名称中的区域标记词，如：东区，东园等

	double m_obj_max_distance;		// 如果是连锁店，可以直接聚合的最大距离
	set<string> m_obj_name_set;		// 名称包含的连锁店名集合
	string m_gov_tail;				// 县乡村等地区政府级别后缀识别结果

	// 可以直接替换的同义名集合，如：国家奥林匹克体育中心 —— 奥体中心
	// 主要包含一些公司的简称和学校的合并名称，如：北京医科大学 —— 北京大学医学部
	set<int> m_smname_group_set;

	set<string> m_unlack_word_set;	// 名称中包含的不可缺失词，如：北医三院儿科 中的 “儿科”

	//======================================
	// 用于周边检索时候的名称集合
	// 原版：使用bigram，比如：奥体中心 处理结果 ["奥体", "体中", "中心"]
	// 原版问题：召回率高，但是无用召回较多，严重影响速度，
	// 			且对于有些简称，缩写，同音字等召回效果不好
	// 新版：对于短名称使用bigram，对于长名称使用分词，结合规范后的全称、拼音等，
	//		召回率不降低的前提下，增加召回的精准性
	set<string> m_grid_name_set;

	// 政府机构的级别
	// 0: 不知道
	// 1: 国家级
	// 2: 省级
	// 3: 市级
	// 4: 区级
	int m_gov_level;

	// 政府机构的级别关键词，如：中国，美国，海淀区等
	string m_gov_lvl_kw;

	string m_dizhi; 				// 小区的甲乙丙丁等
	string m_fendian;				// 存放类似 肯德基中关村店中“中关村”的识别结果

public:
	void clear() {
		m_is_null = true;
		m_status = 0;

		m_id = 0;

		m_lat = 0.0;
		m_lng = 0.0;

		m_name = "";
		m_addr = "";
		m_split_addr = "";
		m_addr_level = 100;
		m_type = "";
		m_source = "";
		m_province = "";
		m_city = "";
		m_district = "";
		m_tel_set.clear();

		// preproc
		m_accurate_type = "";
		m_accurate_keyword = "";
		m_accurate_kw_vec.clear();
		m_accurate_kw_set.clear();

		m_format_name = ""; // format name string
		m_del_punc_name = ""; // del all punc from name
		m_del_punc_name_len = 0;
		m_bigram_name = "";
		m_raw_bracket_vec.clear(); // after format, the deleted string that in brackets

		m_name_vec.clear(); // wordsplit result
		m_bracket_vec.clear();
		m_name_set.clear(); // wordsplit result in set
		m_bracket_set.clear();

		m_main_type = "";
		m_sub_type = "";
		m_soso_type = "";
		m_soso_main_type = "";
		m_soso_sub_type = "";
		m_accurate_main_type = "";
		m_accurate_sub_type = "";

		m_addrsim_xzqh_map.clear();
		m_addrsim_road_map.clear();
		m_addrsim_community_set.clear();
		m_addrsim_poi_set.clear();
		m_addrsim_hn_set.clear();
		m_addrsim_home_set.clear();
		m_addrsim_floor_set.clear();

		m_num_name_set.clear();
		m_door_set.clear();
		m_dpmark_set.clear();

		m_obj_max_distance = 0.0;
		m_obj_name_set.clear();
		m_gov_tail = "";
		m_smname_group_set.clear();
		m_unlack_word_set.clear();
		m_grid_name_set.clear();

		m_gov_level = 0;
		m_gov_lvl_kw = "";

		m_dizhi = "";
		m_fendian = "";
	}

	PreProcData() {
		clear();
	}
};

//===============================================
// 存放所有预处理结果的结构
//===============================================
typedef map<unsigned long long, PreProcData> PreProcMap;


//===============================================
// 用于周边召回的key结构体
//===============================================
struct GridKey {
	int lat;
	int lng;
	string keyword;

	GridKey() {
		lat = 0;
		lng = 0;
		keyword = "";
	}
};

//===============================================
// 因为使用结构体作为map的key
// 所以针对周边召回使用的结构体做了定制的比较函数
//===============================================
struct GridKeyCmp {
	bool operator ()(const GridKey& key1, const GridKey& key2) const {
		if (key1.lat < key2.lat) {
			return true;
		} else if (key1.lat == key2.lat) {
			if (key1.lng < key2.lng)
				return true;
			else if (key1.lng == key2.lng) {
				if (key1.keyword.compare(key2.keyword) < 0)
					return true;
				else
					return false;
			} else
				return false;
		}

		return false;
	}
};

//===============================================
// 存放周边检索的索引，key为切分名称+坐标的结构体，value为poiid
//===============================================
typedef map<GridKey, set<unsigned long long>, GridKeyCmp> GridMap;

//===============================================
// 输入文件转为可用结构需要的配置数据定义
// 说明：
//		1. 输入文件支持每条poi占一行，且特定字符串作为分割符的结构
//		2. 结构中需要定义record对应字段在文档中的位置索引，从0开始计数
//===============================================
class CsvIndexCfg {
public:
	bool m_debug;			// 是否是debug方式，true是

	int m_id;				// 文档中id的index
	int m_lat;				// 文档中纬度的index
	int m_lng;				// 文档中经度的index
	int m_name;				// 文档中名称的index
	int m_addr;				// 文档中地址的index
	int m_split_addr;		// 文档中分割地址的index
	int m_addr_level;		// 文档中地址级别的index
	int m_type;				// 文档中类型码的index
	int m_type_name;		// 文档中类型名称的index
	int m_source;			// 文档中来源的index
	int m_province;			// 文档中省的index
	int m_city;				// 文档中市的index
	int m_district;			// 文档中区的index
	int m_accurate_type;	// 文档中子点识别结果的index
	int m_tel;				// 文档中电话的index
	int m_status;			// 文档中有效状态的index

	int m_max_index;		// 文档中需要使用字段的最大的index，主要是用于检测前面的配置是否有问题
	int m_thread_num;		// 使用线程数量
	string m_delim;			// 分隔符
public:
	void clear() {
		m_debug = false;

		m_id = -1;
		m_lat = -1;
		m_lng = -1;
		m_name = -1;
		m_addr = -1;
		m_split_addr = -1;
		m_addr_level = -1;
		m_type = -1;
		m_type_name = -1;
		m_source = -1;
		m_province = -1;
		m_city = -1;
		m_district = -1;
		m_accurate_type = -1;
		m_tel = -1;
		m_status = -1;

		m_max_index = 52;
		m_thread_num = 4;

		m_delim = "";
	}

	CsvIndexCfg() {
		clear();
	}
};

//=====================================================================
// poi一对一相似度计算产生的所有中间结果缓存结构体
//=====================================================================
class SimTempCache {
public:
	bool m_is_null;						// 标记结构是否为空
	int m_status; 						// 0: 可用，1：不可用
	bool m_is_base; 					// 跟base集合找到相似数据

	// chn类型的数据聚合标记
	// 当使用该标记时，会使用一些特别的策略
	bool m_is_mode_chn;

	// index库自己聚合标记
	bool m_is_mode_self;


	unsigned long long m_base_id;		// 聚合时认为的baseid
	unsigned long long m_inc_id;		// 聚合时认为的incid

	string m_base_name;					// 聚合时认为的base_name
	string m_inc_name;					// 聚合时认为的inc_name

	// step
	// 值为true：该节点有确定的结果，后续节点不需要继续处理
	// 值为false：该节点没有确定的结果，后续节点需要继续处理
	map<string, bool> m_rule_result_map;

	double m_distance;					// poi相距距离
	double m_addr_sim;					// 地址相似度
	set<string> m_obj_name_set;			// 两个poi相同的连锁店名集合
	double m_max_obj_distance;			// 连锁店可以直接聚合的最大距离
	set<string> m_spec_obj_name_set;	// 特别类型的连锁机构的名称集合，如加油站，停车场等
	double m_max_spec_obj_distance;		// 特殊类型可以直接聚合的最大距离
	int m_len_diff;						// 编辑距离
	bool m_name_is_include;				// 名称是否有包含关系
	AddressSimilarityValue m_addr_sim_value; 	//存放地址相似度计算的细节

	set<string> m_same_tel_set;			// 相同电话集合
	set<string> m_same_type_set;		// 不同类型通过映射聚合时保存的类型表
	string m_same_type_str; 			// 两条poi为相同类型（非类型映射），该属性有值，不为空

	vector<string> m_same_words_vec;	// 名称中相同的词
	vector<string> m_sub_same_words_base_vec;	// base名称中相同但认为不重要的词
	vector<string> m_sub_same_words_inc_vec;	// inc名称中相同但认为不重要的词
	vector<string> m_diff_words_base_vec;		// base名称中不相同的词
	vector<string> m_diff_words_inc_vec;		// inc名称中不相同的词
	vector<string> m_sub_diff_words_base_vec;	// base名称中不相同但认为不重要的词
	vector<string> m_sub_diff_words_inc_vec;	// inc名称中不相同但认为不重要的词

	vector<string> m_bracket_same_words_vec;	// 名称的括号中认为相同的词
	vector<string> m_bracket_diff_words_base_vec;	// base名称括号中认为不同的词
	vector<string> m_bracket_diff_words_inc_vec;	// inc名称括号中认为不同的词

	// accurate keyword
	vector<string> m_accurate_kw_base_vec;		// base子点识别出来的关键词
	vector<string> m_accurate_kw_inc_vec;		// inc子点识别出来的关键词

	// name similarity
	double m_name_same_score;			// 名称相同部分的得分
	double m_name_sub_same_score;		// 名称辅相同部分的得分
	double m_name_diff_score;			// 名称不相同部分的得分
	double m_name_sub_diff_score;		// 名称辅相同部分的得分
	double m_bracket_score;				// 括号中内容相似度得分

	// base score
	double m_name_score;				// 名称相似度总分
	double m_tel_score;					// 电话相似度总分
	double m_addr_score;				// 地址相似度总分
	double m_type_score;				// 类型相似度总分
	double m_distance_score;			// 距离相似度总分

	//=====================================================================
	// special score
	// 以下的得分主要用于对前面得分的修正
	//=====================================================================
	// 不同的类型、不同的来源等数据，需要在基本得分基础上做一些针对性的处理
	double m_accurate_type_score;		// 子点识别得分

	// 名称有包含，且一边是另外一边的结尾形态需要做一些加分处理
	// 故宫博物院漱芳斋   漱芳斋
	double m_tail_score;

	// 一些特殊类型需要根据情况做一些减分
	// 如:
	//    1. 不同类型通过映射聚合的需要做一定减分
	//    2. 其他类别、公司类别等跟别的类别数据进行聚合的减分
	double m_spec_score;

	// 核心名称包含在另一条数据的括号中时，需要对特别类型的做减分处理
	// 肯德基    **小吃（肯德基旁）
	double m_in_bracket_score;

	// 名称中有数量词相同修正分
	double m_same_num_score;

	// 非频繁词扣分
	// 如果名称中包含一些非频繁词出现在不同部分需要根据其类型做不同的扣分处理
	double m_area_sub_poi_score;

	// 名称有包含关系
	// 地址或电话相同的情况
	double m_include_addr_tel_score;

	// 总相似得分
	double m_total_same_score;
	// 总不同得分
	double m_total_diff_score;
	// 总相似度得分
	double m_score;

public:
	void clear() {
		m_is_null = true;
		m_status = 0;
		m_is_base = false;

		m_is_mode_chn = false;
		m_is_mode_self = false;

		m_base_id = 0;
		m_inc_id = 0;

		m_base_name = "";
		m_inc_name = "";

		m_rule_result_map.clear();

		m_distance = 9999.0;
		m_addr_sim = 0.0;
		m_obj_name_set.clear();
		m_max_obj_distance = 9999.0;
		m_spec_obj_name_set.clear();
		m_max_spec_obj_distance = 9999.0;
		m_len_diff = 999;
		m_name_is_include = false;

		m_addr_sim_value.clear();

		m_same_tel_set.clear();
		//m_same_type_vec.clear();
		m_same_type_set.clear();
		m_same_type_str = "";

		m_same_words_vec.clear();
		m_sub_same_words_base_vec.clear();
		m_sub_same_words_inc_vec.clear();
		m_diff_words_base_vec.clear();
		m_diff_words_inc_vec.clear();
		m_sub_diff_words_base_vec.clear();
		m_sub_diff_words_inc_vec.clear();

		m_bracket_same_words_vec.clear();
		m_bracket_diff_words_base_vec.clear();
		m_bracket_diff_words_inc_vec.clear();

		// accurate keyword
		m_accurate_kw_base_vec.clear();
		m_accurate_kw_inc_vec.clear();

		// name similarity
		m_name_same_score = 0.0;
		m_name_sub_same_score = 0.0;
		m_name_diff_score = 0.0;
		m_name_sub_diff_score = 0.0;
		m_bracket_score = 0.0;

		// base score
		m_name_score = 0.0;
		m_tel_score = 0.0;
		m_addr_score = 0.0;
		m_type_score = 0.0;
		m_distance_score = 0.0;

		// special score
		m_accurate_type_score = 0.0;
		m_tail_score = 0.0; // if one in endswith other
		m_spec_score = 0.0;
		m_in_bracket_score = 0.0;
		m_same_num_score = 0.0;
		m_area_sub_poi_score = 0.0;
		m_include_addr_tel_score = 0.0;

		m_total_same_score = 0.0;
		m_total_diff_score = 0.0;
		m_score = -1.0;
	}

	SimTempCache() {
		clear();
	}
};

//=====================================================================
// poi聚合用于保存没有聚合上的id集合
// 说明：
// 		由于后期增加了冲突检测，相似度计算中即便认为相同的id关系也有可能在后期
// 因为冲突关系检测的问题成为一个孤立点，所以该结构现在基本已经弃用
//=====================================================================
class SimResultCache {
public:
	map<unsigned long long, bool> m_base_sgl_map;
	map<unsigned long long, bool> m_inc_sgl_map;
};

#endif /* DATA_H_ */
