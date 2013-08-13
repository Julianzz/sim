#ifndef __ONLINE_INIT_H__
#define __ONLINE_INIT_H__

#include <sstream>
#include <map>
#include "json/json.h"
#include "settings.h"


using namespace std;

template < class K,class V> void outMap(const map<K, V> &data,Json::Value& result ) {
	typename map<K, V>::const_iterator it = data.begin();
	for (; it != data.end(); ++it) {
        result[ it->first ] = Json::Value(it->second );
	}
}

class OnlineSim {

public:
    OnlineSim():poi_sim_cfg_(NULL),resource_(NULL),
            tools_(NULL),temp_cache_(NULL) {} ;
    
    ~OnlineSim(){
        if ( tools_ != NULL) {
            delete tools_;
        }
        if( resource_ != NULL ) {
            delete resource_;
        }
        if( poi_sim_cfg_ != NULL ){
            delete poi_sim_cfg_;
        }
        if( temp_cache_ != NULL ) {
            delete temp_cache_;
        }
    }
    
    int init(const string &source_dir, const std::string& mode );
    
    template< class T > int initPreProc(const T& line, PreProcData *pre_proc_data, Record *record) ;
    template< class T > int run(const T& first,const T& second,Json::Value& result );
    
    void outputResult(const SimTempCache& sim_temp_cache, bool is_base, Json::Value& values );
private:
    int initPreProc_(Record* record, PreProcData *pre_proc_data );
    int run_(PreProcData& first,PreProcData& second,Json::Value& result );
private:
    //=================================================
    // 生成record
    void initRecordFromData(const string& str_data, Record* record ) ;
    void initRecordFromData(const vector<string>& vec_data, Record* record );
    void initRecordFromData(const Json::Value& data, Record* record ) ;
    
    void dumpRecord(Record& record,Json::Value& result ) {
        if( record.m_is_null ) {
            return ;
        }
        
        result["status"] = Json::Value(record.m_status);
        result["id"] = Json::Value(StringUtility::ULongLong2Str(record.m_id));
        result["lat"] = Json::Value(record.m_lat);
        result["lng"] = Json::Value(record.m_lng);
        result["name"] = Json::Value(record.m_name);
        result["addr"] = Json::Value(record.m_addr);
        result["type"] = Json::Value(record.m_type);    
        result["type_name"] = Json::Value(record.m_type_name);
        result["source"] = Json::Value(record.m_source);  
        result["province"] = Json::Value(record.m_province);
        result["city"] = Json::Value(record.m_city);  
        result["district"] = Json::Value(record.m_district);
        result["tel"] = Json::Value(record.m_tel);       
         
    }
    
public:
    static OnlineSim* instance();
    
private:
    static OnlineSim* online_sim_;
    
private:
    
    map<string, SetFP> set_fp_map_;
	vector<RuleNode> rule_node_vec_;
	PoiSimCfg *poi_sim_cfg_;
	// resource 的初始化
	Resource *resource_;
	Tools *tools_;
	// leveldb 导出格式
	CsvIndexCfg online_index_cfg_;
    SimTempCache* temp_cache_;
};

template< class T > int OnlineSim::initPreProc(const T& data, PreProcData *pre_proc_data, Record *record) {
	initRecordFromData(data, record );
    return initPreProc_( record, pre_proc_data );
}


template< class T> int OnlineSim::run(const T& firstStr,const T& secondStr, Json::Value& result ) {
    PreProcData firstRecord;
    PreProcData secondRecord;
    int ret1 =0 ,ret2=0;
    Record record1,record2;
    
    ret1 = initPreProc( firstStr, &firstRecord,&record1);
    ret2 = initPreProc( secondStr,&secondRecord,&record2);
    
    if(ret1 !=0 || ret2 != 0 ) {
        std::cout<<"error in init record :"<<std::endl;
        return -1;
    }
    return run_( firstRecord,secondRecord,result );
    
};


#endif
