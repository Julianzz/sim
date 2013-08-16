#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class TrainStrateyRule : public ResourceRule<TrainStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_type == "271016" && inc_data->m_type == "271016") {
    		string short_name = inc_data->m_format_name;
    		string long_name = base_data->m_format_name;
    		if (base_data->m_format_name.size() < inc_data->m_format_name.size()) {
    			short_name = base_data->m_format_name;
    			long_name = inc_data->m_format_name;
    		}

    		string temp_name = short_name + "վ";
    		if (temp_name == long_name) {

    			return setResult("same_train_name", 1.0, true, sim_temp_cache);
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(
    			make_pair("same_train_name", false));
    	return false;  
    }
};

REGINTER_RULE( TrainStratey )