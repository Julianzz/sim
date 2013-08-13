#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class FenDianDealRule : public ResourceRule<FenDianDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	vector<string>::iterator it;
    	string temp_word;

    	if (inc_data->m_fendian != "") {
    		temp_word = inc_data->m_fendian;
    		it = sim_temp_cache->m_diff_words_base_vec.begin();
    		while (it != sim_temp_cache->m_diff_words_base_vec.end()) {
    			if (wordInclude(temp_word, *it, resource_->endList)) {
    				sim_temp_cache->m_sub_same_words_base_vec.push_back(*it);
    				it = sim_temp_cache->m_diff_words_base_vec.erase(it);
    			} else {
    				++it;
    			}
    		}
    	}

    	if (base_data->m_fendian != "") {
    		temp_word = base_data->m_fendian;
    		it = sim_temp_cache->m_diff_words_inc_vec.begin();
    		while (it != sim_temp_cache->m_diff_words_inc_vec.end()) {
    			if (wordInclude(temp_word, *it, resource_->endList)) {
    				sim_temp_cache->m_sub_same_words_inc_vec.push_back(*it);
    				it = sim_temp_cache->m_diff_words_inc_vec.erase(it);
    			} else {
    				++it;
    			}
    		}
    	}
    	return false;    
    }
};

REGINTER_RULE( FenDianDeal )