#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

#include "utils/computeAddressSimilarity.h"
#include "utils/classify_data/classify_data.h"
#include "utils/classify_data/string_process.h"


class InitPreRule : public ResourceRule<InitPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	pre_proc_data->clear();
    	if (record->m_is_null) {
    		return false;
    	}
        
    	// 保存原始信息到预处理结果中
    	pre_proc_data->m_id = record->m_id;
    	pre_proc_data->m_lat = record->m_lat;
    	pre_proc_data->m_lng = record->m_lng;
    	pre_proc_data->m_name = record->m_name;
    	pre_proc_data->m_addr = record->m_addr;
    	pre_proc_data->m_split_addr = record->m_split_addr;
    	pre_proc_data->m_addr_level = record->m_addr_level;
    	pre_proc_data->m_type = record->m_type;
    	pre_proc_data->m_source = record->m_source;
    	pre_proc_data->m_province = record->m_province;
    	pre_proc_data->m_city = record->m_city;
    	pre_proc_data->m_district = record->m_district;
    	pre_proc_data->m_status = record->m_status;

    	          
        return true;
    };
};

REGINTER_RULE( InitPre )