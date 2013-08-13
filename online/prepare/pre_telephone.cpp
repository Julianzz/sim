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


class TelephonePreRule : public ResourceRule<TelephonePreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {

    	if (record->m_tel != "") {
    		split(record->m_tel, ";", pre_proc_data->m_tel_set, "");
    	}
    
        return true;
    };
};

REGINTER_RULE( TelephonePre )