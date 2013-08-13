/*
 * sim.h
 *
 *  Created on: 2012-10-26
 *      Author: stefanyang
 *
 *  poi���ƶȼ���ĺ��Ĺ��ܴ���
 *
 *	poi���ƶȼ���˵��
 *  	poi���ƶȼ�����Ҫ�Ǹ���poi�����ơ���ַ����γ�ȡ����͡��绰���������Լ���
 *  ���Ƿ���ͬһ��poi������ֵ
 *		����������Դ���������ݵ�����������ͳһ����poi����������ԱȽϴ���ˣ�
 *	poi���ƶȼ�����ù�������Ĵ����㷨��ϵķ�ʽ��������޶ȵ�������ƶȼ���ľ��ȣ�
 *	Ϊ����poi����������õĻ���
 *
 *	��ظ���˵��
 *	����ڵ� rulenode��ÿ�����ܽڵ㶼��һ�����򣬹���֮�価���������ⲿ���ù�����ʹ��
 *		����ָ����е��á������ĺô��ǣ����԰ѱȽϸ��ӵ��߳��Ĺ��򼯻���Ϊ���������
 *		С���򣬱��ڳ����ά�������⣬���ڶ������֮����Է���İ��ղ�ͬ��˳����ϣ�
 *		����˴���ĸ���
 *
 */

#ifndef SIM_H_
#define SIM_H_

#include <pthread.h>
#include "data.h"
#include "tools.h"
#include "prepare.h"
#include "typeDefine.h"

// poi���ƶȼ������ýṹ������
class PoiSimCfg;

//=======================================================================
// ����˵����
// const PreProcData *, const PreProcData *: �μӼ����Ԥ�����������
// const void *������ڵ�Ĳ�������
// const Resource *�����ƶȼ�����Ҫ����Դ����
// SimTempCache *�� ���ƶȼ�������н������
typedef bool (*RuleFuncp)(const PreProcData *, const PreProcData *,
		const void *, const Resource *, SimTempCache *);

//==================================================
// ����ڵ�
//==================================================
class RuleNode {
public:
	RuleFuncp fp; // ����ڵ��Ӧ�ĺ���
	void *cfg; // ����

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
// ���ƶȼ������ڵ������
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

// ���ƶȼ���ĺ���ָ��
// ��Ϊ��������Ϊ�ҵ���������poi��������poi����һ������poi��
typedef void (*PoiSimFunc)(const PreProcData *pre_proc_data,
		const PreProcMap *data_map, const set<unsigned long long> *id_set,
		const Resource *resource, const PoiSimCfg *poi_sim_cfg,
		const vector<RuleNode> *rule_node_vec, bool is_base,
		SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

//==================================================
// �����ͺ���
//==================================================
bool setResult(const string &step_name, const double &score,
		const bool return_value, SimTempCache *sim_temp_cache);

// �����Ƿ���ͬ
// �������򵥱Ƚ��ַ������һῼ�ǰ�����ϵ�ͺ���ƴ�����������
bool nameIsSame(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource);

// ����Ƿ�����Ч�����ƶ����ò���
bool isValidCfg(const PoiSimCfg *poi_sim_cfg);

// ���ݵ�ַ���ƶ��Լ���ַ�ľ�ϸ�����жϵ�ַ�Ƿ��ǿ��ŵ�����
bool isSameAddr(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache);

// �жϷֵ���֤���Ƿ���ͬ
int isSameFenDian(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache);

// �������Ƿ��������վ��ͣ������
bool hasSpecObj(const PreProcData *pre_proc_data, const Resource *resource);

//==================================================
// һ��һ�������㺯��
//==================================================
// ��ȡpoi֮��ľ���
void getDistance(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache);

// ��ȡpoi���Ƶı༭����
void getLenDiff(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache);

// ��ȡpoi��ַ�����ƶ�
void getAddrSim(const PreProcData *base_data, const PreProcData *inc_data,
		Resource *resource, SimTempCache *sim_temp_cache);

// ��ȡpoi����ͬ�ĵ绰����
void getSameTel(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache);

// ��ȡpoi��Ϊ��ͬ�����ͱ��������ӳ���ϵ
void getSameType(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache);

// ���ݷִʽ������ȡ�����а�����ͬ��
void getDiffName(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache);

// ��ȡ�����꼰��Ӧ����
void getObj(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache);

// ����idf�����������ݵ����ƶ�ֵ
void getBracketScore(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache, const Resource *resource);

// ��������Ƿ��а�����ϵ
void getIncludeName(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache);

//==================================================
// һ��һ�����жϵĹ�����
// ����ֵ��
// 	false: ��ȷ�����ƻ�����
//	true: ȷ�����ƻ�����
//==================================================
// �ж��Ƿ�����ͬid
bool ruleSameID(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ���������Դ�����ݣ�����������Դ���ݲ��������ƶȼ���
bool ruleSpecSource(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ������ͬ��������
bool ruleDiffDoor(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж������Ƿ����м�ơ��ϲ����Ķ�Ӧ��ϵ
bool ruleAlias(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ������ͬ������Ϣ
bool ruleDiffDpMark(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ������ͬ��������ĺ�׺
bool ruleDiffGovTail(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ����ƾ���һ�����ʹʣ���Ƶ�ȣ���������ԵĹ�����
bool ruleTypeName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ��������꣬������Ӧ�Ĺ�����
bool ruleChain(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж������Ƿ���ȫ��ͬ
// �����������Բ�ͬ��Դ�����ܳ���ͬ�����ε����ƣ��ڸýڵ������˶�Ӧ�Ŀ��Ǻʹ���
bool ruleSameName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ��ǲ�ͬ�����
// �����������Բ�ͬ����Դ�����Ͳ�������ȫ����ͬһ�������ϵ�����������ڲ�������ϵ
// ������ʱ���н��棬�磺���� -- ҽ�Ʊ���
// ��������жϲ��Ǽ򵥵��ж����������Ƿ���ͬ�����ǣ�
// 1. �ж�ԭʼ�����Ƿ���ͬ
// 2. ���ݺ�׺ʶ�����ͣ��ж������Ƿ���ͬ
// 3. ����ӳ���Ա����ͣ������Ƿ����ӳ���ϵ
bool ruleDiffType(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �жϾ����Ƿ�����poi���ƶȼ�����ž���֮��
// ���ڲ�ͬ�����������һ��������ŵ��ٻؾ���Ҳ��һ�����������ѧУ�������Ⱦͷֱ���ò�ͬ���ٻؾ���
bool ruleDistance(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ����������ݣ�������Ӧ�Ĵ���
bool ruleBridge(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ����Զ�ȡ�����������Ӧ�Ĵ���
bool ruleAtm(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж�poi�����Ƿ���ȫ�����ڱ��poi���Ƶ������У�������Ӧ����
bool ruleInBracket(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ���·����
bool ruleRoad(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ��������Ƿ�����˲���ȱʧ���ڲ�ͬ���֣�������Ӧ�Ĵ���
bool ruleUnlackWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �жϲ�ͬ�����Ƿ���ֲ���ȱʧ��
// �������͵����⣬�еĲ�����ȱʧ�ʲ���ʶ��
// ���ǵ������ڲ�ͬ����ʱ����Ҫ���ݿɾۺϵ���������ǲ���ȱʧ��
bool ruleDiffWordUnlack(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ��������Ƿ�����˲�ͬ��������Ϣ��������Ӧ�Ĵ���
bool ruleDiffNum(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ��������Ƿ�����˲�ͬ�ĵ�֧��Ϣ������ұ����ȣ�������Ӧ�Ĵ���
// ��Ҫ�����С���࣬�磺��ͨԷ��30��    ��ͨԷ��30��
bool ruleDiffDizhi(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ж��Ƿ��������Ƿ�����˲�ͬ�ķֵ���Ϣ��������Ӧ�Ĵ���
bool ruleDiffFendian(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �ں�׺�����������Ե�������ж������Ƿ���ǿ�ҳ�ͻ���ܾۺ�
bool ruleDiffSoSoType(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �����ӵ�����ʶ�������ж��Ƿ��ǿɾۺϵ��ӵ�
bool ruleDiffAccurateKeyWord(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ���ͣ���������ر�Ĵ���
bool ruleDiffPark(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ���������������ʡ�������������ʶ����ж�
bool ruleDiffGovLevel(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ��Դ����������ر�Ĵ���
bool ruleDiffBuildType(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �����վ���ͣ��籱�� ����վ  ������ͬ�����Ʋ��淶�����
bool ruleTrain(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ��index�����Լ��ۺϵ�ʱ����Ҫ�����ر�Ĺ�����
bool ruleSelfSim(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �������ƶ�С����ֵʱ����Ҫ�����ر�Ĺ�����
bool ruleDiffName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �����а�����Ƶ���ʣ��ҳ����ڲ�ͬ����ʱ�������ر�Ĺ�����
bool ruleNotFreqWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ���chn��Դ���������ر�Ĵ���
bool ruleChnSim(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

//==================================================
// ���꽻�洦���Ĺ�����
//==================================================
// ����ͬ���ֳ����ڶԷ������еĴʷŵ�����ͬ��
void dealInBracket(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ����ͬ����ƴ�������ڶԷ������еĴʷŵ�����ͬ��
void dealPinYin(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ����ͬ����ȥ���������׺�ֺ���ͬ�Ĵʷŵ�����ͬ��
void dealInclude(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ����ͬ���ִʰ���������ͬ���ֵĴʷŵ�����ͬ��
void dealIncludeInSame(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ��ֻ��һ�ߵĲ�ͬ���ֳ��ֵ�·��Ϣ����Ϊ�õ�·��Ϣ���㲻ͬ���ŵ�����ͬ��
void dealOneSideRoad(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ת����ͬ���ֵ����ʹʵ�����ͬ��
void dealTypeWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ����ͬ����һЩ���ʹʷŵ�����ͬ�У��磺ʯ�����ز�����������	�˳Ϸ��ز�������˾ ��
// �� ���ز�
void dealSameTypeWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// �����������ŵ�����ͬ��
void dealChainName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ��Ի������͵ĸ���ͬ���ֵĴ���
void dealGovDistrict(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

// ���а����ķֵ���Ϣ�Ӳ�ͬ����ȥ��
void dealFendian(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);
//==================================================
// �����������ƶȼ���
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
// �ر��������ƶȼ���
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
// ���ƶ��ּܷ���
//==================================================
void computeScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache);

//==================================================
// �Ƚ����ƶȴ�С
//==================================================
void compareSimScore(SimTempCache *cur_max_cache, SimTempCache *temp_cache);

//==================================================
// ������
//==================================================
void outputResult(const SimTempCache *sim_temp_cache, bool is_base,
		bool is_debug, map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

void outputResult(const SimTempCache *sim_temp_cache, bool is_base,
		pthread_mutex_t &prepare_output_mutex);

//==================================================
// ���ƶȼ�����Ҫ����Ľ���Ĺ���
//==================================================
void initResultCache(const PreProcMap *data_map,
		map<unsigned long long, bool> &sgl_map);

void outputSgl(const map<unsigned long long, bool> &sgl_map, bool is_base);

//==================================================
// ɾ��inc�д��ڵ�baseid
//==================================================
void delSameId(cacheResultMap &baseMap, cacheResultMap &incMap);

//==================================================
// ���ƶȼ��㼰���
//==================================================
// һ��һ
void poiSim(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, const PoiSimCfg *poi_sim_cfg,
		const vector<RuleNode> *rule_node_vec, SimTempCache *sim_temp_cache,
		bool is_base);

// һ�Զ�
typedef void (*PoiSimFuncP)(const PreProcData *pre_proc_data,
		const PreProcMap *data_map, const set<unsigned long long> *id_set,
		const Resource *resource, const PoiSimCfg *poi_sim_cfg,
		const vector<RuleNode> *rule_node_vec, bool is_base,
		SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

//����ҵ��ĵ�һ������poi
void poiSimFirst(const PreProcData *pre_proc_data, const PreProcMap *data_map,
		const set<unsigned long long> *id_set, const Resource *resource,
		const PoiSimCfg *poi_sim_cfg, const vector<RuleNode> *rule_node_vec,
		bool is_base, SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

//����ҵ�����������poi
void poiSimAll(const PreProcData *pre_proc_data, const PreProcMap *data_map,
		const set<unsigned long long> *id_set, const Resource *resource,
		const PoiSimCfg *poi_sim_cfg, const vector<RuleNode> *rule_node_vec,
		bool is_base, SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex);

//����ҵ���������poi
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

// ���̲߳����ṹ��
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

// poi���ƶȼ���Ĳ������ýṹ��
class PoiSimCfg {
public:
	string m_source_dir; // sourceĿ¼·��
	bool m_use_local_idf; // �Ƿ�ʹ��ʵʱidf��trueΪʹ�÷�ʵʱ��
	bool m_debug; // �Ƿ����������Ϣ��true���
	bool m_find_in_self; // �Ƿ����������в����ظ�����
	bool m_del_same_id; // �Ƿ���ͬid�Ӵ���������ɾ��

	bool m_out_base_sgl; // �Ƿ����baseû�оۺϵ�id
	bool m_out_inc_sgl; // �Ƿ����incû�оۺϵ�id

	double m_name_same_weight; // ��������ͬ�Ĳ��������ƶ��е�Ȩ��
	double m_name_sub_same_weight; // �����и���ͬ�Ĳ��������ƶ��е�Ȩ��
	double m_name_diff_weight; // �����в���ͬ�Ĳ��������ƶ��е�Ȩ��
	double m_name_sub_diff_weight; // �����и�����ͬ�Ĳ��������ƶ��е�Ȩ��

	double m_poisim_all_output_th; // ���ƶȼ����������ֵ

	int m_useless_tel_threshold; // ��Ч�绰����Ƶ�ε���ֵ
	int m_thread_num; // ���д�����߳���

	PoiSimFunc m_base_inc_sim_func; // base��inc�������ƶȼ���Ĺ��򼯺Ϻ���
	PoiSimFunc m_inc_inc_sim_func; // inc��inc�������ƶȼ���Ĺ��򼯺Ϻ���

	vector<RuleNode> m_base_inc_rule_node_vec; // base��inc�������ƶȼ���Ĺ��򼯺�
	vector<RuleNode> m_inc_inc_rule_node_vec; // inc��inc�������ƶȼ���Ĺ��򼯺�

	vector<string> m_wordsplit_dic_vec; // �ִ���Ҫʹ�ôʱ��б�

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
// װ��resource��������
//=================================================
void loadResource(const PoiSimCfg *poi_sim_cfg, Resource *resource);
void statResource(const PreProcMap *pre_proc_map, Resource *resource);
void updateResourse(long poi_num, int threshold, Resource *resource);

//=================================================
// װ��tools��������
//=================================================
void loadTools(const PoiSimCfg *poi_sim_cfg, Tools *tools);

// ��Զ�
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
