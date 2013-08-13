#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

#include "utils/computeAddressSimilarity.h"
#include "utils/classify_data/classify_data.h"
#include "utils/classify_data/string_process.h"


class AddressPreRule : public ResourceRule<AddressPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	// µØÖ·ÇÐ·Ö
    	string temp_correct_addr = "";
    	splitAddressCorrection(pre_proc_data->m_split_addr.c_str(),
    			temp_correct_addr);

    	getCorrectionAddressPart(temp_correct_addr.c_str(),
    			pre_proc_data->m_addrsim_xzqh_map,
    			pre_proc_data->m_addrsim_road_map,
    			pre_proc_data->m_addrsim_community_set,
    			pre_proc_data->m_addrsim_poi_set, pre_proc_data->m_addrsim_hn_set,
    			pre_proc_data->m_addrsim_home_set,
    			pre_proc_data->m_addrsim_floor_set);
        
        return true;
    };
};

REGINTER_RULE( AddressPre )