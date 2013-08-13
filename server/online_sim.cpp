#include "online_sim.h"
#include "commontype.h"

OnlineSim* OnlineSim::online_sim_ = NULL ;

OnlineSim* OnlineSim::instance() {
    if( online_sim_ == NULL ) {
        online_sim_ = new OnlineSim();
    }
    return online_sim_;
}

/*
int OnlineSim::initPreProc(const string &data, PreProcData *pre_proc_data, Record *record) {
	initRecordFromData(data, record );
    return initPreProc(record, pre_proc_data);
}

int OnlineSim::initPreProc(const vector<string> &data, PreProcData *pre_proc_data, Record *record ) {
	initRecordFromData(data, record );
    return initPreProc(record, pre_proc_data);
}
*/

int OnlineSim::initPreProc_(Record* record, PreProcData *pre_proc_data ) {
	makePreProcFromRec(record, pre_proc_data, resource_, tools_);
	if (isBadPreProcData(pre_proc_data)) {
        std::cerr<<"error in initPreProc "<<std::endl;
        return -1;
	}
    return 0;
}

void OnlineSim::initRecordFromData(const string &str_data, Record *record ) {
	vector<string> temp_vec;
	split(str_data, online_index_cfg_.m_delim, temp_vec, "");
	initRecordFromData(temp_vec, record );
}    


void OnlineSim::initRecordFromData(const Json::Value& data, Record *record ) {
    record->clear();   
    
    record->m_id  = json_convert<unsigned long long>(string("id"), data );
    record->m_lat = json_convert<double>("lat", data );
    record->m_lng = json_convert<double>("lng", data );
    record->m_name = json_convert<string>("name",data );
    record->m_addr = json_convert<string>("addr",data );
    
	if ( data.isMember("id") ) {
		record->m_id = strtoull(data["id"].asCString(), NULL, 0);
	}

	if ( data.isMember("lat") ) {
		record->m_lat = Str2Double(data["lat"].asString() );
	}

	if ( data.isMember("lng") ) {
		record->m_lng = Str2Double(data["lng"].asString() );
	}
    
	if ( data.isMember("name") ) {
        record->m_name = data["name"].asString();
	}

	if ( data.isMember("addr") ) {
        record->m_addr = data["addr"].asString();
	}

	if ( data.isMember("split_addr") ) {
        record->m_split_addr = data["split_addr"].asString();
	}

	if ( data.isMember("addr_level") ) {
        record->m_addr_level = atoi( data["addr_level"].asCString() ) ;
	}
 
	if ( data.isMember("type") ) {
        record->m_type = data["type"].asString() ;
	}
     
	if ( data.isMember("type_name") ) {
        record->m_type_name =  data["type_name"].asString() ;
	}
	if ( data.isMember("source") ) {
        record->m_source =  data["source"].asString() ;
	}
    
	if ( data.isMember("province") ) {
        record->m_province =  data["province"].asString() ;
	}
    
	if ( data.isMember("city") ) {
        record->m_city =  data["city"].asString() ;
	}

	if ( data.isMember("district") ) {
        record->m_district =  data["district"].asString() ;
	}
	if ( data.isMember("accurate_type") ) {
        record->m_accurate_type =  data["accurate_type"].asString() ;
	}
 
	if ( data.isMember("tel") ) {
        record->m_tel =  data["tel"].asString() ;
	}
    
	if ( data.isMember("status") ) {
        string status = data["status"].asString();
		if (status == "0" ||  status == "1" || status == "2") {
			record->m_status = 0;
		} else if (status == "16") {
			record->m_status = 1;
		} else {
			record->m_status = 0;
		}
	}

	record->m_is_null = false;   
    
}
void OnlineSim::initRecordFromData(const vector<string> &vec_data, Record *record ) {
            
	record->clear();

	if (vec_data.size() < online_index_cfg_.m_max_index + 1) {
		record->m_is_null = true;
		return;
	}

	if (online_index_cfg_.m_id >= 0) {
		record->m_id = strtoull(vec_data[online_index_cfg_.m_id].c_str(), NULL, 0);
	}

	if (online_index_cfg_.m_lat >= 0) {
		record->m_lat = Str2Double(vec_data[online_index_cfg_.m_lat]);
	}

	if (online_index_cfg_.m_lng >= 0) {
		record->m_lng = Str2Double(vec_data[online_index_cfg_.m_lng]);
	}

	if (online_index_cfg_.m_name >= 0) {
		record->m_name = vec_data[online_index_cfg_.m_name];
	}

	if (online_index_cfg_.m_addr >= 0) {
		record->m_addr = vec_data[online_index_cfg_.m_addr];
	}

	if (online_index_cfg_.m_split_addr >= 0) {
		record->m_split_addr = vec_data[online_index_cfg_.m_split_addr];
	}

	if (online_index_cfg_.m_addr_level >= 0) {
		record->m_addr_level = atoi(
				vec_data[online_index_cfg_.m_addr_level].c_str());
	}

	if (online_index_cfg_.m_type >= 0) {
		record->m_type = vec_data[online_index_cfg_.m_type];
	}

	if (online_index_cfg_.m_type_name >= 0) {
		record->m_type_name = vec_data[online_index_cfg_.m_type_name];
	}

	if (online_index_cfg_.m_source >= 0) {
		record->m_source = vec_data[online_index_cfg_.m_source];
	}

	if (online_index_cfg_.m_province >= 0) {
		record->m_province = vec_data[online_index_cfg_.m_province];
	}

	if (online_index_cfg_.m_city >= 0) {
		record->m_city = vec_data[online_index_cfg_.m_city];
	}

	if (online_index_cfg_.m_district >= 0) {
		record->m_district = vec_data[online_index_cfg_.m_district];
	}

	if (online_index_cfg_.m_accurate_type >= 0) {
		record->m_accurate_type = vec_data[online_index_cfg_.m_accurate_type];
	}

	if (online_index_cfg_.m_tel >= 0) {
		record->m_tel = vec_data[online_index_cfg_.m_tel];
	}

	if (online_index_cfg_.m_status >= 0) {
		if (vec_data[online_index_cfg_.m_status] == "0"
				|| vec_data[online_index_cfg_.m_status] == "1"
				|| vec_data[online_index_cfg_.m_status] == "2") {
			record->m_status = 0;
		} else if (vec_data[online_index_cfg_.m_status] == "16") {
			record->m_status = 1;
		} else {
			record->m_status = 0;
		}
	}

	record->m_is_null = false;
}

int OnlineSim::init(const string &source_dir, const std::string& mode ) {
	
	registSetFP(set_fp_map_);
	map<string, SetFP>::iterator iter = set_fp_map_.find(mode);
    if (iter == set_fp_map_.end() ) {
		cout << "mode is err" << endl;
		return -1;
    }
    
    SetFP set_fp = iter->second;
    //======================================
	// 相似度计算节点
	//======================================
	RuleNode *rule_node = new RuleNode();
	set_fp.rule_node_fp(rule_node_vec_, rule_node);

	//======================================
	// 相似度计算配置
	//======================================
	poi_sim_cfg_ = new PoiSimCfg();
	set_fp.poi_sim_cfg_fp(poi_sim_cfg_, source_dir);

	poi_sim_cfg_->m_base_inc_rule_node_vec = rule_node_vec_;
	poi_sim_cfg_->m_inc_inc_rule_node_vec = rule_node_vec_;

	//===============================
	// resource 的初始化
    resource_ = new Resource();
	loadResource(poi_sim_cfg_, resource_);

	//===============================
	// tools 的初始化
	tools_ = new Tools();
	loadTools(poi_sim_cfg_, tools_);

	// leveldb 导出格式
	set_fp.base_csv_cfg_fp(online_index_cfg_);
    
    temp_cache_ = new SimTempCache();
    
    return 0;
}

void OnlineSim::outputResult(const SimTempCache& sim_temp_cache, bool is_base, Json::Value& values ) {
    values["ids"] = Json::Value(Json::arrayValue);
    //std::string id = StringUtility::ULongLong2Str(sim_temp_cache.m_base_id);
    //values["ids"].append(Json::Value();
    //values["ids"].append(Json::Value(sim_temp_cache.m_inc_id));
   
    values["total_score"] = Json::Value(sim_temp_cache.m_score);
    values["same_score"] = Json::Value(sim_temp_cache.m_total_same_score);
    values["diff_score"] = Json::Value(sim_temp_cache.m_total_diff_score);
    values["name_score"] = Json::Value(sim_temp_cache.m_name_score);
    values["addr_score"] = Json::Value(sim_temp_cache.m_addr_score);
    values["type_score"] = Json::Value(sim_temp_cache.m_type_score);
    values["tel_score"] = Json::Value(sim_temp_cache.m_tel_score);
    values["tail_score"] = Json::Value(sim_temp_cache.m_tail_score);
    values["spec_score"] = Json::Value(sim_temp_cache.m_spec_score);
    values["distance_score"] = Json::Value(sim_temp_cache.m_distance_score);
    values["same_num_score"] = Json::Value(sim_temp_cache.m_same_num_score);
    values["accurate_type_score"] = Json::Value(sim_temp_cache.m_accurate_type_score);
    values["area_sub_poi_score"] = Json::Value(sim_temp_cache.m_area_sub_poi_score);
    values["include_addr_tel_score"] = Json::Value(sim_temp_cache.m_include_addr_tel_score);      
    values["name_same_score"] = Json::Value(sim_temp_cache.m_name_same_score);
    values["name_sub_same_score"] = Json::Value(sim_temp_cache.m_name_sub_same_score);      
    values["name_diff_score"] = Json::Value(sim_temp_cache.m_name_diff_score);
    values["name_sub_diff_score"] = Json::Value(sim_temp_cache.m_name_sub_diff_score);      
   
       
       
    values["len_diff"] = Json::Value( sim_temp_cache.m_len_diff );
    values["distance"] = Json::Value(sim_temp_cache.m_distance);
   
    values["same_type"] = Json::Value(sim_temp_cache.m_same_type_str);
    values["add_sim"] = Json::Value(sim_temp_cache.m_addr_sim);
    values["tel_sim"] = Json::Value((Json::UInt)sim_temp_cache.m_same_tel_set.size());
   
    values["status"] = Json::Value(sim_temp_cache.m_status);
   
    outMap(sim_temp_cache.m_rule_result_map, values["rule_result_map"]);

}

int OnlineSim::run_(PreProcData& firstRecord,PreProcData& secondRecord,Json::Value& result ) {
    bool isBase = true;
    SimTempCache simTemCache;
    ::poiSim(&firstRecord,&secondRecord, resource_, poi_sim_cfg_, &rule_node_vec_,&simTemCache,isBase );
    Json::Value sim;
	outputResult(simTemCache, isBase, result );
    
    //::print(&simTemCache);
    return 0;
    
}

