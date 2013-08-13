#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class SameTypeDealRule : public ResourceRule<SameTypeDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	findType(base_data, inc_data, resource_->typeMap, sim_temp_cache);

    	if (sim_temp_cache->m_same_type_set.size() == 1) {
    		sim_temp_cache->m_same_type_str =
    				*(sim_temp_cache->m_same_type_set.begin());
    	}
        return false;
    }
};

REGINTER_RULE( SameTypeDeal )