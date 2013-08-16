#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffDizhiStrateyRule : public ResourceRule<DiffDizhiStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (startswith(sim_temp_cache->m_same_type_str, "28")) {
    		if (base_data->m_dizhi != "" && inc_data->m_dizhi != ""
    				&& base_data->m_dizhi != inc_data->m_dizhi) {
    			return setResult("diff_dizhi", -1.0, true, sim_temp_cache);
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_dizhi", false));
    	return false;

    }
};

REGINTER_RULE( DiffDizhiStratey )