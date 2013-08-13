#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class AliasStrategyRule : public ResourceRule<AliasStrategyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	vector<int> same;
    	set_intersection(base_data->m_smname_group_set.begin(),
    			base_data->m_smname_group_set.end(),
    			inc_data->m_smname_group_set.begin(),
    			inc_data->m_smname_group_set.end(),
    			insert_iterator<vector<int> >(same, same.begin()));
    	if (same.size() > 0) {
    		return setResult("same_mean", 1.0, true, sim_temp_cache);
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("same_mean", false));
    	return false;

    }
};

REGINTER_RULE( AliasStrategy )