#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class AccurateTypeScoreRule : public ResourceRule<AccurateTypeScoreRule >{
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if ((base_data->m_accurate_keyword == ""
    			&& inc_data->m_accurate_keyword != ""
    			&& inc_data->m_accurate_keyword.find("分校") == std::string::npos
    			&& inc_data->m_accurate_keyword.find("校区") == std::string::npos
    			&& inc_data->m_accurate_keyword.find("分部") == std::string::npos)
    			|| (base_data->m_accurate_keyword != ""
    					&& inc_data->m_accurate_keyword == ""
    					&& base_data->m_accurate_keyword.find("分校")
    							== std::string::npos
    					&& base_data->m_accurate_keyword.find("校区")
    							== std::string::npos
    					&& base_data->m_accurate_keyword.find("分部")
    							== std::string::npos)) {
    		sim_temp_cache->m_accurate_type_score +=
    				-sim_temp_cache->m_total_diff_score
    						* (pow(2.71828, (sim_temp_cache->m_distance / 150.0))
    								- 1)
    						/ (pow(2.71828, sim_temp_cache->m_distance / 150.0)
    								+ 5.21) * 0.75;
    	} else if (base_data->m_accurate_keyword != ""
    			&& inc_data->m_accurate_keyword != ""
    			&& base_data->m_accurate_keyword != inc_data->m_accurate_keyword) {
    		sim_temp_cache->m_accurate_type_score += -(pow(2.71828,
    				(sim_temp_cache->m_distance / 150.0)) - 1)
    				/ (pow(2.71828, sim_temp_cache->m_distance / 150.0) + 5.21)
    				* sim_temp_cache->m_total_same_score * 0.625;
    	}

    	if (sim_temp_cache->m_accurate_type_score > 0) {
    		sim_temp_cache->m_total_same_score +=
    				sim_temp_cache->m_accurate_type_score;
    	} else {
    		sim_temp_cache->m_total_diff_score +=
    				-sim_temp_cache->m_accurate_type_score;
    	}

    	return false;    
    }
};

REGINTER_RULE( AccurateTypeScore )