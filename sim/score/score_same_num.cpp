#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class SameNumScoreRule : public ResourceRule<SameNumScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	if (base_data->m_num_name_set.size() > 0) {
    		sim_temp_cache->m_same_num_score += pow(2.71828,
    				(sim_temp_cache->m_distance / 300.0))
    				/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
    				* resource_->max_idf * 2;
    	}

    	if (sim_temp_cache->m_same_num_score > 0) {
    		sim_temp_cache->m_total_same_score += sim_temp_cache->m_same_num_score;
    	} else {
    		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_same_num_score;
    	}
    	return false;    
    }
};

REGINTER_RULE( SameNumScore )