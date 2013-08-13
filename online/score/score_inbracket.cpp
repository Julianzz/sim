#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class InBracketScoreRule : public ResourceRule<InBracketScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (isInclude(base_data->m_format_name, inc_data->m_raw_bracket_vec)
    			|| isInclude(inc_data->m_format_name,
    					base_data->m_raw_bracket_vec)) {
    		sim_temp_cache->m_in_bracket_score += -resource_->max_idf * 2;
    	}

    	if (sim_temp_cache->m_in_bracket_score > 0) {
    		sim_temp_cache->m_total_same_score +=
    				sim_temp_cache->m_in_bracket_score;
    	} else {
    		sim_temp_cache->m_total_diff_score +=
    				-sim_temp_cache->m_in_bracket_score;
    	}
    	return false;    
    }
};

REGINTER_RULE( InBracketScore )