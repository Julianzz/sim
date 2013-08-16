#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class ObjDealRule : public ResourceRule<ObjDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	set_intersection(base_data->m_obj_name_set.begin(),
    			base_data->m_obj_name_set.end(), inc_data->m_obj_name_set.begin(),
    			inc_data->m_obj_name_set.end(),
    			insert_iterator<set<string> >(sim_temp_cache->m_obj_name_set,
    					sim_temp_cache->m_obj_name_set.begin()));

    	_mapStrDbl::const_iterator spec_it, obj_it;
    	set<string>::const_iterator it = sim_temp_cache->m_obj_name_set.begin();
    	for (; it != sim_temp_cache->m_obj_name_set.end(); ++it) {
    		// 获取连锁店可信距离
    		obj_it = resource_->objNameMap.find(*it);
    		if (obj_it != resource_->objNameMap.end()) {
    			if (obj_it->second < sim_temp_cache->m_max_obj_distance) {
    				sim_temp_cache->m_max_obj_distance = obj_it->second;
    			}
    		}

    		// 检查是否是特殊连锁如加油站，厕所等
    		spec_it = resource_->specObjMap.find(*it);
    		if (spec_it != resource_->specObjMap.end()) {
    			sim_temp_cache->m_spec_obj_name_set.insert(spec_it->first);
    			if (spec_it->second < sim_temp_cache->m_max_spec_obj_distance) {
    				sim_temp_cache->m_max_spec_obj_distance = spec_it->second;
    			}
    		}
    	}
        return false;
    }
};

REGINTER_RULE( ObjDeal )