#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class PinYinDealRule : public ResourceRule<PinYinDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	if (startswith(sim_temp_cache->m_same_type_str, "2810")) {
    		return false;
    	}

    	pyVecIsSame(sim_temp_cache->m_diff_words_base_vec,
    			sim_temp_cache->m_diff_words_inc_vec,
    			sim_temp_cache->m_sub_same_words_base_vec,
    			sim_temp_cache->m_sub_same_words_inc_vec, resource_->pinyin, 0);

    	if (startswith(sim_temp_cache->m_same_type_str, "22")
    			|| startswith(sim_temp_cache->m_same_type_str, "23")) {
    		pyVecIsSame(sim_temp_cache->m_diff_words_base_vec,
    				sim_temp_cache->m_diff_words_inc_vec,
    				sim_temp_cache->m_sub_diff_words_base_vec,
    				sim_temp_cache->m_sub_diff_words_inc_vec, resource_->pinyin, 0);
    	} else {
    		pyVecIsSame(sim_temp_cache->m_diff_words_base_vec,
    				sim_temp_cache->m_diff_words_inc_vec,
    				sim_temp_cache->m_sub_diff_words_base_vec,
    				sim_temp_cache->m_sub_diff_words_inc_vec, resource_->pinyin, 1);
    	} 
        return false;
    }
};

REGINTER_RULE( PinYinDeal )