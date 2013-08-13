#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class ComputeScoreRule : public ResourceRule<ComputeScoreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	sim_temp_cache->m_score =
              (sim_temp_cache->m_total_same_score - sim_temp_cache->m_total_diff_score)
    		/ (sim_temp_cache->m_total_same_score + sim_temp_cache->m_total_diff_score);
    	return false;    
    }
};

REGINTER_RULE( ComputeScore )