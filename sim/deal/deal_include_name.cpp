#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class IncludeNameDealRule : public ResourceRule<IncludeNameDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_format_name.find(inc_data->m_format_name) != string::npos
    			|| inc_data->m_format_name.find(base_data->m_format_name)
    					!= string::npos) {
    		sim_temp_cache->m_name_is_include = true;
    	} 
        return false;
    }
};

REGINTER_RULE( IncludeNameDeal )