#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class AddressScoreRule : public ResourceRule<AddressScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	if (sim_temp_cache->m_is_mode_chn) {
    		return false;
    	}

    	double threshold = 0.13;

    	if (sim_temp_cache->m_distance > 300
    			&& sim_temp_cache->m_addr_sim < threshold) {
    		sim_temp_cache->m_addr_score += -(pow(2.71828,
    				(sim_temp_cache->m_distance / 300.0)) - 1)
    				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
    				* resource_->max_idf * 4.5;
    	} else if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3) {
    		if (sim_temp_cache->m_obj_name_set.size() == 0) {
    			// 非连锁店情况
    			if (sim_temp_cache->m_addr_sim > threshold) {
    				// 地址相似
    				sim_temp_cache->m_addr_score += pow(2.71828,
    						(sim_temp_cache->m_distance / 300.0))
    						/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
    						* resource_->max_idf * 0.875;
    			} else if (sim_temp_cache->m_addr_sim == threshold) {
    				// 地址不确定
    				sim_temp_cache->m_addr_score += resource_->max_idf * 0.125;
    			} else if (startswith(sim_temp_cache->m_same_type_str, "11")
    					&& sim_temp_cache->m_addr_sim < 0.48) {
    				// 地址不相似
    				sim_temp_cache->m_addr_score += -(pow(2.71828,
    						(sim_temp_cache->m_distance / 300.0)) - 1)
    						/ (pow(2.71828, sim_temp_cache->m_distance / 300.0)
    								+ 5.21) * resource_->max_idf;
    			} else {
    				sim_temp_cache->m_addr_score += 0.0;
    			}
    		} else {
    			// 连锁店情况
    			if (sim_temp_cache->m_addr_sim < 0.0) {
    				sim_temp_cache->m_addr_score += 0.0;
    			} else if (sim_temp_cache->m_addr_sim < threshold) {
    				// 地址不相似
    				if (base_data->m_addr != "" && inc_data->m_addr != "") {
    					sim_temp_cache->m_addr_score += -(pow(2.71828,
    							(sim_temp_cache->m_distance / 300.0)) - 1)
    							/ (pow(2.71828, sim_temp_cache->m_distance / 300.0)
    									+ 5.21) * resource_->max_idf * 1.5;
    				}
    			} else if (sim_temp_cache->m_addr_sim == threshold) {
    				// 地址不确定
    				if (sim_temp_cache->m_distance
    						<= base_data->m_obj_max_distance) {
    					// 距离很近
    					sim_temp_cache->m_addr_score += pow(2.71828,
    							(sim_temp_cache->m_distance / 300.0))
    							/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
    							* resource_->max_idf * 0.875;
    				}
    			} else {
    				// 地址相似
    				sim_temp_cache->m_addr_score += pow(2.71828,
    						(sim_temp_cache->m_distance / 300.0))
    						/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
    						* resource_->max_idf * 1.5;
    			}
    		}
    	}

    	if (sim_temp_cache->m_addr_sim < threshold && base_data->m_addr_level < 3
    			&& inc_data->m_addr_level < 3) {
    		if (sim_temp_cache->m_distance > 300.0
    				&& (base_data->m_bracket_vec.size() > 0
    						^ inc_data->m_bracket_vec.size())) {
    			sim_temp_cache->m_addr_score += -resource_->max_idf * 1.5;
    		}
    	}

    	if (sim_temp_cache->m_obj_name_set.size() > 0
    			&& sim_temp_cache->m_spec_obj_name_set.size() == 0
    			&& sim_temp_cache->m_addr_sim >= threshold) {
    		sim_temp_cache->m_addr_score += resource_->max_idf * 1.5;
    	}

    	if (sim_temp_cache->m_addr_score > 0) {
    		sim_temp_cache->m_total_same_score += sim_temp_cache->m_addr_score;
    	} else {
    		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_addr_score;
    	}

    	return false;    
    }
};

REGINTER_RULE( AddressScore )