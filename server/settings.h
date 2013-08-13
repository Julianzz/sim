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
void setMysqlCsvCfg(CsvIndexCfg &csv_index_cfg);
void setMysqlDebugCsvCfg(CsvIndexCfg &csv_index_cfg);

void setLDBCsvCfg(CsvIndexCfg &csv_index_cfg);

void setDebugCsvCfg(CsvIndexCfg &csv_index_cfg) ;


////////////////////////////////////////////////////////////////
// poi sim cfg
////////////////////////////////////////////////////////////////
void setOnlinePoiSimCfg(PoiSimCfg *poi_sim_cfg, const string &source_dir);
void setDebugPoiSimCfg(PoiSimCfg *poi_sim_cfg, const string &source_dir);

void setHerbertPoiSimCfg(PoiSimCfg *poi_sim_cfg, const string &source_dir);
void setDebugHerbertPoiSimCfg(PoiSimCfg *poi_sim_cfg, const string &source_dir);
void setSelfPoiSimCfg(PoiSimCfg *poi_sim_cfg, const string &source_dir);


////////////////////////////////////////////////////////////////
// rule node cfg
////////////////////////////////////////////////////////////////
void setOnlineRuleNode(vector<RuleNode> &rule_node_vec, RuleNode *rule_node);
void setSelfSimNode(vector<RuleNode> &rule_node_vec, RuleNode *rule_node);
void setChnRuleNode(vector<RuleNode> &rule_node_vec, RuleNode *rule_node) ;
void setCheckErrRuleNode(vector<RuleNode> &rule_node_vec, RuleNode *rule_node) ;

////////////////////////////////////////////////////////////////
// 注册所有的函数
////////////////////////////////////////////////////////////////
void registSetFP(map<string, SetFP> &set_fp_map);
#endif
