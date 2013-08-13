#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class SpecTypeScoreRule : public ResourceRule<SpecTypeScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	if (sim_temp_cache->m_type_score < 0.0) {
    		// 类型不一样的特别处理
    		if ((!startswith(sim_temp_cache->m_same_type_str, "11")
    				&& (base_data->m_main_type == "11"
    						|| inc_data->m_main_type == "11"
    						|| base_data->m_soso_main_type == "11"
    						|| inc_data->m_soso_main_type == "11"))
    				&& (!isSameAddr(base_data, inc_data, sim_temp_cache)
    						|| sim_temp_cache->m_distance > 500)
    				&& (sim_temp_cache->m_name_diff_score > 2.0
    						|| sim_temp_cache->m_name_sub_diff_score > 2.0)) {
    			sim_temp_cache->m_spec_score += -resource_->max_idf * 0.375;
    		} else if (sim_temp_cache->m_same_type_set.find("990000")
    				!= sim_temp_cache->m_same_type_set.end()
    				|| sim_temp_cache->m_same_type_str == "990000") {
    			// 有数据是 其他 类型的情况
    			if (sim_temp_cache->m_addr_sim < 0.52
    					|| sim_temp_cache->m_distance > 500) {
    				sim_temp_cache->m_spec_score += -resource_->max_idf;
    			}
    		}
    	} else if (sim_temp_cache->m_is_mode_self
    			&& startswith(sim_temp_cache->m_same_type_str, "11")) {
    		int base_count = 0, inc_count = 0;
    		bool base_has_nofreq = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_base_vec, resource_, 0.8,
    				base_count, false);
    		bool inc_has_nofreq = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_inc_vec, resource_, 0.8, inc_count,
    				false);
    		if ((base_has_nofreq || inc_has_nofreq)
    				&& sim_temp_cache->m_len_diff >= 8) {
    			sim_temp_cache->m_spec_score += -resource_->max_idf * 2;
    		}

    		if (sim_temp_cache->m_spec_score < 0) {
    			if (sim_temp_cache->m_addr_score > 0) {
    				sim_temp_cache->m_total_same_score +=
    						-sim_temp_cache->m_addr_score;
    				sim_temp_cache->m_addr_score = 0;
    			}

    			if (sim_temp_cache->m_tel_score > 0) {
    				sim_temp_cache->m_total_same_score +=
    						-sim_temp_cache->m_tel_score;
    				sim_temp_cache->m_addr_score = 0;
    			}

    		}
    	} else if (startswith(sim_temp_cache->m_same_type_str, "11")
    			&& isSameAddr(base_data, inc_data, sim_temp_cache)
    			&& (sim_temp_cache->m_name_same_score
    					+ sim_temp_cache->m_name_sub_same_score)
    					> resource_->max_idf * 0.875) {
    		sim_temp_cache->m_spec_score += -(pow(2.71828,
    				(sim_temp_cache->m_distance / 150.0)) - 1)
    				/ (pow(2.71828, sim_temp_cache->m_distance / 150.0) + 5.21)
    				* sim_temp_cache->m_name_sub_same_score * 0.625;
    	}

    	if (sim_temp_cache->m_spec_score > 0) {
    		sim_temp_cache->m_total_same_score += sim_temp_cache->m_spec_score;
    	} else {
    		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_spec_score;
    	} 
        return false; 
    }
};

REGINTER_RULE( SpecTypeScore )