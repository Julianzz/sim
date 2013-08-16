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


class ClassifyPreRule : public ResourceRule<ClassifyPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	// 识别子点类型
    	ClassifyData(pre_proc_data->m_name, 
                record->m_type_name,
    			pre_proc_data->m_accurate_keyword, 
                resource_->cateCodeMap);

        return true;
    };
};

REGINTER_RULE( ClassifyPre )