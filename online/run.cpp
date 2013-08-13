/*
 * test_main.cpp
 *
 *  Created on: 2012-10-22
 *      Author: stefanyang
 */

#include "settings.h"
#include "process_rule.h"
#include "process_main.h"
#include "process_input.h"
     
using namespace std;

void onlineSim(const string &base_path,
		const string &inc_path, const string &source_dir, SetFP &set_fp) {
	//======================================
	// 相似度计算节点
	//======================================
	vector<RuleNode> rule_node_vec;
	RuleNode *rule_node = new RuleNode();
	set_fp.rule_node_fp(rule_node_vec, rule_node);

	//======================================
	// 相似度计算配置
	//======================================
	PoiSimCfg *poi_sim_cfg = new PoiSimCfg();
	set_fp.poi_sim_cfg_fp(poi_sim_cfg, source_dir);

	poi_sim_cfg->m_base_inc_rule_node_vec = rule_node_vec;
	poi_sim_cfg->m_inc_inc_rule_node_vec = rule_node_vec;

	//===============================
	// resource 的初始化
	Resource *resource = new Resource();
	loadResource(poi_sim_cfg, resource);

	//===============================
	// tools 的初始化
	Tools *tools = new Tools();
	loadTools(poi_sim_cfg, tools);

	// leveldb 导出格式
	CsvIndexCfg base_csv_index_cfg, inc_csv_index_cfg;

	set_fp.base_csv_cfg_fp(base_csv_index_cfg);
	set_fp.inc_csv_cfg_fp(inc_csv_index_cfg);

	//===============================
	// 待处理文件预处理
	PreProcMap *base_map = new PreProcMap();
	PreProcMap *inc_map = new PreProcMap();

	initMapFromCsvFile(base_path.c_str(), base_map, resource, tools,
			&base_csv_index_cfg);
	initMapFromCsvFile(inc_path.c_str(), inc_map, resource, tools,
			&inc_csv_index_cfg);

	cout << "base data:" << base_map->size() << endl;
	cout << "inc data:" << inc_map->size() << endl;

	//======================================
	// 相似度计算
	//======================================
	poiSim(base_map, inc_map, resource, poi_sim_cfg);
}

void usage(){
	cout << "<=== two usage ===>" << endl;
	cout << "./poi_sim mode base_path inc_path	[source dir will default as ../../source/]" << endl;
	cout << "./poi_sim mode base_path inc_path source_dir" << endl;
	cout << "===================" << endl;
	cout << "mode has two values: [ online | self | debug | chn ]" << endl;
}

int main(int argc, char** argv) {
        //process.dump();
    
	if (argc == 3 || argc == 4 ) {
		string base_path = string(argv[1]);
		string inc_path = string(argv[2]);
		string mode = "online";
		if(argc == 4){
			mode = string(argv[1]);
			base_path = string(argv[2]);
			inc_path = string(argv[3]);
		}
        
        string configFile = std::string("./config/config.json");
        OnlineProcess process( configFile );
        process.init();        
        process.batchProcess(mode,base_path,inc_path );

	} else {
		usage();
        
        std::cout<<"+++++++++++++++++++++++++++++++++"<<std::endl;
        std::cout<<"+++++++++++++++++++++++++++++++++"<<std::endl;
        std::cout<<"just for test==========="<<std::endl;
        OnlineProcess process(std::string("./config/config.json"));
        process.init();
        std::string strategy("online");
        std::string base("../bin/online/base");
        std::string inc("../bin/online/base");
        
        process.batchProcess(strategy,base,inc );
        

	}
    
    

	return 0;
}

