#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class NotFreqStrateyRule : public ResourceRule<NotFreqStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (startswith(sim_temp_cache->m_same_type_str, "2810")) {
    		int base_count = 0, inc_count = 0;
    		bool base_has_nofreq = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_base_vec, resource_, 3.0,
    				base_count, false);
    		bool inc_has_nofreq = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_inc_vec, resource_, 3.0, inc_count,
    				false);
    		if (base_has_nofreq || inc_has_nofreq) {
    			return setResult("diff_not_freq", -1.0, true, sim_temp_cache);
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_not_freq", false));
    	return false;
    }
};

REGINTER_RULE( NotFreqStratey )