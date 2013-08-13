#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffTypeStrateyRule : public ResourceRule<DiffTypeStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	//cout << base_data->m_accurate_type << "\t" << inc_data->m_accurate_type << endl;
    	//cout << base_data->m_accurate_main_type << "\t" << inc_data->m_accurate_main_type << endl;
    	//cout << base_data->m_accurate_sub_type << "\t" << inc_data->m_accurate_sub_type << endl;

    	// 连锁店如果相同，不考虑类型的问题
    //	if (sim_temp_cache->m_obj_name_set.size() > 0) {
    //		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_type", false));
    //		return false;
    //	}

    	if(base_data->m_soso_type != ""
    			&& inc_data->m_soso_type != ""
    			&& !isTypeConflict(base_data->m_soso_type,
    					inc_data->m_soso_type, resource_)){
    		// sosotype 如果存在能聚合的可能，不能判定类型冲突
    	}else if (sim_temp_cache->m_is_mode_self
    			&& isConflict(base_data, inc_data, resource_)
    			&& base_data->m_del_punc_name != inc_data->m_del_punc_name) {
    		return setResult("diff_type", -1.0, true, sim_temp_cache);
    	}

    	// 精准类有一边没有识别出来不做判断
    	if (base_data->m_accurate_type == "000000"
    			|| inc_data->m_accurate_type == "000000") {
    		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_type", false));
    		return false;
    	}

    	if (base_data->m_accurate_type != inc_data->m_accurate_type
    			&& base_data->m_accurate_sub_type == inc_data->m_accurate_sub_type
    			&& !endswith(base_data->m_accurate_type, "99")
    			&& !endswith(inc_data->m_accurate_type, "99")) {
    		return setResult("diff_type", -1.0, true, sim_temp_cache);
    	} else if (base_data->m_accurate_sub_type != inc_data->m_accurate_sub_type
    			&& base_data->m_accurate_main_type == inc_data->m_accurate_main_type
    			&& !endswith(base_data->m_accurate_sub_type, "99")
    			&& !endswith(inc_data->m_accurate_sub_type, "99")) {
    		return setResult("diff_type", -1.0, true, sim_temp_cache);
    	}

    	bool is_not_same = false;
    	if (base_data->m_accurate_main_type == inc_data->m_accurate_main_type
    			|| base_data->m_accurate_sub_type == inc_data->m_accurate_sub_type
    			|| base_data->m_accurate_type != inc_data->m_accurate_type) {
    		is_not_same = false;
    	} else if (base_data->m_accurate_main_type != inc_data->m_accurate_main_type
    			&& !endswith(base_data->m_accurate_main_type, "99")
    			&& !endswith(inc_data->m_accurate_main_type, "99")) {
    		is_not_same = true;
    	} else if (base_data->m_accurate_sub_type != inc_data->m_accurate_sub_type
    			&& !endswith(base_data->m_accurate_sub_type, "99")
    			&& !endswith(inc_data->m_accurate_sub_type, "99")) {
    		is_not_same = true;
    	} else if (base_data->m_accurate_type != inc_data->m_accurate_type
    			&& !endswith(base_data->m_accurate_type, "99")
    			&& !endswith(inc_data->m_accurate_type, "99")) {
    		is_not_same = true;
    	}

    	if (base_data->m_accurate_type != "" && inc_data->m_accurate_type != "") {
    		if (is_not_same) {
    			// 精准类不相同
    			return setResult("diff_type", -1.0, true, sim_temp_cache);
    		}
    	} else if (sim_temp_cache->m_same_type_set.size() == 0) {
    		return setResult("diff_type", -1.0, true, sim_temp_cache);
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_type", false));
    	return false;
    }
};

REGINTER_RULE( DiffTypeStratey )