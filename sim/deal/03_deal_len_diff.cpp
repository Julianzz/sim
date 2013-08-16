#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class LenDiffDealRule : public ResourceRule<LenDiffDealRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
    //	sim_temp_cache->m_len_diff = abs(
    //			(int) base_data->m_del_punc_name_len
    //					- (int) inc_data->m_del_punc_name_len);
    	sim_temp_cache->m_len_diff = (int) levDis(base_data->m_del_punc_name,
    			inc_data->m_del_punc_name);
        return false;
    }
};

REGINTER_RULE( LenDiffDeal )