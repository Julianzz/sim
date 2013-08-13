#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"


static const char* params[] =  { 
    "airport_recall_distance",
    "airport_reliable_distance",
    "train_recall_distance",
    "train_reliable_distance",
    "default_recall_distance",
    "default_reliable_distance",
    "addr_sim"
 };


class DistanceStrateyRule : public ResourceRule<DistanceStrateyRule> {
    
private:
	double m_airport_recall_distance;
	double m_airport_reliable_distance;

	double m_train_recall_distance;
	double m_train_reliable_distance;

	double m_default_recall_distance;
	double m_default_reliable_distance;

	double m_addr_sim;
    
public:
    
    virtual bool config(Json::Value& items){
        
        for( size_t i =0 ; i< sizeof(params)/sizeof(params[0]); i++ ) {
            if ( items[params[i]].isNull() ) {
                std::cout<< "DiffDoorStratey should provide:"<< params[i] <<std::endl;
                return false;
             }
        }
        
        m_airport_recall_distance = items["airport_recall_distance"].asDouble();
        m_airport_reliable_distance = items["airport_reliable_distance"].asDouble();
        
        m_train_recall_distance = items["train_recall_distance"].asDouble();
        m_train_reliable_distance = items["train_reliable_distance"].asDouble();
        
        m_default_recall_distance = items["default_recall_distance"].asDouble();
        m_default_reliable_distance = items["default_reliable_distance"].asDouble();
        
        m_addr_sim = items["addr_sim"].asDouble();
        
        return true;
    }
    
public:
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_type == "271020"
    			&& base_data->m_type == inc_data->m_type) {
    		// airport
    		if (sim_temp_cache->m_distance <= m_airport_recall_distance) {
    			// 在召回范围的
    			if (endswith(base_data->m_format_name, "机场")
    					&& endswith(inc_data->m_format_name, "机场")) {
    				if (base_data->m_door_set.size() == 0
    						&& inc_data->m_door_set.size() == 0) {
    					return setResult("distance", 1.0, true, sim_temp_cache);
    				}
    			} else if (endswith(base_data->m_format_name, "机场")
    					^ endswith(inc_data->m_format_name, "机场")) {
    				return setResult("distance", -1.0, true, sim_temp_cache);
    			}
    		} else {
    			// 超出召回范围的
    			return setResult("distance", -1.0, true, sim_temp_cache);
    		}
    	}
    //	else if (base_data->m_type == "271016"
    //			&& base_data->m_type == inc_data->m_type) {
    	// train
    //		if (sim_temp_cache->m_distance <= m_train_recall_distance) {
    //			// 在召回范围的
    //			if (sim_temp_cache->m_distance
    //					<= m_train_reliable_distance) {
    //				if (base_data->m_door_set.size() == 0
    //						&& inc_data->m_door_set.size() == 0
    //						&& base_data->m_unlack_word_set.size() == 0
    //						&& inc_data->m_unlack_word_set.size() == 0
    //						&& base_data->m_dpmark_set.size() == 0
    //						&& inc_data->m_dpmark_set.size() == 0
    //						&& base_data->m_num_name_set.size() == 0
    //						&& inc_data->m_num_name_set.size() == 0) {
    //					return setResult("distance", 1.0, true, sim_temp_cache);
    //				}
    //			}
    //		} else {
    //			// 超出召回范围的
    //			return setResult("distance", -1.0, true, sim_temp_cache);
    //		}
    //	}
        
    	else {
    		// normal type data, use default cfg
    		if (sim_temp_cache->m_distance > m_default_recall_distance) {
    			// 超出召回范围的
    			return setResult("distance", -1.0, true, sim_temp_cache);
    		} else if (sim_temp_cache->m_distance
    				> m_default_reliable_distance) {
    			// 超出可信范围
    			if (!nameIsSame(base_data, inc_data, resource_)
    					&& sim_temp_cache->m_same_tel_set.size() == 0
    					&& sim_temp_cache->m_addr_sim < m_addr_sim) {
    				// 名称不同 电话不同 地址不相似
    				return setResult("distance", -1.0, true, sim_temp_cache);
    			}
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("distance", false));
        return false;
    }
};

REGINTER_RULE( DistanceStratey )