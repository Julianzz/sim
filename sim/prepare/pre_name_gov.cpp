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


class NameGovPreRule : public ResourceRule<NameGovPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	/////////////////////////
    	// 补全 市旅游局 为 北京市旅游局
    	// 上海站  上海火车站(北京西站 有可能被替成北京西火车站，这个需要处理)
    	if (pre_proc_data->m_main_type != "12" && 
                pre_proc_data->m_main_type != "20" &&
                pre_proc_data->m_main_type != "24") {
            return true;
        }
        
    	if (startswith(pre_proc_data->m_format_name, "省")
    			&& pre_proc_data->m_province != "") {
                    
    		pre_proc_data->m_format_name = pre_proc_data->m_province
    				+ pre_proc_data->m_format_name.substr(2, pre_proc_data->m_format_name.size() - 2);
                            
    	} else if (startswith(pre_proc_data->m_format_name, "市")
    			&& pre_proc_data->m_city != "") {
    		pre_proc_data->m_format_name = pre_proc_data->m_city
    				+ pre_proc_data->m_format_name.substr(2, pre_proc_data->m_format_name.size() - 2);
            
    	} else if (startswith(pre_proc_data->m_format_name, "区")
    			&& pre_proc_data->m_district != ""
    			&& endswith(pre_proc_data->m_district, "区")) {
                    
    		        pre_proc_data->m_format_name = pre_proc_data->m_district
    				+ pre_proc_data->m_format_name.substr(2, pre_proc_data->m_format_name.size() - 2);
    	}
        
        return true;
    };
};

REGINTER_RULE( NameGovPre )