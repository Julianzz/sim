#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class UnlackWordStrateyRule : public ResourceRule<UnlackWordStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {

    	if (base_data->m_unlack_word_set.size()
    			!= inc_data->m_unlack_word_set.size()) {
    		// 存在不同的词
    		return setResult("unlack_word", -1.0, true, sim_temp_cache);
    	} else if (!checkInclude(base_data->m_unlack_word_set,
    			inc_data->m_unlack_word_set, inc_data->m_name_set)
    			&& !checkInclude(inc_data->m_unlack_word_set,
    					base_data->m_unlack_word_set, base_data->m_name_set)) {
    		// 存在不同的词
    		return setResult("unlack_word", -1.0, true, sim_temp_cache);
    	}

    //	if (base_data->m_unlack_word_set.size()
    //			!= inc_data->m_unlack_word_set.size()) {
    //		return setResult("unlack_word", -1.0, true, sim_temp_cache);
    //	} else if (base_data->m_unlack_word_set.size() != 0) {
    //		set<string>::const_iterator it = base_data->m_unlack_word_set.begin();
    //		for (; it != base_data->m_unlack_word_set.end(); ++it) {
    //			if (inc_data->m_unlack_word_set.find(*it)
    //					== inc_data->m_unlack_word_set.end()) {
    //				// 存在不同的词
    //				return setResult("unlack_word", -1.0, true, sim_temp_cache);
    //			}
    //		}
    //	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("unlack_word", false));
    	return false;
    }
};

REGINTER_RULE( UnlackWordStratey )