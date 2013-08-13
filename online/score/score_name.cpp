#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class NameScoreRule : public ResourceRule<NameScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    	// same
    	sim_temp_cache->m_name_same_score = 2
    			* sumIDF(sim_temp_cache->m_same_words_vec, resource_) * 2;

    	// sub same
    	sim_temp_cache->m_name_sub_same_score += sumIDF(
    			sim_temp_cache->m_sub_same_words_base_vec, resource_) * 2;
    	sim_temp_cache->m_name_sub_same_score += sumIDF(
    			sim_temp_cache->m_sub_same_words_inc_vec, resource_) * 2;

    	// diff
    	sim_temp_cache->m_name_diff_score += sumIDF(
    			sim_temp_cache->m_diff_words_base_vec, resource_) * 2;
    	sim_temp_cache->m_name_diff_score += sumIDF(
    			sim_temp_cache->m_diff_words_inc_vec, resource_) * 2;

    	// sub diff
    	sim_temp_cache->m_name_sub_diff_score += sumIDF(
    			sim_temp_cache->m_sub_diff_words_base_vec, resource_) * 2;
    	sim_temp_cache->m_name_sub_diff_score += sumIDF(
    			sim_temp_cache->m_sub_diff_words_inc_vec, resource_) * 2;

    	double temp_same_score = 0.0;
    	double temp_diff_score = 0.0;

    //	if (sim_temp_cache->m_name_same_score < 0.001) {
    //		temp_same_score = sim_temp_cache->m_name_sub_same_score;
    //	} else if (sim_temp_cache->m_name_sub_same_score < 2.5) {
    //		temp_same_score = sim_temp_cache->m_name_same_score
    //				+ sim_temp_cache->m_name_sub_same_score;
    //	} else {
    //		temp_same_score = temp_cfg->m_name_same_weight
    //				* sim_temp_cache->m_name_same_score
    //				+ temp_cfg->m_name_sub_same_weight
    //						* sim_temp_cache->m_name_sub_same_score;
    //	}

    	temp_same_score = simConfig_->m_name_same_weight
    			* sim_temp_cache->m_name_same_score
    			+ simConfig_->m_name_sub_same_weight
    					* sim_temp_cache->m_name_sub_same_score;

    //	if (sim_temp_cache->m_name_diff_score < 0.001) {
    //		temp_diff_score = sim_temp_cache->m_name_sub_diff_score;
    //	} else if (sim_temp_cache->m_name_sub_diff_score < 2.5) {
    //		temp_diff_score = sim_temp_cache->m_name_diff_score
    //				+ sim_temp_cache->m_name_sub_diff_score;
    //	} else {
    //		temp_diff_score = temp_cfg->m_name_diff_weight
    //				* sim_temp_cache->m_name_diff_score
    //				+ temp_cfg->m_name_sub_diff_weight
    //						* sim_temp_cache->m_name_sub_diff_score;
    //	}

    	temp_diff_score = simConfig_->m_name_diff_weight
    			* sim_temp_cache->m_name_diff_score
    			+ simConfig_->m_name_sub_diff_weight
    					* sim_temp_cache->m_name_sub_diff_score;

    	sim_temp_cache->m_name_score += temp_same_score - temp_diff_score;

    	sim_temp_cache->m_total_same_score += temp_same_score;
    	sim_temp_cache->m_total_diff_score += temp_diff_score;
    	return false;    
    }
};

REGINTER_RULE( NameScore )