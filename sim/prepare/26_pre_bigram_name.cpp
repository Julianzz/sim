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


class BigramPreRule : public ResourceRule<BigramPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
            
    	// Éú³ÉbigramÃû³Æ
    	vec2str(pre_proc_data->m_name_vec, pre_proc_data->m_bigram_name);
    	if (startswith(pre_proc_data->m_type, "26")) {
    		pre_proc_data->m_bigram_name = pre_proc_data->m_del_punc_name;
    	} else if (pre_proc_data->m_bigram_name == "") {
    		pre_proc_data->m_bigram_name = pre_proc_data->m_del_punc_name;
    	} else if (resource_->xzqh.find(pre_proc_data->m_bigram_name)
    			!= resource_->xzqh.end()) {
    		pre_proc_data->m_bigram_name = pre_proc_data->m_del_punc_name;
    	}

        return true;
    };
};

REGINTER_RULE( BigramPre )