#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class InBracketDealRule : public ResourceRule<InBracketDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	// 骐达公司苏州街店  骐达公司（苏州街）
    	filteIfNotNull(sim_temp_cache->m_diff_words_base_vec,
    			sim_temp_cache->m_sub_same_words_base_vec, 
                inc_data->m_bracket_set);

    	filteIfNotNull(sim_temp_cache->m_diff_words_inc_vec,
    			sim_temp_cache->m_sub_same_words_inc_vec, 
                base_data->m_bracket_set);

    	// use pinyin to filte word that find in other poi's brackets
    	filteIfNotNull(sim_temp_cache->m_diff_words_base_vec,
    			sim_temp_cache->m_sub_same_words_base_vec, 
                inc_data->m_bracket_vec,
    			resource_->pinyin);
    	filteIfNotNull(sim_temp_cache->m_diff_words_inc_vec,
    			sim_temp_cache->m_sub_same_words_inc_vec,
                base_data->m_bracket_vec,
    			resource_->pinyin); 
        return false;
    }
};

REGINTER_RULE( InBracketDeal )