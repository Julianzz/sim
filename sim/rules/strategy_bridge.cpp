#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class BridgeStrateyRule : public ResourceRule<BridgeStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
        if ((endswith(base_data->m_format_name, "ге")
        		&& base_data->m_main_type == "26")
        		^ (endswith(inc_data->m_format_name, "ге")
        				&& inc_data->m_main_type == "26")) {
        	return setResult("bridge", -1.0, true, sim_temp_cache);
        }

        sim_temp_cache->m_rule_result_map.insert(make_pair("bridge", false));
        return false;
    }
};

REGINTER_RULE( BridgeStratey )