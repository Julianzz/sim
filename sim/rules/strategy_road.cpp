#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

static const char* params[] =  { 
    "reliable_distance"
 };
 
class RoadStrateyRule : public ResourceRule<RoadStrateyRule> {
    
private:
	double m_reliable_distance;
    
public:
    
    virtual bool config(Json::Value& items){
        for( size_t i =0 ; i< sizeof(params)/sizeof(params[0]); i++ ) {
            if ( items[params[i]].isNull() ) {
                std::cout<< "DiffDoorStratey should provide:"<< params[i] <<std::endl;
                return false;
             }
        }
        
    	m_reliable_distance = items["reliable_distance"].asDouble();
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_type == "261200"
    			&& base_data->m_type == inc_data->m_type) {
                    
    		if (nameIsSame(base_data, inc_data, resource_)
    				&& sim_temp_cache->m_distance
    						<= m_reliable_distance) {
    			return setResult("road", 1.0, true, sim_temp_cache);
    		} else {
    			return setResult("road", -1.0, true, sim_temp_cache);
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("road", false));
        return false;
    }
};

REGINTER_RULE( RoadStratey )