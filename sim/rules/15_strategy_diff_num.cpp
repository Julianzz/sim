#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffNumStrateyRule : public ResourceRule<DiffNumStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (sim_temp_cache->m_obj_name_set.size() > 0
    			&& sim_temp_cache->m_addr_sim >= 0.52
    			&& sim_temp_cache->m_distance <= sim_temp_cache->m_max_obj_distance
    			&& sim_temp_cache->m_spec_obj_name_set.size() == 0) {
    		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_num", false));
    		return false;
    	} else if (((base_data->m_num_name_set.size() == 1
    			&& inc_data->m_num_name_set.size() == 0)
    			|| (base_data->m_num_name_set.size() == 0
    					&& inc_data->m_num_name_set.size() == 1))
    			&& (startswith(sim_temp_cache->m_same_type_str, "10")
    					|| startswith(sim_temp_cache->m_same_type_str, "13")
    					|| startswith(sim_temp_cache->m_same_type_str, "14")
    					|| startswith(sim_temp_cache->m_same_type_str, "16"))
    			&& (sim_temp_cache->m_addr_sim >= 0.0
    					|| sim_temp_cache->m_same_tel_set.size() > 0)) {
    		// 只有一边有号码，且地址没有明显冲突的特别类型
    		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_num", false));
    		return false;
    	}

    	if (base_data->m_num_name_set.size() != inc_data->m_num_name_set.size()) {
    		// 数字内容不同
    		if (sim_temp_cache->m_same_type_str == "201400"
    				&& sim_temp_cache->m_distance <= 80) {
    			// 康泰尔医药第6药房	康泰尔医药（胜利北街）离得很近，该规则不能断定是否相同
    		} else if (startswith(sim_temp_cache->m_same_type_str, "11")
    				&& sim_temp_cache->m_distance <= 50) {
    			// 北京联友世纪科技有限责任公司	北京联友世纪科技有限责任公司第六分公司
    		} else {
    			if (base_data->m_num_name_set.size() == 1
    					&& inc_data->m_num_name_set.size() == 0) {
    				// 好乐华联连锁便利店(NO.1019)	好乐华联便利NO.1019的问题
    				if (inc_data->m_bracket_set.find(
    						*(base_data->m_num_name_set.begin()))
    						== inc_data->m_bracket_set.end()) {
    					return setResult("diff_num", -1.0, true, sim_temp_cache);
    				}
    			} else if (base_data->m_num_name_set.size() == 0
    					&& inc_data->m_num_name_set.size() == 1) {
    				if (base_data->m_bracket_set.find(
    						*(inc_data->m_num_name_set.begin()))
    						== base_data->m_bracket_set.end()) {
    					return setResult("diff_num", -1.0, true, sim_temp_cache);
    				}
    			} else {
    				return setResult("diff_num", -1.0, true, sim_temp_cache);
    			}
    		}
    	} else if (base_data->m_num_name_set.size() != 0) {
    		if (!checkSetInclude(base_data->m_num_name_set,
    				inc_data->m_num_name_set)
    				&& !checkSetInclude(inc_data->m_num_name_set,
    						base_data->m_num_name_set)) {
    			// 数字内容不同
    			return setResult("diff_num", -1.0, true, sim_temp_cache);
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_num", false));
    	return false;
    }
};

REGINTER_RULE( DiffNumStratey )