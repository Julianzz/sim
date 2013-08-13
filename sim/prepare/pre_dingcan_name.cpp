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


class NameDingCanPreRule : public ResourceRule<NameDingCanPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	// 正对订餐小秘书名称特征的特别处理
    	if (pre_proc_data->m_source == "dingcanxiaomishu_hezuo"
    			&& pre_proc_data->m_format_name.find(" ") != string::npos) {
    		formatName(pre_proc_data->m_format_name);
    	}
        
        return true;
    };
};

REGINTER_RULE( NameDingCanPre )