#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class ChainNameDealRule : public ResourceRule<ChainNameDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (sim_temp_cache->m_obj_name_set.size() > 0
    			&& sim_temp_cache->m_same_words_vec.size() > 1) {
    		vector<string>::iterator it = sim_temp_cache->m_same_words_vec.begin();

    		while (it != sim_temp_cache->m_same_words_vec.end()) {
    			if (sim_temp_cache->m_obj_name_set.find(*it)
    					!= sim_temp_cache->m_obj_name_set.end()) {
    				sim_temp_cache->m_sub_same_words_base_vec.push_back(*it);
    				sim_temp_cache->m_sub_same_words_inc_vec.push_back(*it);
    				it = sim_temp_cache->m_same_words_vec.erase(it);
    			} else {
    				++it;
    			}
    		}
    	}
        return false;
    }
};

REGINTER_RULE( ChainNameDeal )