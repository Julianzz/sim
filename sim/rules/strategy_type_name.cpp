#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class TypeNameStrateyRule : public ResourceRule<TypeNameStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (sim_temp_cache->m_spec_obj_name_set.size() > 0) {
    		// 特殊连锁店类型的类型词不使用该策略
    		sim_temp_cache->m_rule_result_map.insert(make_pair("type_name", false));
    		return false;
    	}

    	_mapStrDbl::const_iterator base_it, inc_it, end_it;
    	base_it = resource_->typeNameMap.find(base_data->m_format_name);
    	inc_it = resource_->typeNameMap.find(inc_data->m_format_name);
    	end_it = resource_->typeNameMap.end();

    	if (base_it != end_it && inc_it != end_it) {
    		if (base_it->first == inc_it->first) {
    			if (sim_temp_cache->m_addr_sim < 0.15) {
    				// 地址冲突不聚合
    				return setResult("type_name", -1.0, true, sim_temp_cache);
    			} else if (sim_temp_cache->m_same_tel_set.size() > 0
    					|| (sim_temp_cache->m_addr_sim > 0.52
    							&& base_data->m_addr_level < 3
    							&& inc_data->m_addr_level < 3)
    					|| (base_it->second > sim_temp_cache->m_distance)) {
    				// 电话相同 地址相似 距离很近
    				return setResult("type_name", 1.0, true, sim_temp_cache);
    			} else {
    				return setResult("type_name", -1.0, true, sim_temp_cache);
    			}
    		}
    	} else if (base_it != end_it ^ inc_it != end_it) {
    		return setResult("type_name", -1.0, true, sim_temp_cache);
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("type_name", false));
    	return false;
    }
};

REGINTER_RULE( TypeNameStratey )