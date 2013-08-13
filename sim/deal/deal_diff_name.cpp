#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffNameDealRule : public ResourceRule<DiffNameDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	//both in base and inc
    	set_intersection(base_data->m_name_set.begin(),
                base_data->m_name_set.end(),
    			inc_data->m_name_set.begin(), 
                inc_data->m_name_set.end(),
    			insert_iterator<_vector>(sim_temp_cache->m_same_words_vec,
    					sim_temp_cache->m_same_words_vec.begin()));

    	//only in base
    	set_difference(base_data->m_name_set.begin(), 
                base_data->m_name_set.end(),
    			inc_data->m_name_set.begin(), 
                inc_data->m_name_set.end(),
    			insert_iterator<_vector>(sim_temp_cache->m_diff_words_base_vec,
    					sim_temp_cache->m_diff_words_base_vec.begin()));

    	//only in inc
    	set_difference(inc_data->m_name_set.begin(), 
                inc_data->m_name_set.end(),
    			base_data->m_name_set.begin(),
                base_data->m_name_set.end(),
    			insert_iterator<_vector>(sim_temp_cache->m_diff_words_inc_vec,
    					sim_temp_cache->m_diff_words_inc_vec.begin()));
        return false;
    }
};

REGINTER_RULE( DiffNameDeal )