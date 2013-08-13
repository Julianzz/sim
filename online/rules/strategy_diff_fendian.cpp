#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffFendianStrateyRule : public ResourceRule<DiffFendianStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (isSameFenDian(base_data, inc_data, sim_temp_cache) < 0) {
    		if (((sim_temp_cache->m_addr_sim <= 0.52
    				&& (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3))
    				|| (base_data->m_addr_level >= 3 || inc_data->m_addr_level >= 3))
    				&& (sim_temp_cache->m_distance >= 50
    						&& sim_temp_cache->m_same_tel_set.size() == 0)) {
    			return setResult("diff_fendian", -1.0, true, sim_temp_cache);
    		}
    	} else if (startswith(sim_temp_cache->m_same_type_str, "2410")) {
    		// 北京大学（***）   北京大学（***）  可能是一个，这类不做处理
    	} else if (sim_temp_cache->m_bracket_score < 0.0) {
    		string base_tail =
    				base_data->m_bracket_vec[base_data->m_bracket_vec.size() - 1];
    		string inc_tail = inc_data->m_bracket_vec[inc_data->m_bracket_vec.size()
    				- 1];
    		if (base_tail != inc_tail) {
    			if (!isSameAddr(base_data, inc_data, sim_temp_cache)
    					&& (sim_temp_cache->m_distance >= 100
    							&& sim_temp_cache->m_same_tel_set.size() == 0)) {
    				return setResult("diff_fendian", -1.0, true, sim_temp_cache);
    			}
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_dizhi", false));
    	return false;
    }
};

REGINTER_RULE( DiffFendianStratey )