#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class AtmStrateyRule: public ResourceRule<AtmStrateyRule> {

public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (  (base_data->m_bigram_name.find("atm") != std::string::npos
    		   ^ inc_data->m_bigram_name.find("atm") != std::string::npos)
    		|| ( base_data->m_num_name_set.find("atm") != base_data->m_num_name_set.end()
    			^ inc_data->m_num_name_set.find("atm") != inc_data->m_num_name_set.end() )
        ) {
    		return setResult("atm", -1.0, true, sim_temp_cache);
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("atm", false));
    	return false;   
    }
      
};

REGINTER_RULE( AtmStratey )