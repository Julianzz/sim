#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DistanceScoreRule : public ResourceRule<DistanceScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	if (sim_temp_cache->m_addr_score > 0 && sim_temp_cache->m_distance > 600) {
    		sim_temp_cache->m_distance_score += -(pow(2.71828,
    				(sim_temp_cache->m_distance / 300.0)) - 1)
    				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
    				* resource_->max_idf * 0.375;
    	}

    	if (sim_temp_cache->m_distance_score > 0) {
    		sim_temp_cache->m_total_same_score += sim_temp_cache->m_distance_score;
    	} else {
    		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_distance_score;
    	}
    	return false;    
    }
};

REGINTER_RULE( DistanceScore )