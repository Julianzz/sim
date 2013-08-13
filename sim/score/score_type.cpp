#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class TypeScoreRule : public ResourceRule<TypeScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (sim_temp_cache->m_same_type_str == "13") {
    		// 购物
    		if (base_data->m_sub_type != inc_data->m_sub_type) {
    			sim_temp_cache->m_type_score += -(pow(2.71828,
    					(sim_temp_cache->m_distance / 300.0)) - 1)
    					/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
    					* resource_->max_idf * 0.625;
    		}
    	} else if (sim_temp_cache->m_same_type_str == "16") {
    		// 娱乐
    		if (base_data->m_sub_type == "1699" || inc_data->m_sub_type == "1699") {
    			sim_temp_cache->m_type_score += 0.0;
    		} else if (base_data->m_sub_type != inc_data->m_sub_type) {
    			sim_temp_cache->m_type_score += -(pow(2.71828,
    					(sim_temp_cache->m_distance / 300.0)) - 1)
    					/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
    					* resource_->max_idf * 0.8;
    		}
    	} else if (sim_temp_cache->m_same_type_str == "26") {
    		// 地名地址
    		if (base_data->m_sub_type == "2699" || inc_data->m_sub_type == "2699") {
    			sim_temp_cache->m_type_score += 0.0;
    		} else if (base_data->m_sub_type == "2613"
    				^ inc_data->m_sub_type == "2613") {
    			sim_temp_cache->m_type_score += -resource_->max_idf * 1.5;
    		}
    	} else if (base_data->m_main_type == "11" ^ inc_data->m_main_type == "11") {
    		// 公司
    		if (sim_temp_cache->m_len_diff >= 10
    				&& ((base_data->m_main_type == "11"
    						&& inc_data->m_main_type == "12")
    						|| (base_data->m_main_type == "12"
    								&& inc_data->m_main_type == "11"))) {
    			sim_temp_cache->m_type_score += -resource_->max_idf;
    		}

    		sim_temp_cache->m_type_score += -(pow(2.71828,
    				(sim_temp_cache->m_distance / 300.0)) - 1)
    				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
    				* resource_->max_idf * 0.25;
    	} else if (base_data->m_soso_main_type != ""
    			&& inc_data->m_soso_main_type != ""
    			&& (base_data->m_soso_main_type == "11"
    					^ inc_data->m_soso_main_type == "11")) {
    		// 公司
    		sim_temp_cache->m_type_score += -(pow(2.71828,
    				(sim_temp_cache->m_distance / 300.0)) - 1)
    				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
    				* resource_->max_idf * 0.25;
    	} else if (base_data->m_main_type == "99"
    			|| inc_data->m_main_type == "99") {
    		// 其他
    		sim_temp_cache->m_type_score += -(pow(2.71828,
    				(sim_temp_cache->m_distance / 300.0)) - 1)
    				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
    				* resource_->max_idf * 0.25;
    	}

    	if (sim_temp_cache->m_type_score > 0) {
    		sim_temp_cache->m_total_same_score += sim_temp_cache->m_type_score;
    	} else {
    		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_type_score;
    	}
        return false;
    }
};

REGINTER_RULE( TypeScore )