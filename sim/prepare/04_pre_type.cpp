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


class TypePreRule : public ResourceRule<TypePreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	// 获取精细分类
    	if (pre_proc_data->m_accurate_keyword != "") {
    		std::vector<std::string> temp_accurate_type_vec;
    		if (split(pre_proc_data->m_accurate_keyword, "#",
    				temp_accurate_type_vec, "") == -1) {
    			return true;
    		}

    		if (temp_accurate_type_vec.size() == 4) {
    			pre_proc_data->m_accurate_type = temp_accurate_type_vec[1];
    			pre_proc_data->m_accurate_keyword = temp_accurate_type_vec[3];

    			if (pre_proc_data->m_accurate_keyword != "") {
    				// 同义词替换
    				_map::const_iterator transIt = resource_->transWord.find(
    						pre_proc_data->m_accurate_keyword);
    				if (transIt != resource_->transWord.end()) {
    					pre_proc_data->m_accurate_keyword = transIt->second;
    				}
    			}
    		}
    	}

    	// 获取大中小类
    	if (pre_proc_data->m_type.size() == 6) {
    		pre_proc_data->m_main_type = pre_proc_data->m_type.substr(0, 2);
    		pre_proc_data->m_sub_type = pre_proc_data->m_type.substr(0, 4);
    	}

    	if (pre_proc_data->m_accurate_type.size() == 6) {
    		pre_proc_data->m_accurate_main_type =
    				pre_proc_data->m_accurate_type.substr(0, 2);
    		pre_proc_data->m_accurate_sub_type =
    				pre_proc_data->m_accurate_type.substr(0, 4);
    	}
        
        return true;
    };
};

REGINTER_RULE( TypePre )