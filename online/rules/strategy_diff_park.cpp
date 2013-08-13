#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffParkStrateyRule : public ResourceRule<DiffParkStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (startswith(sim_temp_cache->m_same_type_str, "1911")
    			|| sim_temp_cache->m_obj_name_set.find("停车库")
    					!= sim_temp_cache->m_obj_name_set.end()) {
    		int base_count = 0, inc_count = 0;
    		if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec, resource_,
    				3.0, base_count, false)
    				|| hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
    						resource_, 3.0, inc_count, false)) {
    			// 存在特别的非频繁词
    			return setResult("diff_park", -1.0, true, sim_temp_cache);
    		} else if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec,
    				resource_, 2.5, base_count, false)
    				|| hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
    						resource_, 2.5, inc_count, false)) {
    			// 存在可能的非频繁词 且 地址 电话不同
    			if (sim_temp_cache->m_same_tel_set.size() == 0
    					&& !isSameAddr(base_data, inc_data, sim_temp_cache)) {
    				return setResult("diff_park", -1.0, true, sim_temp_cache);
    			}
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_park", false));
        
    	return false;   
    }
};

REGINTER_RULE( DiffParkStratey )