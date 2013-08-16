#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"


static const char* params[] =  { "distance","addr_sim" };

class DiffDpMarkStrateyRule : public ResourceRule<DiffDpMarkStrateyRule> {
private:
	double m_distance;
	double m_addr_sim;
    
public:
    
    virtual bool config(Json::Value& items){
        for( size_t i =0 ; i< sizeof(params)/sizeof(params[0]); i++ ) {
            if ( items[params[i]].isNull() ) {
                std::cout<< "DiffDpMarkStrateyRule should provide:"<< params[i] <<std::endl;
                return false;
             }
        }
        
        m_distance = items["distance"].asDouble();
        m_addr_sim = items["addr_sim"].asDouble();
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_dpmark_set.size() > 0
    			^ inc_data->m_dpmark_set.size() > 0) {
    //		if (sim_temp_cache->m_distance > this->m_distance
    //				|| startswith(sim_temp_cache->m_same_type_str, "28")) {
    //			return setResult("diff_dpmark", -1.0, true, sim_temp_cache);
    //		}
    		return setResult("diff_dpmark", -1.0, true, sim_temp_cache);
    	} else if (base_data->m_dpmark_set.size() > 0
    			&& inc_data->m_dpmark_set.size() > 0) {
    		if (!checkSetInclude(base_data->m_dpmark_set, inc_data->m_dpmark_set)
    				|| !checkSetInclude(inc_data->m_dpmark_set,
    						base_data->m_dpmark_set)) {
    			// ÓÐ³åÍ»µÄ´Ê
    			return setResult("diff_dpmark", -1.0, true, sim_temp_cache);
    		} else {
    			sim_temp_cache->m_rule_result_map.insert(
    					make_pair("diff_dpmark", false));
    			return false;
    		}
    	} else if (base_data->m_dpmark_set.size() == 0
    			^ inc_data->m_dpmark_set.size() == 0) {
    		if (base_data->m_door_set.size() > 0 && inc_data->m_door_set.size() > 0
    				&& (sim_temp_cache->m_distance < this->m_distance
    						|| sim_temp_cache->m_addr_sim > this->m_addr_sim)) {
    			sim_temp_cache->m_rule_result_map.insert(
    					make_pair("diff_dpmark", false));
    			return false;
    		} else {
    			return setResult("diff_dpmark", -1.0, true, sim_temp_cache);
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_dpmark", false));
    	return false;

    }
};

REGINTER_RULE( DiffDpMarkStratey )