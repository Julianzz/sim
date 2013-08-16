#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

static const char* params[] =  { "distance" };

class DiffDoorStrateyRule : public ResourceRule<DiffDoorStrateyRule> {
    
private:
	double m_distance;
    
public:
    
    virtual bool config(Json::Value& items){
        for( size_t i =0 ; i< sizeof(params)/sizeof(params[0]); i++ ) {
            if ( items[params[i]].isNull() ) {
                std::cout<< "DiffDoorStratey should provide:"<< params[i] <<std::endl;
                return false;
             }
        }
        m_distance = items["distance"].asDouble();
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_door_set.size() > 0 && inc_data->m_door_set.size() > 0
    			&& sim_temp_cache->m_distance <= m_distance) {
    		if (isSpecDoor(base_data->m_door_set, resource_)
    				&& isSpecDoor(inc_data->m_door_set, resource_)
    				&& (!checkInclude(base_data->m_door_set, inc_data->m_door_set,
    						inc_data->m_del_punc_name)
    						|| !checkInclude(inc_data->m_door_set,
    								base_data->m_door_set,
    								base_data->m_del_punc_name))) {
    			return setResult("diff_door", -1.0, true, sim_temp_cache);
    		}
    		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_door", false));
    		return false;
            
    	} else if ((!checkInclude(base_data->m_door_set, inc_data->m_door_set,
    			inc_data->m_del_punc_name)
    			|| !checkInclude(inc_data->m_door_set, base_data->m_door_set,
    					base_data->m_del_punc_name))
    			&& base_data->m_del_punc_name != inc_data->m_del_punc_name) {
    		return setResult("diff_door", -1.0, true, sim_temp_cache);
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_door", false));
    	return false;

    }
};

REGINTER_RULE( DiffDoorStratey )