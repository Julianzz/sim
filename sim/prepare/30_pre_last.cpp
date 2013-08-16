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


class LastPreRule : public ResourceRule<LastPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {              
    	//========================================================================
    	// 标记是处理好的预处理数据
    	pre_proc_data->m_is_null = false;                
        
        return true;
    };
};

REGINTER_RULE( LastPre )