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


class NameTrainPreRule : public ResourceRule<NameTrainPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
    
    	/////////////////////////
    	// 补全 市旅游局 为 北京市旅游局
    	// 上海站  上海火车站(北京西站 有可能被替成北京西火车站，这个需要处理)
                
    	if (pre_proc_data->m_type != "271016") {
            return true;
        }
        
    	if (pre_proc_data->m_format_name.size() > 6) {

    		if (!startswith(pre_proc_data->m_format_name, "火车站")) {
    			pre_proc_data->m_format_name = replaceStr( pre_proc_data->m_format_name, "火车站", "站");
    		}
    	}

        return true;
    };
};

REGINTER_RULE( NameTrainPre )