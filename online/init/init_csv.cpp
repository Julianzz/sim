#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class CsvInitRule : public ResourceRule<CsvInitRule> {
    
private:
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
    
    virtual bool config(Json::Value& items){
        
    	this->m_id = 6;
    	this->m_name = 12;
    	this->m_addr = 13;
    	this->m_lat = 15;
    	this->m_lng = 16;
    	this->m_split_addr = 28;
    	this->m_addr_level = 29;
    	this->m_type = 18;
    	this->m_type_name = 17;
    	this->m_source = 4;
    	this->m_province = 8;
    	this->m_city = 9;
    	this->m_district = 10;
    	this->m_tel = 14;
    	this->m_status = 19;
    	this->m_max_index = 29;
        
    	this->m_thread_num = 1;
    	this->m_delim = "\t";
    	this->m_debug = false;
        
        return true;
    }
    
    virtual bool initRecord(const std::string& str_data, Record *record ) {
    	vector<string> vec_data;
    	split(str_data, this->m_delim, vec_data, "");
        
    	record->clear();
    	if (vec_data.size() < (size_t)(this->m_max_index + 1) ) {
    		record->m_is_null = true;
    		return false; 
    	}

    	if (this->m_id >= 0) {
    		record->m_id = strtoull(vec_data[this->m_id].c_str(), NULL, 0);
    	}

    	if (this->m_lat >= 0) {
    		record->m_lat = Str2Double(vec_data[this->m_lat]);
    	}

    	if (this->m_lng >= 0) {
    		record->m_lng = Str2Double(vec_data[this->m_lng]);
    	}

    	if (this->m_name >= 0) {
    		record->m_name = vec_data[this->m_name];
    	}

    	if (this->m_addr >= 0) {
    		record->m_addr = vec_data[this->m_addr];
    	}

    	if (this->m_split_addr >= 0) {
    		record->m_split_addr = vec_data[this->m_split_addr];
    	}

    	if (this->m_addr_level >= 0) {
    		record->m_addr_level = atoi(
    				vec_data[this->m_addr_level].c_str());
    	}

    	if (this->m_type >= 0) {
    		record->m_type = vec_data[this->m_type];
    	}

    	if (this->m_type_name >= 0) {
    		record->m_type_name = vec_data[this->m_type_name];
    	}

    	if (this->m_source >= 0) {
    		record->m_source = vec_data[this->m_source];
    	}

    	if (this->m_province >= 0) {
    		record->m_province = vec_data[this->m_province];
    	}

    	if (this->m_city >= 0) {
    		record->m_city = vec_data[this->m_city];
    	}

    	if (this->m_district >= 0) {
    		record->m_district = vec_data[this->m_district];
    	}

    	if (this->m_accurate_type >= 0) {
    		record->m_accurate_type = vec_data[this->m_accurate_type];
    	}

    	if (this->m_tel >= 0) {
    		record->m_tel = vec_data[this->m_tel];
    	}

    	if (this->m_status >= 0) {
    		if (vec_data[this->m_status] == "0"
    				|| vec_data[this->m_status] == "1"
    				|| vec_data[this->m_status] == "2") {
    			record->m_status = 0;
    		} else if (vec_data[this->m_status] == "16") {
    			record->m_status = 1;
    		} else {
    			record->m_status = 0;
    		}
    	}

    	record->m_is_null = false;
        
        return true;
        
    }
    
};

REGINTER_RULE( CsvInit )