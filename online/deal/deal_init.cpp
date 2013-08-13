#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class InitDealRule : public ResourceRule<InitDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	sim_temp_cache->clear();
    	sim_temp_cache->m_base_id = base_data->m_id;
    	sim_temp_cache->m_inc_id = inc_data->m_id;
    	sim_temp_cache->m_base_name = base_data->m_name;
    	sim_temp_cache->m_inc_name = inc_data->m_name;
    	sim_temp_cache->m_status = base_data->m_status;
    	//sim_temp_cache->m_is_base = is_base;

        return false;
    }
};

REGINTER_RULE( InitDeal )