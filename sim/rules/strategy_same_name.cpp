#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

static const char* params[] =  { "recall_distance","reliable_distance","addr_sim" };

class SameNameStrateyRule : public ResourceRule< SameNameStrateyRule> {
    
private:
	double m_recall_distance;
	double m_reliable_distance;
	double m_addr_sim;
    
public:
    
    virtual bool config(Json::Value& items){
        for( size_t i =0 ; i< sizeof(params)/sizeof(params[0]); i++ ) {
            if ( items[params[i]].isNull() ) {
                std::cout<< "SameNameStratey should provide:"<< params[i] <<std::endl;
             }
        }
        m_recall_distance = items["recall_distance"].asDouble();
        m_reliable_distance = items["reliable_distance"].asDouble();
        m_addr_sim = items["addr_sim"].asDouble();
        
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {

    	// 解决拼音相同 但是类型不同造成的误聚
    	if (sim_temp_cache->m_same_type_str == ""
    			&& base_data->m_del_punc_name != inc_data->m_del_punc_name) {
    		sim_temp_cache->m_rule_result_map.insert(make_pair("same_name", false));
    		return false;
    	}

    	if ((base_data->m_obj_name_set.size() > 0
    			|| inc_data->m_obj_name_set.size() > 0)
    			&& nameIsSame(base_data, inc_data, resource_)) {
    		// 连锁店有可能名称相同，但是地址不一样不能聚合的情况，所以不在该节点处理
    		// 八方山庄 等 因为连锁店识别有问题，类型不一样名称一样的数据，可能聚合不了
    		if (!hasSpecObj(base_data, resource_)
    				&& !hasSpecObj(inc_data, resource_)) {
    			if (sim_temp_cache->m_same_tel_set.size() > 0
    					|| (base_data->m_addr_level < 3
    							&& inc_data->m_addr_level < 3
    							&& sim_temp_cache->m_addr_sim
    									>= m_addr_sim)
    					|| (sim_temp_cache->m_distance
    							< base_data->m_obj_max_distance
    							|| sim_temp_cache->m_distance
    									< inc_data->m_obj_max_distance)) {
    				return setResult("same_name", 1.0, true, sim_temp_cache);
    			}
    		}
    	} else if (nameIsSame(base_data, inc_data, resource_)) {
    		if (sim_temp_cache->m_same_type_str == "271016"
    				&& sim_temp_cache->m_addr_sim > 0.49
    				&& sim_temp_cache->m_distance < 1000) {
    			// 火车站类型，名称完全一样
    			return setResult("same_name", 1.0, true, sim_temp_cache);
    		} else if (sim_temp_cache->m_same_tel_set.size() > 0
    				|| (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3
    						&& sim_temp_cache->m_addr_sim > m_addr_sim)
    				|| sim_temp_cache->m_distance < m_reliable_distance) {
    			// 电话相同或者地址相似或距离很近
    			return setResult("same_name", 1.0, true, sim_temp_cache);
    		} else if (sim_temp_cache->m_same_tel_set.size() == 0
    				&& sim_temp_cache->m_addr_sim <= m_addr_sim
    				&& sim_temp_cache->m_distance > m_reliable_distance) {
    			return setResult("same_name", -1.0, true, sim_temp_cache);
    		} else if (sim_temp_cache->m_distance < m_recall_distance) {
    			// 距离在召回范围和确信范围之间
    			if (base_data->m_type == "241000"
    					&& base_data->m_type == inc_data->m_type) {
    				// 学校大学类
    				return setResult("same_name", 1.0, true, sim_temp_cache);
    			} else if (base_data->m_main_type == inc_data->m_main_type
    					&& resource_->areaTypeSet.find(base_data->m_main_type)
    							!= resource_->areaTypeSet.end()) {
    				// 面类型的点
    				return setResult("same_name", 1.0, true, sim_temp_cache);
    			}
    		}
    	}
    //	else if (base_data->m_format_name == inc_data->m_format_name) {
    //		// 规范名称一样
    //		if ((sim_temp_cache->m_same_tel_set.size() > 0
    //				|| sim_temp_cache->m_addr_sim > m_addr_sim)
    //				&& sim_temp_cache->m_distance <= m_reliable_distance) {
    //			// 电话相同或者地址相似
    //			return setResult("same_name", 0.85, true, sim_temp_cache);
    //		}
    //	}
    	else {
    		if (sim_temp_cache->m_distance > m_reliable_distance) {
    			return setResult("same_name", -1.0, true, sim_temp_cache);
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("same_name", false));
    	return false;
    }
};

REGINTER_RULE( SameNameStratey )