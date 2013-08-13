#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class SameIDStrateyRule : public ResourceRule<SameIDStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	if (base_data->m_id == inc_data->m_id) {
    		return setResult("same_id", 1.0, true, sim_temp_cache);
    	}
    	sim_temp_cache->m_rule_result_map.insert(make_pair("same_id", false));
    	return false;    
    }
};

REGINTER_RULE( SameIDStratey )