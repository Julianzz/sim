#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class AreaSubPoiScoreRule : public ResourceRule<AreaSubPoiScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    //	if (base_data->m_main_type != inc_data->m_main_type) {
    //		return;
    //	}

    	int base_count = 0, inc_count = 0;

    	if (sim_temp_cache->m_is_mode_self && !sim_temp_cache->m_name_is_include) {
    		if (sim_temp_cache->m_addr_sim >= 0.52
    				&& startswith(sim_temp_cache->m_same_type_str, "1310")) {
    			// 地址相同的商场
    		} else {
    			bool base_has_nofreq = hasNotFreqTypeWord(
    					sim_temp_cache->m_diff_words_base_vec, resource_, 1.8,
    					base_count, false);
    			bool inc_has_nofreq = hasNotFreqTypeWord(
    					sim_temp_cache->m_diff_words_inc_vec, resource_, 1.8,
    					inc_count, false);
    			if (base_has_nofreq || inc_has_nofreq) {
    				sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 8;
    			}
    		}
    	}

    	if (sim_temp_cache->m_same_type_set.size() > 1) {
    		bool base_has_nofreq = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_base_vec, resource_, 3.0,
    				base_count, false);
    		bool inc_has_nofreq = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_inc_vec, resource_, 3.0, inc_count,
    				false);

    		if ((base_has_nofreq || inc_has_nofreq)
    				&& !isSameAddr(base_data, inc_data, sim_temp_cache)
    				&& sim_temp_cache->m_same_tel_set.size() == 0) {
    			sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 4;
    		}
    	} else if (startswith(sim_temp_cache->m_same_type_str, "12")) {
    		// 政府机构类型
    		// 延庆镇下水磨村委会综合治理办公室	延庆镇下水磨村村民委员会 非频繁词出现在diff中要减分

    		if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec, resource_,
    				3.5, base_count, true)
    				|| hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
    						resource_, 3.5, inc_count, true)) {
    			if (base_data->m_addr_level > 2 && inc_data->m_addr_level > 2
    					&& sim_temp_cache->m_same_tel_set.size() == 0) {
    				sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 6;
    			} else if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3
    					&& sim_temp_cache->m_addr_sim < 0.52) {
    				sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 6;
    			} else {
    				sim_temp_cache->m_area_sub_poi_score -= resource_->max_idf * 1.5
    						* (sim_temp_cache->m_diff_words_base_vec.size()
    								+ sim_temp_cache->m_diff_words_inc_vec.size());
    			}
    		}
    	} else if (startswith(sim_temp_cache->m_same_type_str, "19")) {
    		if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec, resource_,
    				3.1, base_count, false)
    				&& hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
    						resource_, 3.1, inc_count, false)) {
    			//cout << "-=-==" << base_count << "\t" << inc_count << endl;
    			if (base_count > 0 && inc_count > 0) {
    				sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 4;
    				if (sim_temp_cache->m_tel_score > 0) {
    					sim_temp_cache->m_total_same_score -=
    							sim_temp_cache->m_tel_score;
    				}

    				if (sim_temp_cache->m_addr_score > 0) {
    					sim_temp_cache->m_total_same_score -=
    							sim_temp_cache->m_addr_score;
    				}
    			}
    		}
    	} else if (startswith(sim_temp_cache->m_same_type_str, "10")) {
    		if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec, resource_,
    				3.5, base_count, false)
    				&& hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
    						resource_, 3.5, inc_count, false)) {
    			if (base_count > 0 || inc_count > 0) {
    				sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 5;
    				if (sim_temp_cache->m_tel_score > 0) {
    					sim_temp_cache->m_total_same_score -=
    							sim_temp_cache->m_tel_score;
    				}

    				if (sim_temp_cache->m_addr_score > 0) {
    					sim_temp_cache->m_total_same_score -=
    							sim_temp_cache->m_addr_score;
    				}
    			}
    		}
    	} else if (startswith(sim_temp_cache->m_same_type_str, "1313")) {
    		if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec, resource_,
    				2.8, base_count, false)
    				&& hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
    						resource_, 2.8, inc_count, false)) {
    			if (base_count > 0 || inc_count > 0) {
    				sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 4;
    			}
    		}
    	} else if (startswith(sim_temp_cache->m_same_type_str, "22")
    			|| startswith(sim_temp_cache->m_same_type_str, "23")
    			|| startswith(sim_temp_cache->m_same_type_str, "1425")) {
    		bool base_has_nofreq = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_base_vec, resource_, 2.8,
    				base_count, false);
    		bool inc_has_nofreq = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_inc_vec, resource_, 2.8, inc_count,
    				false);
    		if (base_has_nofreq || inc_has_nofreq) {
    			if (base_count > 0 || inc_count > 0) {
    				sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 5;
    				if (sim_temp_cache->m_tel_score > 0) {
    					sim_temp_cache->m_total_same_score -=
    							sim_temp_cache->m_tel_score;
    				}

    				if (sim_temp_cache->m_addr_score > 0) {
    					sim_temp_cache->m_total_same_score -=
    							sim_temp_cache->m_addr_score;
    				}
    			}
    		}
    	} else if (startswith(sim_temp_cache->m_same_type_str, "2810")) {
    		bool base_has_nofreq = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_base_vec, resource_, 1.2,
    				base_count, false);
    		bool inc_has_nofreq = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_inc_vec, resource_, 1.2, inc_count,
    				false);
    		if (base_has_nofreq || inc_has_nofreq) {
    			sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 4;
    		}
    	} else if (startswith(sim_temp_cache->m_same_type_str, "20")
    			&& endswith(base_data->m_bigram_name, "卫生站")
    			&& endswith(inc_data->m_bigram_name, "卫生站")) {
    		// 不老屯镇黄土坎村社区卫生服务站	黄土坎村社区卫生站
    		if (endswith(base_data->m_bigram_name, inc_data->m_bigram_name)
    				|| endswith(inc_data->m_bigram_name,
    						base_data->m_bigram_name)) {
    			// 有包含关系
    		} else if (isSameAddr(base_data, inc_data, sim_temp_cache)
    				|| sim_temp_cache->m_same_tel_set.size() > 0) {
    			// 电话，地址相同
    		} else {
    			sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 4;
    		}
    	} else {
    		// 当是面数据时，不同部分出现非频繁词时，需要减分
    		// 银通小区铁三局宿舍   银通小区
    		bool base_has_nofreq_high = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_base_vec, resource_, 3.2,
    				base_count, false);
    		bool inc_has_nofreq_high = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_inc_vec, resource_, 3.2, inc_count,
    				false);

    		bool base_has_nofreq_low = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_base_vec, resource_, 3.2,
    				base_count, false);
    		bool inc_has_nofreq_low = hasNotFreqTypeWord(
    				sim_temp_cache->m_diff_words_inc_vec, resource_, 3.2, inc_count,
    				false);

    		if (base_has_nofreq_high || inc_has_nofreq_high) {
    			if ((base_data->m_addr_level > 2 || inc_data->m_addr_level > 2)) {
    				// 地址不详
    				sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 4;
    			} else if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3
    					&& sim_temp_cache->m_addr_sim < 0.52) {
    				// 地址明显不一样，距离较远（不是拐角）
    				sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf * 4;
    			}
    		} else if (base_has_nofreq_low || inc_has_nofreq_low) {
    			if ((base_data->m_addr_level > 2 || inc_data->m_addr_level > 2)) {
    				// 地址不详
    				if (sim_temp_cache->m_same_tel_set.size() == 0) {
    					// 电话没有相同的
    					sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf
    							* 4;
    				} else {
    					sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf
    							* 3;
    				}
    			} else if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3
    					&& sim_temp_cache->m_addr_sim < 0.52
    					&& sim_temp_cache->m_distance > 50) {
    				// 地址明显不一样，距离较远（不是拐角）
    				if (sim_temp_cache->m_same_tel_set.size() == 0) {
    					// 电话没有相同的
    					sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf
    							* 4;
    				} else {
    					sim_temp_cache->m_area_sub_poi_score += -resource_->max_idf
    							* 3;
    				}
    			}
    		}
    	}

    	if (sim_temp_cache->m_area_sub_poi_score > 0) {
    		sim_temp_cache->m_total_same_score +=
    				sim_temp_cache->m_area_sub_poi_score;
    	} else {
    		sim_temp_cache->m_total_diff_score +=
    				-sim_temp_cache->m_area_sub_poi_score;
    	}
    	return false;    
    }
};

REGINTER_RULE( AreaSubPoiScore )