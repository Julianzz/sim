/*
 * sim.h
 *
 *  Created on: 2012-10-26
 *      Author: stefanyang
 *
 *  poi相似度计算的核心功能代码
 *
 *	poi相似度计算说明
 *  	poi相似度计算主要是根据poi的名称、地址、经纬度、类型、电话等特征属性计算
 *  其是否是同一个poi的相似值
 *		由于数据来源多样，数据的特征并不是统一，且poi的数据量相对比较大，因此，
 *	poi相似度计算采用规则和中文处理算法结合的方式，以最大限度的提高相似度计算的精度，
 *	为后续poi聚类打下良好的基础
 *
 *	相关概念说明
 *	规则节点 rulenode：每个功能节点都是一个规则，规则之间尽量独立，外部调用规则是使用
 *		函数指针进行调用。这样的好处是，可以把比较复杂的冗长的规则集划分为多个独立的
 *		小规则，便于程序的维护，另外，对于多个规则之间可以方便的按照不同的顺序组合，
 *		提高了代码的复用
 *
 */

#ifndef SIM_H_
#define SIM_H_

#include <pthread.h>
#include "data.h"
#include "tools.h"
#include "prepare.h"
#include "typeDefine.h"

// poi相似度计算配置结构体声明
class PoiSimCfg;

//=======================================================================
// 参数说明：
// const PreProcData *, const PreProcData *: 参加计算的预处理过的数据
// const void *：规则节点的参数配置
// const Resource *：相似度计算需要的资源集合
// SimTempCache *： 相似度计算的所有结果集合
typedef bool (*RuleFuncp)(const PreProcData *, const PreProcData *,
		const void *, const Resource *, SimTempCache *);

//==================================================
// 规则节点
//==================================================
class RuleNode {
public:
	RuleFuncp fp; // 规则节点对应的函数
	void *cfg; // 配置

public:
	void clear() {
		fp = NULL;
		cfg = NULL;
	}

	RuleNode() {
		clear();
	}
};

//==================================================
// 相似度计算规则节点参数类
//==================================================
class RuleSpecSourceCfg {
public:
	vector<string> *m_source_name_vec;

public:
	RuleSpecSourceCfg() {
		m_source_name_vec = NULL;
	}
};

class RuleDiffDoorCfg {
public:
	double m_distance;
};

class RuleDiffDpmarkCfg {
public:
	double m_distance;
	double m_addr_sim;
};

class RuleDiffGovTailCfg {
public:
	set<string> *m_type_set;

public:
	RuleDiffGovTailCfg() {
		m_type_set = NULL;
	}
};

class RuleSameNameCfg {
public:
	double m_recall_distance;
	double m_reliable_distance;
	double m_addr_sim;
};

class RuleDistanceCfg {
public:
	double m_airport_recall_distance;
	double m_airport_reliable_distance;

	double m_train_recall_distance;
	double m_train_reliable_distance;

	double m_default_recall_distance;
	double m_default_reliable_distance;

	double m_addr_sim;
};

class RuleRoadCfg {
public:
	double m_reliable_distance;
};

// 相似度计算的函数指针
// 因为计算结果分为找到所有相似poi，最相似poi，第一个相似poi等
typedef void (*PoiSimFunc)(const PreProcData *pre_proc_data,
		const PreProcMap *data_map, const set<unsigned long long> *id_set,
		const Resource *resource, const PoiSimCfg *poi_sim_cfg,
		const vector<RuleNode> *rule_node_vec, bool is_base,
		SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

//==================================================
// 辅助型函数
//==================================================
bool setResult(const string &step_name, const double &score,
		const bool return_value, SimTempCache *sim_temp_cache);

// 名称是否相同
// 不仅检查简单比较字符串，且会考虑包含关系和汉语拼音的相似情况
bool nameIsSame(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource);

// 检查是否是有效的相似度配置参数
bool isValidCfg(const PoiSimCfg *poi_sim_cfg);

// 根据地址相似度以及地址的精细级别，判断地址是否是可信的相似
bool isSameAddr(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache);

// 判断分店特证词是否相同
int isSameFenDian(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache);

// 名称中是否包含加油站、停车场等
bool hasSpecObj(const PreProcData *pre_proc_data, const Resource *resource);

//==================================================
// 一对一基本计算函数
//==================================================
// 获取poi之间的距离
void getDistance(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache);

// 获取poi名称的编辑距离
void getLenDiff(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache);

// 获取poi地址的相似度
void getAddrSim(const PreProcData *base_data, const PreProcData *inc_data,
		Resource *resource, SimTempCache *sim_temp_cache);

// 获取poi中相同的电话集合
void getSameTel(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache);

// 获取poi认为相同的类型编码或类型映射关系
void getSameType(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache);

// 根据分词结果，获取名称中包含不同词
void getDiffName(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache);

// 获取连锁店及对应属性
void getObj(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache);

// 根据idf计算括号内容的相似度值
void getBracketScore(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache, const Resource *resource);

// 检测名称是否有包含关系
void getIncludeName(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache);

//==================================================
// 一对一快速判断的规则函数
// 返回值：
// 	false: 不确定相似或不相似
//	true: 确定相似或不相似
//==================================================
// 判断是否是相同id
bool ruleSameID(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否是特殊来源的数据，部分特殊来源数据不参与相似度计算
bool ruleSpecSource(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否包含不同的门数据
bool ruleDiffDoor(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断名称是否是有简称、合并名的对应关系
bool ruleAlias(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否包含不同区域信息
bool ruleDiffDpMark(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否包含不同行政级别的后缀
bool ruleDiffGovTail(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否名称就是一个类型词，如酒店等，并做针对性的规则处理
bool ruleTypeName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否是连锁店，并做对应的规则处理
bool ruleChain(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断名称是否完全相同
// 由于数据来自不同来源，可能出现同音异形的名称，在该节点中做了对应的考虑和处理
bool ruleSameName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否是不同的类别
// 由于数据来自不同的来源，类型并不是完全按照同一个类别体系建立，并且内部现有体系
// 类别间有时还有交叉，如：足疗 -- 医疗保健
// 因此类型判断不是简单的判断其类型吗是否相同，而是：
// 1. 判断原始类型是否相同
// 2. 根据后缀识别类型，判断类型是否相同
// 3. 按照映射表对比类型，看看是否存在映射关系
bool ruleDiffType(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断距离是否是在poi相似度计算可信距离之类
// 由于不同的类别，特征不一样，其可信的召回距离也不一样，如机场、学校、餐厅等就分别采用不同的召回距离
bool ruleDistance(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否是桥类数据，并做相应的处理
bool ruleBridge(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否是自动取款机，并做相应的处理
bool ruleAtm(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断poi名称是否完全包含于别的poi名称的括号中，并做相应处理
bool ruleInBracket(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否是路数据
bool ruleRoad(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否名称中是否出现了不可缺失词在不同部分，并做相应的处理
bool ruleUnlackWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断不同部分是否出现不可缺失词
// 由于类型的问题，有的不可能缺失词不能识别
// 但是当出现在不同部分时，需要根据可聚合的所有类别考虑不可缺失词
bool ruleDiffWordUnlack(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否名称中是否出现了不同的数字信息，并做相应的处理
bool ruleDiffNum(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否名称中是否出现了不同的地支信息，如甲乙丙丁等，并做相应的处理
// 主要是针对小区类，如：天通苑甲30号    天通苑乙30号
bool ruleDiffDizhi(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 判断是否名称中是否出现了不同的分店信息，并做相应的处理
bool ruleDiffFendian(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 在后缀类型特征明显的情况，判断类型是否有强烈冲突不能聚合
bool ruleDiffSoSoType(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 根据子点特征识别结果，判断是否是可聚合的子点
bool ruleDiffAccurateKeyWord(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 针对停车场做的特别的处理
bool ruleDiffPark(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 针对行政机构做的省市区行政级别的识别和判断
bool ruleDiffGovLevel(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 针对大厦类做的特别的处理
bool ruleDiffBuildType(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 处理火车站类型，如北京 北京站  类型相同，名称不规范的情况
bool ruleTrain(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 当index数据自己聚合的时候需要做的特别的规则处理
bool ruleSelfSim(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 名称相似度小于阈值时，需要做的特别的规则处理
bool ruleDiffName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 名称中包含非频繁词，且出现在不同部分时，做的特别的规则处理
bool ruleNotFreqWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 针对chn来源数据做的特别的处理
bool ruleChnSim(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

//==================================================
// 做完交叉处理后的规则处理
//==================================================
// 将不同部分出现在对方括号中的词放到辅相同中
void dealInBracket(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 将不同部分拼音出现在对方括号中的词放到辅相同中
void dealPinYin(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 将不同部分去掉无意义后缀字后相同的词放到辅相同中
void dealInclude(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 将不同部分词包含于主相同部分的词放到辅相同中
void dealIncludeInSame(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 当只有一边的不同部分出现道路信息，认为该道路信息不算不同，放到辅相同中
void dealOneSideRoad(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 转移相同部分的类型词到辅相同中
void dealTypeWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 将相同部分一些类型词放到辅相同中，如：石房房地产评估事务所	兴诚房地产评估公司 中
// 的 房地产
void dealSameTypeWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 将连锁店名放到辅相同中
void dealChainName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 针对机构类型的辅相同部分的处理
void dealGovDistrict(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// 将有包含的分店信息从不同部分去除
void dealFendian(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);
//==================================================
// 基本因子相似度计算
//==================================================
void computeNameScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

void computeTelScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

void computeTypeScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

void computeAddrScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

void computeDistanceScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

//==================================================
// 特别属性相似度计算
//==================================================
void computeSameTailScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

void computeAccurateTypeScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

void computeSpecTypeScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

void computeSameNumScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

void computeSameAT(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

void computeAreaSubPoiScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

//==================================================
// 相似度总分计算
//==================================================
void computeScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

//==================================================
// 比较相似度大小
//==================================================
void compareSimScore(SimTempCache *cur_max_cache, SimTempCache *temp_cache);

//==================================================
// 结果输出
//==================================================
void outputResult(const SimTempCache *sim_temp_cache, bool is_base,
		bool is_debug, map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

void outputResult(const SimTempCache *sim_temp_cache, bool is_base,
		pthread_mutex_t &prepare_output_mutex);

//==================================================
// 相似度计算需要缓存的结果的管理
//==================================================
void initResultCache(const PreProcMap *data_map,
		map<unsigned long long, bool> &sgl_map);

void outputSgl(const map<unsigned long long, bool> &sgl_map, bool is_base);

//==================================================
// 删除inc中存在的baseid
//==================================================
void delSameId(cacheResultMap &baseMap, cacheResultMap &incMap);

//==================================================
// 相似度计算及输出
//==================================================
// 一对一
void poiSim(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, const PoiSimCfg *poi_sim_cfg,
		const vector<RuleNode> *rule_node_vec, SimTempCache *sim_temp_cache,
		bool is_base);

// 一对多
typedef void (*PoiSimFuncP)(const PreProcData *pre_proc_data,
		const PreProcMap *data_map, const set<unsigned long long> *id_set,
		const Resource *resource, const PoiSimCfg *poi_sim_cfg,
		const vector<RuleNode> *rule_node_vec, bool is_base,
		SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

//输出找到的第一个相似poi
void poiSimFirst(const PreProcData *pre_proc_data, const PreProcMap *data_map,
		const set<unsigned long long> *id_set, const Resource *resource,
		const PoiSimCfg *poi_sim_cfg, const vector<RuleNode> *rule_node_vec,
		bool is_base, SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

//输出找到的所有相似poi
void poiSimAll(const PreProcData *pre_proc_data, const PreProcMap *data_map,
		const set<unsigned long long> *id_set, const Resource *resource,
		const PoiSimCfg *poi_sim_cfg, const vector<RuleNode> *rule_node_vec,
		bool is_base, SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

//输出找到的最相似poi
void poiSimMax(const PreProcData *pre_proc_data, const PreProcMap *data_map,
		const set<unsigned long long> *id_set, const Resource *resource,
		const PoiSimCfg *poi_sim_cfg, const vector<RuleNode> *rule_node_vec,
		bool is_base, SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

void poiSim(const PreProcData *pre_proc_data, const PreProcMap *data_map,
		const GridMap *grid_map, const Resource *resource,
		const PoiSimCfg *poi_sim_cfg, const vector<RuleNode> *rule_node_vec,
		PoiSimFunc poi_sim_func, bool is_base, SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

// 多线程参数结构体
struct PoiSimParam {
	PreProcMap::const_iterator it;
	PreProcMap::const_iterator end_it;
	const PreProcMap *base_map;
	const PreProcMap *inc_map;
	const GridMap *base_grid_map;
	const GridMap *inc_grid_map;
	const Resource *resource;
	const PoiSimCfg *poi_sim_cfg;
	map<string, PoiSimFunc> *poi_sim_func_map;
	SimResultCache *sim_result_cache;
	pthread_mutex_t *prepare_read_mutex;
	pthread_mutex_t *prepare_output_mutex;
};

// poi相似度计算的参数配置结构体
class PoiSimCfg {
public:
	string m_source_dir; // source目录路径
	bool m_use_local_idf; // 是否使用实时idf表，true为使用非实时表
	bool m_debug; // 是否输出调试信息，true输出
	bool m_find_in_self; // 是否在自身集合中查找重复数据
	bool m_del_same_id; // 是否将相同id从待处理集合中删除

	bool m_out_base_sgl; // 是否输出base没有聚合的id
	bool m_out_inc_sgl; // 是否输出inc没有聚合的id

	double m_name_same_weight; // 名称中相同的部分在相似度中的权重
	double m_name_sub_same_weight; // 名称中辅相同的部分在相似度中的权重
	double m_name_diff_weight; // 名称中不相同的部分在相似度中的权重
	double m_name_sub_diff_weight; // 名称中辅不相同的部分在相似度中的权重

	double m_poisim_all_output_th; // 相似度计算输出的阈值

	int m_useless_tel_threshold; // 有效电话出现频次的阈值
	int m_thread_num; // 并行处理的线程数

	PoiSimFunc m_base_inc_sim_func; // base和inc数据相似度计算的规则集合函数
	PoiSimFunc m_inc_inc_sim_func; // inc和inc数据相似度计算的规则集合函数

	vector<RuleNode> m_base_inc_rule_node_vec; // base和inc数据相似度计算的规则集合
	vector<RuleNode> m_inc_inc_rule_node_vec; // inc和inc数据相似度计算的规则集合

	vector<string> m_wordsplit_dic_vec; // 分词需要使用词表列表

public:
	void clear() {
		m_source_dir = "";
		m_use_local_idf = false;
		m_debug = false;

		m_find_in_self = true;
		m_del_same_id = true;

		m_out_base_sgl = true;
		m_out_inc_sgl = true;

		m_name_same_weight = 0.625;
		m_name_sub_same_weight = 0.375;
		m_name_diff_weight = 0.625;
		m_name_sub_diff_weight = 0.375;

		m_poisim_all_output_th = -0.4;

		m_useless_tel_threshold = 50;
		m_thread_num = 4;

		m_base_inc_sim_func = poiSimMax;
		m_inc_inc_sim_func = poiSimAll;

		m_base_inc_rule_node_vec.clear();
		m_inc_inc_rule_node_vec.clear();

		m_wordsplit_dic_vec.clear();
	}

	PoiSimCfg() {
		clear();
	}
};

//=================================================
// 装载resource所有内容
//=================================================
void loadResource(const PoiSimCfg *poi_sim_cfg, Resource *resource);
void statResource(const PreProcMap *pre_proc_map, Resource *resource);
void updateResourse(long poi_num, int threshold, Resource *resource);

//=================================================
// 装载tools所有内容
//=================================================
void loadTools(const PoiSimCfg *poi_sim_cfg, Tools *tools);

// 多对多
void poiSim(PreProcMap::const_iterator &it, PreProcMap::const_iterator &end_it,
		const PreProcMap *base_map, const PreProcMap *inc_map,
		const GridMap *base_grid_map, const GridMap *inc_grid_map,
		const Resource *resource, const PoiSimCfg *poi_sim_cfg,
		map<string, PoiSimFunc> &poi_sim_func_map,
		SimResultCache *sim_result_cache, pthread_mutex_t &prepare_read_mutex,
		pthread_mutex_t &prepare_output_mutex);

void *poiSimWraper(void *param);

void poiSim(PreProcMap *base_map, PreProcMap *inc_map, Resource *resource,
		PoiSimCfg *poi_sim_cfg);

#endif /* SIM_H_ */
