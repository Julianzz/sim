#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class ChainStrateyRule : public ResourceRule<ChainStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_obj_name_set.size() == 0
    			&& inc_data->m_obj_name_set.size() == 0) {
    		// 都为空不做处理
    	} else if (base_data->m_obj_name_set.size() == 0
    			^ inc_data->m_obj_name_set.size() == 0) {
    		// 有一边为空，认为不同
    		return setResult("chain", -1.0, true, sim_temp_cache);
    	} else {
    		// 数量不同
    		if (!checkSetInclude(base_data->m_obj_name_set,
    				inc_data->m_obj_name_set)
    				&& !checkSetInclude(inc_data->m_obj_name_set,
    						base_data->m_obj_name_set)) {
    			// 互不包含，认为不同
    			return setResult("chain", -1.0, true, sim_temp_cache);
    		} else {
    			// 存在包含关系
    			if (sim_temp_cache->m_spec_obj_name_set.size() > 0) {
    				// 特殊连锁店
    				if (sim_temp_cache->m_is_mode_self) {
    					// 自聚模式地址不详，距离较远不考虑聚合
    					if (sim_temp_cache->m_addr_sim < 0.52
    							&& sim_temp_cache->m_distance > 50) {
    						return setResult("chain", -1.0, true, sim_temp_cache);
    					}
    				}

    				if (sim_temp_cache->m_spec_obj_name_set.find("停车库")
    						!= sim_temp_cache->m_spec_obj_name_set.end()
    						|| sim_temp_cache->m_spec_obj_name_set.find("加油站")
    								!= sim_temp_cache->m_spec_obj_name_set.end()) {
    					// 停车场 或者 加油站
    					if (base_data->m_name_vec.size() > 0
    							&& inc_data->m_name_vec.size() > 0) {
    						// 凯旋城停车场	凯旋中心停车场
    						if (sim_temp_cache->m_distance >= 300) {
    							if (sim_temp_cache->m_same_tel_set.size() == 0
    									&& !isSameAddr(base_data, inc_data,
    											sim_temp_cache)) {
    								return setResult("chain", -1.0, true,
    										sim_temp_cache);
    							}
    						}
    					}
    				} else {
    					// 其他特殊连锁店
    					if (!((sim_temp_cache->m_distance
    							< sim_temp_cache->m_max_spec_obj_distance)
    							|| (base_data->m_addr_level < 3
    									&& inc_data->m_addr_level < 3
    									&& sim_temp_cache->m_addr_sim > 0.52)
    							|| sim_temp_cache->m_same_tel_set.size() > 0)) {
    						// 距离太远且没有地址电话作为辅助参考时认为不同
    						return setResult("chain", -1.0, true, sim_temp_cache);
    					} else if (sim_temp_cache->m_distance > 1000) {
    						return setResult("chain", -1.0, true, sim_temp_cache);
    					}
    				}
    			} else {
    				// 普通连锁店
    				if (base_data->m_obj_name_set.size()
    						!= inc_data->m_obj_name_set.size()) {
    					// 连锁店数量不一样
    					return setResult("chain", -1.0, true, sim_temp_cache);
    				}

    				if (isConflict(base_data, inc_data, resource_)) {
    					// 类型有冲突，认为不符合连锁店判断标准，不做判断
    					sim_temp_cache->m_rule_result_map.insert(
    							make_pair("chain", false));
    					return false;
    				} else if (sim_temp_cache->m_distance > 600
    						&& sim_temp_cache->m_addr_sim < 0.52
    						&& sim_temp_cache->m_same_tel_set.size() == 0
    						&& !nameIsSame(base_data, inc_data, resource_)) {
    					// 距离太远且没有地址电话作为辅助参考时认为不同
    					return setResult("chain", -1.0, true, sim_temp_cache);
    				} else if ((isSameFenDian(base_data, inc_data, sim_temp_cache)
    						< 1) && base_data->m_addr_level < 3
    						&& inc_data->m_addr_level < 3
    						&& sim_temp_cache->m_addr_sim < 0.18
    						&& sim_temp_cache->m_distance > 30) {
    					// 详细地址不一样
    					return setResult("chain", -1.0, true, sim_temp_cache);
    				} else if ((isSameAddr(base_data, inc_data, sim_temp_cache)
    						&& sim_temp_cache->m_distance < 200)
    						|| (sim_temp_cache->m_distance
    								< sim_temp_cache->m_max_obj_distance)) {
    					return setResult("chain", 1.0, true, sim_temp_cache);
    				} else if (isSameFenDian(base_data, inc_data, sim_temp_cache)
    						> 0) {
    					if ((isSameAddr(base_data, inc_data, sim_temp_cache)
    							&& sim_temp_cache->m_distance < 300)
    							|| (sim_temp_cache->m_distance
    									< sim_temp_cache->m_max_obj_distance)) {
    						return setResult("chain", 1.0, true, sim_temp_cache);
    					}
    				}
    			}
    		}
    	}

    	if (sim_temp_cache->m_spec_obj_name_set.size() > 0
    			&& sim_temp_cache->m_distance
    					< sim_temp_cache->m_max_obj_distance) {
    		// 普通连锁店距离很近
    		return setResult("chain", 1.0, true, sim_temp_cache);
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("chain", false));
    	return false;
    }
};

REGINTER_RULE( ChainStratey )