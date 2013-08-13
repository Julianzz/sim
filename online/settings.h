#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include <unistd.h>
#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"
using namespace std;

////////////////////////////////////////////////////////////////
// 基本定义
////////////////////////////////////////////////////////////////
typedef void (*SetCsvCfgFP)(CsvIndexCfg &csv_index_cfg);
typedef void (*SetPoiSimCfgFP)(PoiSimCfg *poi_sim_cfg, const string &source_dir);
typedef void (*SetRuleNodeFP)(vector<RuleNode> &rule_node_vec, RuleNode *rule_node);

class SetFP {
public:
	SetCsvCfgFP base_csv_cfg_fp;
	SetCsvCfgFP inc_csv_cfg_fp;
	SetPoiSimCfgFP poi_sim_cfg_fp;
	SetRuleNodeFP rule_node_fp;
};


////////////////////////////////////////////////////////////////
// csv cfg
////////////////////////////////////////////////////////////////
void setMysqlCsvCfg(CsvIndexCfg &csv_index_cfg){
	//===============================
	// csv file cfg
	// 原pp格式
	csv_index_cfg.clear();
	csv_index_cfg.m_id = 10;
	csv_index_cfg.m_name = 27;
	csv_index_cfg.m_addr = 18;
	csv_index_cfg.m_lat = 21;
	csv_index_cfg.m_lng = 22;
	csv_index_cfg.m_split_addr = 19;
	csv_index_cfg.m_addr_level = 20;
	csv_index_cfg.m_type = 26;
	csv_index_cfg.m_source = 6;
	csv_index_cfg.m_province = 12;
	csv_index_cfg.m_city = 13;
	csv_index_cfg.m_district = 14;
	csv_index_cfg.m_tel = 35;
	csv_index_cfg.m_max_index = 35;

	csv_index_cfg.m_thread_num = 1;
	csv_index_cfg.m_delim = "\t";
	csv_index_cfg.m_debug = false;
}

void setMysqlDebugCsvCfg(CsvIndexCfg &csv_index_cfg){
	//===============================
	// csv file cfg
	// 原pp格式
	setMysqlCsvCfg(csv_index_cfg);
	csv_index_cfg.m_debug = true;
}

void setLDBCsvCfg(CsvIndexCfg &csv_index_cfg){
	//===============================
	// csv file cfg
	csv_index_cfg.clear();
	csv_index_cfg.m_id = 6;
	csv_index_cfg.m_name = 12;
	csv_index_cfg.m_addr = 13;
	csv_index_cfg.m_lat = 15;
	csv_index_cfg.m_lng = 16;
	csv_index_cfg.m_split_addr = 28;
	csv_index_cfg.m_addr_level = 29;
	csv_index_cfg.m_type = 18;
	csv_index_cfg.m_type_name = 17;
	csv_index_cfg.m_source = 4;
	csv_index_cfg.m_province = 8;
	csv_index_cfg.m_city = 9;
	csv_index_cfg.m_district = 10;
	csv_index_cfg.m_tel = 14;
	csv_index_cfg.m_status = 19;
	csv_index_cfg.m_max_index = 29;


	csv_index_cfg.m_thread_num = 1;
	csv_index_cfg.m_delim = "\t";
	csv_index_cfg.m_debug = false;
}

void setDebugCsvCfg(CsvIndexCfg &csv_index_cfg){
	setLDBCsvCfg(csv_index_cfg);
	csv_index_cfg.m_debug = true;
}


////////////////////////////////////////////////////////////////
// poi sim cfg
////////////////////////////////////////////////////////////////
void setOnlinePoiSimCfg(PoiSimCfg *poi_sim_cfg, const string &source_dir){
	poi_sim_cfg->m_debug = false;
	poi_sim_cfg->m_find_in_self = true;
	poi_sim_cfg->m_del_same_id = true;

	poi_sim_cfg->m_out_base_sgl = true;
	poi_sim_cfg->m_out_inc_sgl = true;

	poi_sim_cfg->m_name_same_weight = 1.0;
	poi_sim_cfg->m_name_sub_same_weight = 0.175;
	poi_sim_cfg->m_name_diff_weight = 1.0;
	poi_sim_cfg->m_name_sub_diff_weight = 0.175;
	poi_sim_cfg->m_poisim_all_output_th = 0.0;
	poi_sim_cfg->m_useless_tel_threshold = 20;

	poi_sim_cfg->m_base_inc_sim_func = poiSimMax;
	poi_sim_cfg->m_inc_inc_sim_func = poiSimAll;

	poi_sim_cfg->m_thread_num = 4;

	poi_sim_cfg->m_use_local_idf = false;
	poi_sim_cfg->m_source_dir = source_dir;

	poi_sim_cfg->m_wordsplit_dic_vec.clear();
	poi_sim_cfg->m_wordsplit_dic_vec.push_back(
			source_dir + string("word.name.dic"));
	poi_sim_cfg->m_wordsplit_dic_vec.push_back(
			source_dir + string("word.soso.dic"));
	poi_sim_cfg->m_wordsplit_dic_vec.push_back(
			source_dir + string("word.sm.dic"));
	poi_sim_cfg->m_wordsplit_dic_vec.push_back(
			source_dir + string("word.xzqh.dic"));
	poi_sim_cfg->m_wordsplit_dic_vec.push_back(
			source_dir + string("word.roads.dic"));
	poi_sim_cfg->m_wordsplit_dic_vec.push_back(
			source_dir + string("word.number.dic"));
}

void setDebugPoiSimCfg(PoiSimCfg *poi_sim_cfg, const string &source_dir){
	setOnlinePoiSimCfg(poi_sim_cfg, source_dir);

	poi_sim_cfg->m_debug = true;
	poi_sim_cfg->m_thread_num = 1;
	poi_sim_cfg->m_use_local_idf = true;
}

void setHerbertPoiSimCfg(PoiSimCfg *poi_sim_cfg, const string &source_dir){
	setOnlinePoiSimCfg(poi_sim_cfg, source_dir);
	poi_sim_cfg->m_find_in_self = false;
}

void setDebugHerbertPoiSimCfg(PoiSimCfg *poi_sim_cfg, const string &source_dir){
	setHerbertPoiSimCfg(poi_sim_cfg, source_dir);
	poi_sim_cfg->m_debug = true;
	poi_sim_cfg->m_thread_num = 1;
	poi_sim_cfg->m_use_local_idf = true;
}

void setSelfPoiSimCfg(PoiSimCfg *poi_sim_cfg, const string &source_dir){
	setOnlinePoiSimCfg(poi_sim_cfg, source_dir);

	poi_sim_cfg->m_thread_num = 8;
}


////////////////////////////////////////////////////////////////
// rule node cfg
////////////////////////////////////////////////////////////////
void setOnlineRuleNode(vector<RuleNode> &rule_node_vec, RuleNode *rule_node){
	rule_node->clear();
	rule_node->fp = ruleSameID;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffGovLevel;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	RuleDistanceCfg *rule_distance_cfg = new RuleDistanceCfg();
	rule_distance_cfg->m_airport_recall_distance = 6000.0;
	rule_distance_cfg->m_airport_reliable_distance = 6000.0;
	rule_distance_cfg->m_train_recall_distance = 2000.0;
	rule_distance_cfg->m_train_reliable_distance = 500.0;
	rule_distance_cfg->m_default_recall_distance = 3000;
	rule_distance_cfg->m_default_reliable_distance = 1000;
	rule_distance_cfg->m_addr_sim = 0.52;
	rule_node->clear();
	rule_node->fp = ruleDistance;
	rule_node->cfg = rule_distance_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleSpecSourceCfg *rule_spec_src_cfg = new RuleSpecSourceCfg();
	rule_spec_src_cfg->m_source_name_vec = new vector<string>;
	rule_spec_src_cfg->m_source_name_vec->push_back("road_cross");
	rule_node->clear();
	rule_node->fp = ruleSpecSource;
	rule_node->cfg = rule_spec_src_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleRoadCfg *rule_road_cfg = new RuleRoadCfg();
	rule_road_cfg->m_reliable_distance = 400;
	rule_node->clear();
	rule_node->fp = ruleRoad;
	rule_node->cfg = rule_road_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleSameNameCfg *rule_same_name_cfg = new RuleSameNameCfg();
	rule_same_name_cfg->m_addr_sim = 0.52;
	rule_same_name_cfg->m_recall_distance = 3000;
	rule_same_name_cfg->m_reliable_distance = 1000;
	rule_node->clear();
	rule_node->fp = ruleSameName;
	rule_node->cfg = rule_same_name_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleDiffDoorCfg *rule_diff_door_cfg = new RuleDiffDoorCfg();
	rule_diff_door_cfg->m_distance = 5.0;
	rule_node->clear();
	rule_node->fp = ruleDiffDoor;
	rule_node->cfg = rule_diff_door_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleDiffDpmarkCfg *rule_diff_dpmark_cfg = new RuleDiffDpmarkCfg();
	rule_diff_dpmark_cfg->m_distance = 100.0;
	rule_diff_dpmark_cfg->m_addr_sim = 0.52;
	rule_node->clear();
	rule_node->fp = ruleDiffDpMark;
	rule_node->cfg = rule_diff_dpmark_cfg;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleAtm;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleAlias;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	RuleDiffGovTailCfg *rule_diff_govtail_cfg = new RuleDiffGovTailCfg();
	rule_diff_govtail_cfg->m_type_set = new set<string>;
	rule_diff_govtail_cfg->m_type_set->insert("26");
	rule_node->clear();
	rule_node->fp = ruleDiffGovTail;
	rule_node->cfg = rule_diff_govtail_cfg;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleTypeName;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleBridge;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleUnlackWord;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffNum;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffDizhi;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffFendian;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
	rule_node->clear();
	rule_node->fp = ruleDiffType;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
//	rule_node->clear();
//	rule_node->fp = ruleDiffSoSoType;
//	rule_node->cfg = NULL;
//	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
	rule_node->clear();
	rule_node->fp = ruleDiffBuildType;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleTrain;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleChain;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);
}


void setSelfSimNode(vector<RuleNode> &rule_node_vec, RuleNode *rule_node){
	rule_node->clear();
	rule_node->fp = ruleSameID;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffGovLevel;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	RuleDistanceCfg *rule_distance_cfg = new RuleDistanceCfg();
	rule_distance_cfg->m_airport_recall_distance = 6000.0;
	rule_distance_cfg->m_airport_reliable_distance = 6000.0;
	rule_distance_cfg->m_train_recall_distance = 2000.0;
	rule_distance_cfg->m_train_reliable_distance = 500.0;
	rule_distance_cfg->m_default_recall_distance = 1250;
	rule_distance_cfg->m_default_reliable_distance = 1000;
	rule_distance_cfg->m_addr_sim = 0.52;
	rule_node->clear();
	rule_node->fp = ruleDistance;
	rule_node->cfg = rule_distance_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleSpecSourceCfg *rule_spec_src_cfg = new RuleSpecSourceCfg();
	rule_spec_src_cfg->m_source_name_vec = new vector<string>;
	rule_spec_src_cfg->m_source_name_vec->push_back("road_cross");
	rule_node->clear();
	rule_node->fp = ruleSpecSource;
	rule_node->cfg = rule_spec_src_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleRoadCfg *rule_road_cfg = new RuleRoadCfg();
	rule_road_cfg->m_reliable_distance = 400;
	rule_node->clear();
	rule_node->fp = ruleRoad;
	rule_node->cfg = rule_road_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleDiffDoorCfg *rule_diff_door_cfg = new RuleDiffDoorCfg();
	rule_diff_door_cfg->m_distance = 5.0;
	rule_node->clear();
	rule_node->fp = ruleDiffDoor;
	rule_node->cfg = rule_diff_door_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleDiffDpmarkCfg *rule_diff_dpmark_cfg = new RuleDiffDpmarkCfg();
	rule_diff_dpmark_cfg->m_distance = 100.0;
	rule_diff_dpmark_cfg->m_addr_sim = 0.52;
	rule_node->clear();
	rule_node->fp = ruleDiffDpMark;
	rule_node->cfg = rule_diff_dpmark_cfg;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleAtm;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleAlias;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	RuleDiffGovTailCfg *rule_diff_govtail_cfg = new RuleDiffGovTailCfg();
	rule_diff_govtail_cfg->m_type_set = new set<string>;
	rule_diff_govtail_cfg->m_type_set->insert("26");
	rule_node->clear();
	rule_node->fp = ruleDiffGovTail;
	rule_node->cfg = rule_diff_govtail_cfg;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleTypeName;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleBridge;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleUnlackWord;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffNum;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffDizhi;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffFendian;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleSelfSim;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	RuleSameNameCfg *rule_same_name_cfg = new RuleSameNameCfg();
	rule_same_name_cfg->m_addr_sim = 0.52;
	rule_same_name_cfg->m_recall_distance = 1000;
	rule_same_name_cfg->m_reliable_distance = 800;
	rule_node->clear();
	rule_node->fp = ruleSameName;
	rule_node->cfg = rule_same_name_cfg;
	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
	rule_node->clear();
	rule_node->fp = ruleDiffType;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
//	rule_node->clear();
//	rule_node->fp = ruleDiffSoSoType;
//	rule_node->cfg = NULL;
//	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
	rule_node->clear();
	rule_node->fp = ruleDiffBuildType;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleTrain;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleChain;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);
}

void setChnRuleNode(vector<RuleNode> &rule_node_vec, RuleNode *rule_node){
	rule_node->clear();
	rule_node->fp = ruleSameID;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffGovLevel;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleChnSim;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);


	RuleDistanceCfg *rule_distance_cfg = new RuleDistanceCfg();
	rule_distance_cfg->m_airport_recall_distance = 6000.0;
	rule_distance_cfg->m_airport_reliable_distance = 6000.0;
	rule_distance_cfg->m_train_recall_distance = 2000.0;
	rule_distance_cfg->m_train_reliable_distance = 500.0;
	rule_distance_cfg->m_default_recall_distance = 3000;
	rule_distance_cfg->m_default_reliable_distance = 2000;
	rule_distance_cfg->m_addr_sim = 0.52;
	rule_node->clear();
	rule_node->fp = ruleDistance;
	rule_node->cfg = rule_distance_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleSpecSourceCfg *rule_spec_src_cfg = new RuleSpecSourceCfg();
	rule_spec_src_cfg->m_source_name_vec = new vector<string>;
	rule_spec_src_cfg->m_source_name_vec->push_back("road_cross");
	rule_node->clear();
	rule_node->fp = ruleSpecSource;
	rule_node->cfg = rule_spec_src_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleRoadCfg *rule_road_cfg = new RuleRoadCfg();
	rule_road_cfg->m_reliable_distance = 400;
	rule_node->clear();
	rule_node->fp = ruleRoad;
	rule_node->cfg = rule_road_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleSameNameCfg *rule_same_name_cfg = new RuleSameNameCfg();
	rule_same_name_cfg->m_addr_sim = 0.52;
	rule_same_name_cfg->m_recall_distance = 3000;
	rule_same_name_cfg->m_reliable_distance = 2000;
	rule_node->clear();
	rule_node->fp = ruleSameName;
	rule_node->cfg = rule_same_name_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleDiffDoorCfg *rule_diff_door_cfg = new RuleDiffDoorCfg();
	rule_diff_door_cfg->m_distance = 5.0;
	rule_node->clear();
	rule_node->fp = ruleDiffDoor;
	rule_node->cfg = rule_diff_door_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleDiffDpmarkCfg *rule_diff_dpmark_cfg = new RuleDiffDpmarkCfg();
	rule_diff_dpmark_cfg->m_distance = 100.0;
	rule_diff_dpmark_cfg->m_addr_sim = 0.52;
	rule_node->clear();
	rule_node->fp = ruleDiffDpMark;
	rule_node->cfg = rule_diff_dpmark_cfg;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleAtm;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleAlias;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	RuleDiffGovTailCfg *rule_diff_govtail_cfg = new RuleDiffGovTailCfg();
	rule_diff_govtail_cfg->m_type_set = new set<string>;
	rule_diff_govtail_cfg->m_type_set->insert("26");
	rule_node->clear();
	rule_node->fp = ruleDiffGovTail;
	rule_node->cfg = rule_diff_govtail_cfg;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleTypeName;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleBridge;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleUnlackWord;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffNum;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffDizhi;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffFendian;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
//	rule_node->clear();
//	rule_node->fp = ruleDiffType;
//	rule_node->cfg = NULL;
//	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
//	rule_node->clear();
//	rule_node->fp = ruleDiffSoSoType;
//	rule_node->cfg = NULL;
//	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
//	rule_node->clear();
//	rule_node->fp = ruleDiffBuildType;
//	rule_node->cfg = NULL;
//	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleTrain;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleChain;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);
}

void setCheckErrRuleNode(vector<RuleNode> &rule_node_vec, RuleNode *rule_node){
	rule_node->clear();
	rule_node->fp = ruleSameID;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffGovLevel;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	RuleSpecSourceCfg *rule_spec_src_cfg = new RuleSpecSourceCfg();
	rule_spec_src_cfg->m_source_name_vec = new vector<string>;
	rule_spec_src_cfg->m_source_name_vec->push_back("road_cross");
	rule_node->clear();
	rule_node->fp = ruleSpecSource;
	rule_node->cfg = rule_spec_src_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleRoadCfg *rule_road_cfg = new RuleRoadCfg();
	rule_road_cfg->m_reliable_distance = 2000;
	rule_node->clear();
	rule_node->fp = ruleRoad;
	rule_node->cfg = rule_road_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleSameNameCfg *rule_same_name_cfg = new RuleSameNameCfg();
	rule_same_name_cfg->m_addr_sim = 0.52;
	rule_same_name_cfg->m_recall_distance = 10000;
	rule_same_name_cfg->m_reliable_distance = 1000;
	rule_node->clear();
	rule_node->fp = ruleSameName;
	rule_node->cfg = rule_same_name_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleDiffDoorCfg *rule_diff_door_cfg = new RuleDiffDoorCfg();
	rule_diff_door_cfg->m_distance = 5.0;
	rule_node->clear();
	rule_node->fp = ruleDiffDoor;
	rule_node->cfg = rule_diff_door_cfg;
	rule_node_vec.push_back(*rule_node);

	RuleDiffDpmarkCfg *rule_diff_dpmark_cfg = new RuleDiffDpmarkCfg();
	rule_diff_dpmark_cfg->m_distance = 100.0;
	rule_diff_dpmark_cfg->m_addr_sim = 0.52;
	rule_node->clear();
	rule_node->fp = ruleDiffDpMark;
	rule_node->cfg = rule_diff_dpmark_cfg;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleAtm;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleAlias;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	RuleDiffGovTailCfg *rule_diff_govtail_cfg = new RuleDiffGovTailCfg();
	rule_diff_govtail_cfg->m_type_set = new set<string>;
	rule_diff_govtail_cfg->m_type_set->insert("26");
	rule_node->clear();
	rule_node->fp = ruleDiffGovTail;
	rule_node->cfg = rule_diff_govtail_cfg;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleTypeName;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleBridge;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleUnlackWord;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffNum;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffDizhi;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleDiffFendian;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
	rule_node->clear();
	rule_node->fp = ruleDiffType;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
//	rule_node->clear();
//	rule_node->fp = ruleDiffSoSoType;
//	rule_node->cfg = NULL;
//	rule_node_vec.push_back(*rule_node);

	// 必须在名称相同检查完之后
	rule_node->clear();
	rule_node->fp = ruleDiffBuildType;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleTrain;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);

	rule_node->clear();
	rule_node->fp = ruleChain;
	rule_node->cfg = NULL;
	rule_node_vec.push_back(*rule_node);
}

////////////////////////////////////////////////////////////////
// 注册所有的函数
////////////////////////////////////////////////////////////////
void registSetFP(map<string, SetFP> &set_fp_map){
	SetFP set_fp;

	set_fp.base_csv_cfg_fp = setLDBCsvCfg;
	set_fp.inc_csv_cfg_fp = setLDBCsvCfg;
	set_fp.poi_sim_cfg_fp = setOnlinePoiSimCfg;
	set_fp.rule_node_fp = setOnlineRuleNode;
	set_fp_map.insert(make_pair("online", set_fp));

	set_fp.base_csv_cfg_fp = setLDBCsvCfg;
	set_fp.inc_csv_cfg_fp = setLDBCsvCfg;
	set_fp.poi_sim_cfg_fp = setSelfPoiSimCfg;
	set_fp.rule_node_fp = setSelfSimNode;
	set_fp_map.insert(make_pair("self", set_fp));

	set_fp.base_csv_cfg_fp = setDebugCsvCfg;
	set_fp.inc_csv_cfg_fp = setDebugCsvCfg;
	set_fp.poi_sim_cfg_fp = setDebugPoiSimCfg;
	set_fp.rule_node_fp = setSelfSimNode;
	set_fp_map.insert(make_pair("self_debug", set_fp));

	set_fp.base_csv_cfg_fp = setDebugCsvCfg;
	set_fp.inc_csv_cfg_fp = setDebugCsvCfg;
	set_fp.poi_sim_cfg_fp = setDebugPoiSimCfg;
	set_fp.rule_node_fp = setOnlineRuleNode;
	set_fp_map.insert(make_pair("debug", set_fp));

	set_fp.base_csv_cfg_fp = setMysqlCsvCfg;
	set_fp.inc_csv_cfg_fp = setMysqlCsvCfg;
	set_fp.poi_sim_cfg_fp = setHerbertPoiSimCfg;
	set_fp.rule_node_fp = setOnlineRuleNode;
	set_fp_map.insert(make_pair("herbert", set_fp));

	set_fp.base_csv_cfg_fp = setMysqlCsvCfg;
	set_fp.inc_csv_cfg_fp = setMysqlCsvCfg;
	set_fp.poi_sim_cfg_fp = setHerbertPoiSimCfg;
	set_fp.rule_node_fp = setSelfSimNode;
	set_fp_map.insert(make_pair("herbert_base", set_fp));

	set_fp.base_csv_cfg_fp = setMysqlCsvCfg;
	set_fp.inc_csv_cfg_fp = setMysqlCsvCfg;
	set_fp.poi_sim_cfg_fp = setDebugHerbertPoiSimCfg;
	set_fp.rule_node_fp = setSelfSimNode;
	set_fp_map.insert(make_pair("herbert_base_debug", set_fp));

	set_fp.base_csv_cfg_fp = setMysqlDebugCsvCfg;
	set_fp.inc_csv_cfg_fp = setMysqlDebugCsvCfg;
	set_fp.poi_sim_cfg_fp = setDebugHerbertPoiSimCfg;
	set_fp.rule_node_fp = setOnlineRuleNode;
	set_fp_map.insert(make_pair("herbert_debug", set_fp));

	set_fp.base_csv_cfg_fp = setLDBCsvCfg;
	set_fp.inc_csv_cfg_fp = setLDBCsvCfg;
	set_fp.poi_sim_cfg_fp = setOnlinePoiSimCfg;
	set_fp.rule_node_fp = setChnRuleNode;
	set_fp_map.insert(make_pair("chn", set_fp));

	set_fp.base_csv_cfg_fp = setLDBCsvCfg;
	set_fp.inc_csv_cfg_fp = setLDBCsvCfg;
	set_fp.poi_sim_cfg_fp = setDebugPoiSimCfg;
	set_fp.rule_node_fp = setChnRuleNode;
	set_fp_map.insert(make_pair("chn_debug", set_fp));

	set_fp.base_csv_cfg_fp = setLDBCsvCfg;
	set_fp.inc_csv_cfg_fp = setLDBCsvCfg;
	set_fp.poi_sim_cfg_fp = setOnlinePoiSimCfg;
	set_fp.rule_node_fp = setCheckErrRuleNode;
	set_fp_map.insert(make_pair("check_err", set_fp));

	set_fp.base_csv_cfg_fp = setLDBCsvCfg;
	set_fp.inc_csv_cfg_fp = setLDBCsvCfg;
	set_fp.poi_sim_cfg_fp = setDebugPoiSimCfg;
	set_fp.rule_node_fp = setCheckErrRuleNode;
	set_fp_map.insert(make_pair("check_err_debug", set_fp));
}

#endif
