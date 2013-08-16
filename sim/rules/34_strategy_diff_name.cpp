#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffNameStrateyRule : public ResourceRule<DiffNameStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (sim_temp_cache->m_obj_name_set.size() == 0
    			|| !isSameAddr(base_data, inc_data, sim_temp_cache)) {
    		if (sim_temp_cache->m_name_score < 0) {
    			return setResult("diff_name", -1.0, true, sim_temp_cache);
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_name", false));
    	return false;  
    }
};

REGINTER_RULE( DiffNameStratey )