#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class OneSideRoadDealRule : public ResourceRule<OneSideRoadDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_main_type != "26" && inc_data->m_main_type != "26") {
    		if (sim_temp_cache->m_diff_words_base_vec.size() == 1
    				&& sim_temp_cache->m_diff_words_inc_vec.size() == 0) {
    			if (resource_->road.find(sim_temp_cache->m_diff_words_base_vec[0])
    					!= resource_->road.end()) {
    				sim_temp_cache->m_sub_same_words_base_vec.push_back(
    						sim_temp_cache->m_diff_words_base_vec[0]);
    				sim_temp_cache->m_diff_words_base_vec.erase(
    						sim_temp_cache->m_diff_words_base_vec.begin());
    			}
    		} else if (sim_temp_cache->m_diff_words_base_vec.size() == 0
    				&& sim_temp_cache->m_diff_words_inc_vec.size() == 1) {
    			if (resource_->road.find(sim_temp_cache->m_diff_words_inc_vec[0])
    					!= resource_->road.end()) {
    				sim_temp_cache->m_sub_same_words_inc_vec.push_back(
    						sim_temp_cache->m_diff_words_inc_vec[0]);
    				sim_temp_cache->m_diff_words_inc_vec.erase(
    						sim_temp_cache->m_diff_words_inc_vec.begin());
    			}
    		}
    	}
    	return false;    
    }
};

REGINTER_RULE( OneSideRoadDeal )