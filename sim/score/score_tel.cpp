#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class TelScoreRule : public ResourceRule<TelScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	if (sim_temp_cache->m_type_score < 0) {
    		return false;
    	}

    	if (sim_temp_cache->m_same_tel_set.size() > 0) {
    		sim_temp_cache->m_tel_score += resource_->max_idf * 2;
    	}

    	if (startswith(sim_temp_cache->m_same_type_str, "13")
    			&& sim_temp_cache->m_same_type_str != "131000") {
    		// 购物
    		sim_temp_cache->m_tel_score = sim_temp_cache->m_tel_score * 0.025;
    	} else if (startswith(sim_temp_cache->m_same_type_str, "12")
    			|| startswith(sim_temp_cache->m_same_type_str, "14")) {
    		// 机构或生活服务
    		sim_temp_cache->m_tel_score = sim_temp_cache->m_tel_score * 0.025;
    	} else if (startswith(sim_temp_cache->m_same_type_str, "11")) {
    		// 公司
    		sim_temp_cache->m_tel_score = sim_temp_cache->m_tel_score * 0.05;
    	}

    	if (sim_temp_cache->m_obj_name_set.size() > 0
    			&& sim_temp_cache->m_spec_obj_name_set.size() == 0
    			&& sim_temp_cache->m_addr_sim >= 0.52) {
    		sim_temp_cache->m_tel_score += resource_->max_idf * 1.0;
    	}

    	if (sim_temp_cache->m_tel_score > 0) {
    		sim_temp_cache->m_total_same_score += sim_temp_cache->m_tel_score;
    	} else {
    		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_tel_score;
    	} 
        
        return false; 
    }
};

REGINTER_RULE( TelScore )