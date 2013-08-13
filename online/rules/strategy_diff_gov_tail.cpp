#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"


static const char* params[] =  { "type_set" };

class DiffGovTailStrateyRule : public ResourceRule<DiffGovTailStrateyRule> {
private:
    set<string> m_type_set;
public:
    
    virtual bool config(Json::Value& items){
        for( size_t i =0 ; i< sizeof(params)/sizeof(params[0]); i++ ) {
            if ( items[params[i]].isNull() ) {
                std::cout<< "DiffDoorStratey should provide:"<< params[i] <<std::endl;
                return false;
             }
        }
        size_t size = items["type_set"].size();
        for( size_t i = 0; i<size; i++ ) {
            m_type_set.insert(items["type_set"][i].asString());
        }
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_del_punc_name != inc_data->m_del_punc_name) {
            
    		if (base_data->m_main_type == inc_data->m_main_type
    				&& (this->m_type_set).find(base_data->m_main_type)
    						!= this->m_type_set.end()) {
    			if (base_data->m_gov_tail != "" && inc_data->m_gov_tail != ""
    					&& base_data->m_gov_tail != inc_data->m_gov_tail) {
    				return setResult("diff_gov_tail", -1.0, true, sim_temp_cache);
    			} else if ((base_data->m_gov_tail != "" ^ inc_data->m_gov_tail != "")
    					&& base_data->m_sub_type != inc_data->m_sub_type
    					&& (base_data->m_name_vec.size() == 1
    							&& inc_data->m_name_vec.size() == 1
    							&& base_data->m_name_vec[0]
    									!= inc_data->m_name_vec[0])) {
    				return setResult("diff_gov_tail", -1.0, true, sim_temp_cache);
    			} else if (base_data->m_sub_type != inc_data->m_sub_type
    					&& base_data->m_sub_type != "2699"
    					&& inc_data->m_sub_type != "2699"
    					&& base_data->m_format_name != inc_data->m_format_name) {
    				return setResult("diff_gov_tail", -1.0, true, sim_temp_cache);
    			}
            }
        }
        return false;
    }
};

REGINTER_RULE( DiffGovTailStratey )