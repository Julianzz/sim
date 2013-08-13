#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class TypeWordDealRule : public ResourceRule<TypeWordDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	set<string>::iterator it;

    	// 相同部分
    	if (sim_temp_cache->m_same_words_vec.size() > 1) {
    		for (it = sim_temp_cache->m_same_type_set.begin();
    				it != sim_temp_cache->m_same_type_set.end(); ++it) {
    			_mapSet::const_iterator map_it = resource_->typeword.find(*it);
    			if (map_it != resource_->typeword.end()) {
    				vector<string>::iterator word_it =
    						sim_temp_cache->m_same_words_vec.begin();
    				while (word_it != sim_temp_cache->m_same_words_vec.end()) {
    					if (map_it->second.find(*word_it) != map_it->second.end()) {
    						// 是类型词
    						sim_temp_cache->m_sub_same_words_base_vec.push_back(
    								*word_it);
    						sim_temp_cache->m_sub_same_words_inc_vec.push_back(
    								*word_it);
    						word_it = sim_temp_cache->m_same_words_vec.erase(
    								word_it);
    					} else {
    						++word_it;
    					}
    				}
    			}
    		}
    	}

    	// 不同部分
    	for (it = sim_temp_cache->m_same_type_set.begin();
    			it != sim_temp_cache->m_same_type_set.end(); ++it) {
    		_mapSet::const_iterator map_it = resource_->typeword.find(*it);
    		if (map_it != resource_->typeword.end()) {
    			vector<string>::iterator word_it =
    					sim_temp_cache->m_diff_words_base_vec.begin();
    			while (word_it != sim_temp_cache->m_diff_words_base_vec.end()) {
    				if (map_it->second.find(*word_it) != map_it->second.end()) {
    					// 是类型词
    					sim_temp_cache->m_sub_diff_words_base_vec.push_back(
    							*word_it);
    					word_it = sim_temp_cache->m_diff_words_base_vec.erase(
    							word_it);
    				} else {
    					++word_it;
    				}
    			}
    		}
    	}

    	for (it = sim_temp_cache->m_same_type_set.begin();
    			it != sim_temp_cache->m_same_type_set.end(); ++it) {
    		_mapSet::const_iterator map_it = resource_->typeword.find(*it);
    		if (map_it != resource_->typeword.end()) {
    			vector<string>::iterator word_it =
    					sim_temp_cache->m_diff_words_inc_vec.begin();
    			while (word_it != sim_temp_cache->m_diff_words_inc_vec.end()) {
    				if (map_it->second.find(*word_it) != map_it->second.end()) {
    					// 是类型词
    					sim_temp_cache->m_sub_diff_words_inc_vec.push_back(
    							*word_it);
    					word_it = sim_temp_cache->m_diff_words_inc_vec.erase(
    							word_it);
    				} else {
    					++word_it;
    				}
    			}
    		}
    	}
        return false;
    }
};

REGINTER_RULE( TypeWordDeal )