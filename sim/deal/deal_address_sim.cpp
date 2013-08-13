#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

#include "sim.h"
#include <algorithm>
#include <cmath>
#include "rules.h"
#include "tools.h"
#include "init.h"
#include "prepare.h"
#include "utils/MapTools.h"
#include "utils/computeAddressSimilarity.h"
#include "utils/classify_data/classify_data.h"
#include "utils/classify_data/string_process.h"


class AddressSimDealRule : public ResourceRule<AddressSimDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    //	sim_temp_cache->m_addr_sim = computeAddressSimilarity(
    //			base_data->m_addrsim_xzqh_map, inc_data->m_addrsim_xzqh_map,
    //			base_data->m_addrsim_road_map, inc_data->m_addrsim_road_map,
    //			base_data->m_addrsim_poi_set, inc_data->m_addrsim_poi_set);

    	sim_temp_cache->m_addr_sim = computeAddressSimilarity(
    			base_data->m_addrsim_xzqh_map, 
                inc_data->m_addrsim_xzqh_map,
    			base_data->m_addrsim_road_map, 
                inc_data->m_addrsim_road_map,
    			base_data->m_addrsim_community_set,
    			inc_data->m_addrsim_community_set, 
                base_data->m_addrsim_poi_set,
    			inc_data->m_addrsim_poi_set, 
                base_data->m_addrsim_hn_set,
    			inc_data->m_addrsim_hn_set, 
                base_data->m_addrsim_home_set,
    			inc_data->m_addrsim_home_set, 
                base_data->m_addrsim_floor_set,
    			inc_data->m_addrsim_floor_set, 
                sim_temp_cache->m_addr_sim_value,
    			resource_->m_group_score, 
                resource_->m_sigle_score);
                
    	return false;    
    }
};

REGINTER_RULE( AddressSimDeal )