#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffWordUnlackStrateyRule : public ResourceRule<DiffWordUnlackStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	string temp_main_type = "";
    	set<string> temp_word_set;

    	temp_word_set.insert(sim_temp_cache->m_diff_words_base_vec.begin(),
    			sim_temp_cache->m_diff_words_base_vec.end());
    	temp_word_set.insert(sim_temp_cache->m_diff_words_inc_vec.begin(),
    			sim_temp_cache->m_diff_words_inc_vec.end());

    	set<string>::iterator it = sim_temp_cache->m_same_type_set.begin();

    	for (; it != sim_temp_cache->m_same_type_set.end(); ++it) {
    		if ((*it).size() >= 2) {
    			temp_main_type = (*it).substr(0, 2);

    			_mapSet::const_iterator map_it = resource_->unlackword.find(
    					temp_main_type);
    			if (map_it != resource_->unlackword.end()) {
    				set<string>::iterator word_it = temp_word_set.begin();
    				for (; word_it != temp_word_set.end(); ++word_it) {
    					if (map_it->second.find(*word_it) != map_it->second.end()) {
    						// 存在不同的词
    						return setResult("diff_word_unlack", -1.0, true,
    								sim_temp_cache);
    					}
    				}
    			}
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(
    			make_pair("diff_word_unlack", false));
    	return false; 
    }
};

REGINTER_RULE( DiffWordUnlackStratey )