#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class SameATScoreRule : public ResourceRule<SameATScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	if (sim_temp_cache->m_name_is_include) {
    		// soso类型冲突，认为有风险
    		if (isConflict(base_data, inc_data, resource_)) {
    			return false;
    		}

    		// 公司和研究机构聚合有风险
    		if (startswith(sim_temp_cache->m_same_type_str, "11")
    				|| startswith(sim_temp_cache->m_same_type_str, "12")) {
    			return false;
    		} else if (sim_temp_cache->m_same_type_str == "") {
    			set<string>::iterator it = sim_temp_cache->m_same_type_set.begin();
    			for (; it != sim_temp_cache->m_same_type_set.end(); ++it) {
    				if (startswith(*it, "11") || startswith(*it, "12")) {
    					return false;
    				}
    			}
    		}

    		// 类型词，虽然有包含关系，地址不是高精度相同，认为聚合有风险
    		if (resource_->typeNameMap.find(base_data->m_format_name)
    				!= resource_->typeNameMap.end()
    				|| resource_->typeNameMap.find(inc_data->m_format_name)
    						!= resource_->typeNameMap.end()) {
    			if (sim_temp_cache->m_addr_sim < 0.52) {
    				sim_temp_cache->m_include_addr_tel_score -= resource_->max_idf
    						* 2;
    			}
    		}

    		if (sim_temp_cache->m_addr_sim >= 0.52) {
    			sim_temp_cache->m_include_addr_tel_score += resource_->max_idf * 2;
    		}

    		if (sim_temp_cache->m_same_tel_set.size() > 0) {
    			sim_temp_cache->m_include_addr_tel_score += resource_->max_idf;
    		}
    	}

    	if (sim_temp_cache->m_include_addr_tel_score > 0) {
    		sim_temp_cache->m_total_same_score +=
    				sim_temp_cache->m_include_addr_tel_score;
    	} else {
    		sim_temp_cache->m_total_diff_score +=
    				-sim_temp_cache->m_include_addr_tel_score;
    	}
    	return false;    
    }
};

REGINTER_RULE( SameATScore )