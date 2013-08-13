#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

static const char* params[] =  { "source_name" };

class SpecSourceStrateyRule : public ResourceRule<SpecSourceStrateyRule> {
    
private:
	vector<string> m_source_name_vec;
    
public:

    virtual bool config(Json::Value& items){
        
        for( size_t i =0 ; i< sizeof(params)/sizeof(params[0]); i++ ) {
            if ( items[params[i]].isNull() ) {
                std::cout<< "DiffDoorStratey should provide:"<< params[i] <<std::endl;
                return false;
             }
        }
                
        size_t size = items["source_name"].size();
        for( size_t i = 0; i<size; i++ ) {
            m_source_name_vec.push_back(items["source_name"][i].asString());
        }
        return true;
    }
    
public:
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
            
    	vector<string>::const_iterator it = m_source_name_vec.begin();

    	for (; it != m_source_name_vec.end(); ++it) {
    		if (base_data->m_source == *it || inc_data->m_source == *it) {
    			return setResult("spec_source", -1.0, true, sim_temp_cache);
    		}
    	}
    	sim_temp_cache->m_rule_result_map.insert(make_pair("spec_source", false));
    	return false;
    }
};

REGINTER_RULE( SpecSourceStratey )