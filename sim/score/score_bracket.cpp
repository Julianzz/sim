#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class BracketScoreRule : public ResourceRule<BracketScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_bracket_set.size() > 0
    			&& inc_data->m_bracket_set.size() > 0) {
                    
    		getSetSim(base_data->m_bracket_set, 
                    inc_data->m_bracket_set,
    				sim_temp_cache->m_bracket_score,
    				sim_temp_cache->m_bracket_same_words_vec,
    				sim_temp_cache->m_bracket_diff_words_base_vec,
    				sim_temp_cache->m_bracket_diff_words_inc_vec, resource_);
    	} else {
    		sim_temp_cache->m_bracket_score = 0.0;
    	}   
        return false;
    }
};

REGINTER_RULE( BracketScore )