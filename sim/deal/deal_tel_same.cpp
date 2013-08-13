#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class TelSameDealRule : public ResourceRule<TelSameDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_tel_set.size() > 0 && inc_data->m_tel_set.size() > 0) {
            
    		set<string>::const_iterator base_it, inc_it;

    		for (inc_it = inc_data->m_tel_set.begin();
    				inc_it != inc_data->m_tel_set.end(); ++inc_it) {
    			for (base_it = base_data->m_tel_set.begin();
    					base_it != base_data->m_tel_set.end(); ++base_it) {
    				if (tailIsSame(*base_it, *inc_it, 7)
    						&& !startswith(*inc_it, "400")
    						&& !startswith(*inc_it, "800")
    						&& resource_->uselessTelMap.find(*inc_it)
    								== resource_->uselessTelMap.end()) {
    					sim_temp_cache->m_same_tel_set.insert(*inc_it);
    				}
    			}
    		}
    	}
        
    	return false;    
    }
};

REGINTER_RULE( TelSameDeal )