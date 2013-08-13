#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class GovDistrictDealRule : public ResourceRule<GovDistrictDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {

    	if (sim_temp_cache->m_same_words_vec.size() < 2) {
    		return false ;
    	}
    	sim_temp_cache->m_sub_same_words_base_vec.clear();
    	sim_temp_cache->m_sub_same_words_inc_vec.clear();

    	_vectorit srcit;

    	string tempTail("");

    	srcit = sim_temp_cache->m_same_words_vec.begin();
    	while (srcit != sim_temp_cache->m_same_words_vec.end()) {
    		if ((*srcit).length() < 2) {
    			++srcit;
    			continue;
    		}

    		tempTail = (*srcit).substr((*srcit).length() - 2, 2);
    		if (resource_->govTailSet.find(tempTail) != resource_->govTailSet.end()) {
    			//sim_temp_cache->m_sub_same_words_base_vec.push_back(*srcit);
    			//sim_temp_cache->m_sub_same_words_inc_vec.push_back(*srcit);
    			srcit = sim_temp_cache->m_same_words_vec.erase(srcit);
    			if (sim_temp_cache->m_same_words_vec.size() < 2) {
    				return false ;
    			}
    		} else {
    			++srcit;
    		}
    	}

    	if (startswith(sim_temp_cache->m_same_type_str, "12")
    			&& sim_temp_cache->m_distance <= 150) {
    		srcit = sim_temp_cache->m_diff_words_base_vec.begin();
    		while (srcit != sim_temp_cache->m_diff_words_base_vec.end()) {
    			if ((*srcit).length() < 2) {
    				++srcit;
    				continue;
    			}

    			tempTail = (*srcit).substr((*srcit).length() - 2, 2);
    			if (resource_->govTailSet.find(tempTail)
    					!= resource_->govTailSet.end()) {
    				sim_temp_cache->m_sub_diff_words_base_vec.push_back(*srcit);
    				srcit = sim_temp_cache->m_diff_words_base_vec.erase(srcit);
    			} else {
    				++srcit;
    			}
    		}

    		srcit = sim_temp_cache->m_diff_words_inc_vec.begin();
    		while (srcit != sim_temp_cache->m_diff_words_inc_vec.end()) {
    			if ((*srcit).length() < 2) {
    				++srcit;
    				continue;
    			}

    			tempTail = (*srcit).substr((*srcit).length() - 2, 2);
    			if (resource_->govTailSet.find(tempTail)
    					!= resource_->govTailSet.end()) {
    				sim_temp_cache->m_sub_diff_words_inc_vec.push_back(*srcit);
    				srcit = sim_temp_cache->m_diff_words_inc_vec.erase(srcit);
    			} else {
    				++srcit;
    			}
    		}
    	}

    	return false;    
    }
};

REGINTER_RULE( GovDistrictDeal )