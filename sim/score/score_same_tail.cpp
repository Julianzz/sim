#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class SameTailScoreRule : public ResourceRule<SameTailScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	if (endswith(base_data->m_format_name, inc_data->m_format_name)
    			|| endswith(inc_data->m_format_name, base_data->m_format_name)) {
    		if (startswith(sim_temp_cache->m_same_type_str, "22")) {
    			sim_temp_cache->m_tail_score += resource_->max_idf
    					* pow(2.71828,
    							-(pow(6.01828, sim_temp_cache->m_distance / 1480.0)
    									- 1));
    		} else if (startswith(sim_temp_cache->m_same_type_str, "23")
    				&& sim_temp_cache->m_distance <= 100) {
    			sim_temp_cache->m_tail_score += resource_->max_idf
    					* pow(10.71828,
    							-(pow(9.01828, sim_temp_cache->m_distance / 880.0)
    									- 1));
    		} else if (startswith(sim_temp_cache->m_same_type_str, "12")) {
    			sim_temp_cache->m_tail_score += resource_->max_idf
    					* pow(3.71828,
    							-(pow(3.51828, sim_temp_cache->m_distance / 2980.0)
    									- 1));
    		} else {
    			sim_temp_cache->m_tail_score += resource_->max_idf
    					* pow(10.71828,
    							-(pow(9.51828, sim_temp_cache->m_distance / 680.0)
    									- 1));
    		}
    	} else if (endswith(base_data->m_bigram_name, inc_data->m_bigram_name)
    			|| endswith(inc_data->m_bigram_name, base_data->m_bigram_name)) {
    		sim_temp_cache->m_tail_score +=
    				resource_->max_idf
    						* pow(3.71828,
    								-(pow(3.51828,
    										sim_temp_cache->m_distance / 2980.0) - 1));
    	}

    	if (sim_temp_cache->m_tail_score > 0) {
    		sim_temp_cache->m_total_same_score += sim_temp_cache->m_tail_score;
    	} else {
    		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_tail_score;
    	}
        return false;
    }
};

REGINTER_RULE( SameTailScore )