#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffAccurateKeywordStrategyRule : public ResourceRule<DiffAccurateKeywordStrategyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	findDiffAccurateKeyword(sim_temp_cache->m_diff_words_base_vec,
    			base_data->m_accurate_kw_set, resource_,
    			sim_temp_cache->m_accurate_kw_base_vec);
    	findDiffAccurateKeyword(sim_temp_cache->m_diff_words_inc_vec,
    			inc_data->m_accurate_kw_set, resource_,
    			sim_temp_cache->m_accurate_kw_inc_vec);

    	if (sim_temp_cache->m_accurate_kw_base_vec.size() > 0
    			|| sim_temp_cache->m_accurate_kw_inc_vec.size() > 0) {
    		return setResult("diff_accurate_kw", -1.0, true, sim_temp_cache);
    	}

    	sim_temp_cache->m_rule_result_map.insert(
    			make_pair("diff_accurate_kw", false));
    	return false;  
    }
    
};

REGINTER_RULE( DiffAccurateKeywordStrategy )