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


class NameAddressPreRule : public ResourceRule<NameAddressPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	// 获取地支的数据
    	if (pre_proc_data->m_main_type == "28") {
    		findDizhi(pre_proc_data, resource_);
    	}
        
        return true;
    };
};

REGINTER_RULE( NameAddressPre )