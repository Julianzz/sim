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


class FormatVecPreRule : public ResourceRule<FormatVecPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {

    	// 规范化数字
    	transNum(pre_proc_data->m_name_vec);
    	transNum(pre_proc_data->m_bracket_vec);

    	// 合并数字,字母内容
    	mergeNumAlp(pre_proc_data, resource_);

    	// 合并结尾的特别单字
    	mergeTailSglWord(pre_proc_data);

    	// 获取 门 相关信息
    	filteDoor2(pre_proc_data->m_raw_bracket_vec, pre_proc_data->m_door_set,
    			*resource_);
    	filteDoorInBrac(pre_proc_data->m_bracket_vec, pre_proc_data->m_door_set,
    			*resource_);
    	filteDoor(pre_proc_data->m_name_vec, pre_proc_data->m_door_set, *resource_);

    	// 小区数量词的规范
    	if (pre_proc_data->m_main_type == "28") {
    		vector<string>::iterator it = pre_proc_data->m_name_vec.begin();
    		for (; it != pre_proc_data->m_name_vec.end(); ++it) {
    			transZhNum(*it, resource_->number, resource_->number2, 0);
    		}
    	}

        
        return true;
    };
};

REGINTER_RULE( FormatVecPre )