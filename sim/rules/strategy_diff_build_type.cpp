#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffBuidTypeStrateyRule : public ResourceRule<DiffBuidTypeStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	bool base_is_building = false, inc_is_building = false;
    	if (base_data->m_sub_type == "2812"
    			|| base_data->m_soso_sub_type == "2812") {
    		base_is_building = true;
    	}

    	if (inc_data->m_sub_type == "2812" || inc_data->m_soso_sub_type == "2812") {
    		inc_is_building = true;
    	}

    	if (sim_temp_cache->m_same_type_str != "2812"
    			&& (base_is_building ^ inc_is_building)) {
    		return setResult("diff_build_type", -1.0, true, sim_temp_cache);
    	}

    	sim_temp_cache->m_rule_result_map.insert(
    			make_pair("diff_build_type", false));
    	return false;
    }
};

REGINTER_RULE( DiffBuidTypeStratey )