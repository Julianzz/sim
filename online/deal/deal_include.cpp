#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class IncludeDealRule : public ResourceRule<IncludeDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
		    SimTempCache *sim_temp_cache) {
            
        	if (sim_temp_cache->m_same_type_str == "261400") {
        		return false;
        	} else if (startswith(sim_temp_cache->m_same_type_str, "2810")) {
        		return false;
        	} else if (startswith(sim_temp_cache->m_same_type_str, "2210")) {
        		return false;
        	}

        	filteInclude(sim_temp_cache->m_diff_words_base_vec,
        			sim_temp_cache->m_diff_words_inc_vec,
        			sim_temp_cache->m_sub_same_words_base_vec,
        			sim_temp_cache->m_sub_same_words_inc_vec, resource_->endList);

        	filteInclude(sim_temp_cache->m_diff_words_inc_vec,
        			sim_temp_cache->m_diff_words_base_vec,
        			sim_temp_cache->m_sub_same_words_inc_vec,
        			sim_temp_cache->m_sub_same_words_base_vec, resource_->endList);
                    
            return false;
    }
};

REGINTER_RULE( IncludeDeal )