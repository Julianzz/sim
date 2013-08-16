#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class SameTypeWordDealRule : public ResourceRule<SameTypeWordDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	vector<string>::iterator it = sim_temp_cache->m_same_words_vec.begin();

    	_mapSet::const_iterator map_it = resource_->typeword.find("01");
    	if (map_it != resource_->typeword.end()) {
    		while (it != sim_temp_cache->m_same_words_vec.end()) {
    			if (map_it->second.find(*it) != map_it->second.end()
    					&& sim_temp_cache->m_same_words_vec.size() > 1) {
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

REGINTER_RULE( SameTypeWordDeal )