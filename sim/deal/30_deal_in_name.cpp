#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class InNameDealRule : public ResourceRule<InNameDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {

    	filter(base_data->m_format_name, 
                sim_temp_cache->m_diff_words_inc_vec,
    			sim_temp_cache->m_sub_same_words_inc_vec);
    	filter(inc_data->m_format_name, 
                sim_temp_cache->m_diff_words_base_vec,
    			sim_temp_cache->m_sub_same_words_base_vec);
                
    	return false;    
    }
};

REGINTER_RULE( InNameDeal )