#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class SelfSameStrateyRule : public ResourceRule<SelfSameStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	//double threshold = 0.52;

    	sim_temp_cache->m_is_mode_self = true;

    	if (nameIsSame(base_data, inc_data, resource_)
    			&& sim_temp_cache->m_same_type_str == "271016") {
    		sim_temp_cache->m_rule_result_map.insert(make_pair("self_sim", false));
    		return false;
    	}

    	if (startswith(sim_temp_cache->m_same_type_str, "2610")) {
    		if (base_data->m_del_punc_name != inc_data->m_del_punc_name) {
    			return setResult("self_sim", -1.0, true, sim_temp_cache);
    		}
    	}

    	if (!nameIsSame(base_data, inc_data, resource_)) {
    		if (startswith(sim_temp_cache->m_same_type_str, "2410")) {
    			if (sim_temp_cache->m_distance > 650) {
    				return setResult("self_sim", -1.0, true, sim_temp_cache);
    			}
    		} else if ((sim_temp_cache->m_addr_sim < 0.52
    				&& sim_temp_cache->m_same_tel_set.size() == 0)
    				&& sim_temp_cache->m_distance > 150) {
    			return setResult("self_sim", -1.0, true, sim_temp_cache);
    		}
    	}

    	//cout << "1---" << endl;

    	if (base_data->m_dpmark_set.size() > 0
    			|| inc_data->m_dpmark_set.size() > 0) {
    		if (!checkSetInclude(base_data->m_dpmark_set, inc_data->m_dpmark_set)
    				|| !checkSetInclude(inc_data->m_dpmark_set,
    						base_data->m_dpmark_set)) {
    			// 有冲突的词
    			return setResult("self_sim", -1.0, true, sim_temp_cache);
    		}
    	}

    	//cout << "2---" << endl;

    	if (sim_temp_cache->m_spec_obj_name_set.size() > 0) {
    		// 特殊连锁店
    		if (base_data->m_del_punc_name != inc_data->m_del_punc_name) {
    			return setResult("self_sim", -1.0, true, sim_temp_cache);
    		} else if (sim_temp_cache->m_distance > 10
    				&& !isSameAddr(base_data, inc_data, sim_temp_cache)) {
    			return setResult("self_sim", -1.0, true, sim_temp_cache);
    		}
    	}

    	//cout << "3---" << endl;

    	if ((base_data->m_source == "base" || base_data->m_source == "mapabc.com")
    			&& (inc_data->m_source == "base"
    					|| inc_data->m_source == "mapabc.com")) {
    		if (sim_temp_cache->m_len_diff > 4 && sim_temp_cache->m_addr_sim < 0.52
    				&& !sim_temp_cache->m_name_is_include
    				&& sim_temp_cache->m_same_tel_set.size() == 0) {
    			return setResult("self_sim", -1.0, true, sim_temp_cache);
    		}
    	}

    	//cout << "4---" << endl;
    	if (sim_temp_cache->m_same_type_str == ""
    			&& !nameIsSame(base_data, inc_data, resource_)) {
    		sim_temp_cache->m_spec_score += -(pow(2.71828,
    				(sim_temp_cache->m_distance / 150.0)) - 1)
    				/ (pow(2.71828, sim_temp_cache->m_distance / 150.0) + 5.21)
    				* sim_temp_cache->m_name_sub_same_score * 0.625;
    	}

    	//cout << "5---" << endl;

    	map<string, string>::const_iterator it1, it2;
    	it1 = resource_->transWord.find(base_data->m_del_punc_name);
    	it2 = resource_->transWord.find(inc_data->m_del_punc_name);
    	if (it1 != resource_->transWord.end() && it2 != resource_->transWord.end()
    			&& it1->second == it2->second) {
    		// 同义词 不做处理
    		sim_temp_cache->m_rule_result_map.insert(make_pair("self_sim", false));
    		return false;
    	}

    //	if (sim_temp_cache->m_len_diff >= 8) {
    //		return setResult("self_sim", -1.0, true, sim_temp_cache);
    //	}

    	if (sim_temp_cache->m_len_diff >= 6 && sim_temp_cache->m_addr_sim < 0.52
    			&& !sim_temp_cache->m_name_is_include
    			&& sim_temp_cache->m_same_tel_set.size() == 0) {
    		if (base_data->m_main_type == "13" && inc_data->m_main_type == "13"
    				&& base_data->m_sub_type != "1399"
    				&& inc_data->m_sub_type != "1399"
    				&& base_data->m_sub_type != inc_data->m_sub_type) {
    			if (base_data->m_del_punc_name != inc_data->m_del_punc_name) {
    				return setResult("self_sim", -1.0, true, sim_temp_cache);
    			}
    		}
    	}

    	//cout << "6---" << endl;

    	if (sim_temp_cache->m_len_diff >= 8 && sim_temp_cache->m_addr_sim < 0.52
    			&& !sim_temp_cache->m_name_is_include
    			&& sim_temp_cache->m_same_tel_set.size() == 0) {
    //		if (!isSameAddr(base_data, inc_data, sim_temp_cache)) {
    //			return setResult("self_sim", -1.0, true, sim_temp_cache);
    //		} else if (startswith(sim_temp_cache->m_same_type_str, "11")) {
    //			return setResult("self_sim", -1.0, true, sim_temp_cache);
    //		}

    		int lendiff_bigram = (int) levDis(base_data->m_bigram_name,
    				inc_data->m_bigram_name);
    		if (lendiff_bigram >= 4) {
    			if (!isSameAddr(base_data, inc_data, sim_temp_cache)) {
    				// 地址不相似
    				sim_temp_cache->m_addr_score += -(pow(2.71828,
    						(sim_temp_cache->m_distance / 300.0)) - 1)
    						/ (pow(2.71828, sim_temp_cache->m_distance / 300.0)
    								+ 5.21) * resource_->max_idf * 2
    						- resource_->max_idf;
    			}
    		}
    	}

    	//cout << "7---" << endl;

    	if (base_data->m_source == inc_data->m_source) {
    		if (base_data->m_del_punc_name != inc_data->m_del_punc_name
    				&& (!isSameAddr(base_data, inc_data, sim_temp_cache)
    						&& sim_temp_cache->m_same_tel_set.size() == 0)) {
    			sim_temp_cache->m_addr_score += -(pow(2.71828,
    					(sim_temp_cache->m_distance / 300.0)) - 1)
    					/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
    					* resource_->max_idf * 2;
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("self_sim", false));
    	return false;                
    }
};

REGINTER_RULE( SelfSameStratey )
    
    
