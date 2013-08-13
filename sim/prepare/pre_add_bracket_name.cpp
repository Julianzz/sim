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


class AddBracketNamePreRule : public ResourceRule<AddBracketNamePreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {

    	// 名称只为类型词时，将括号内的内容添加到规范名称中
    	addContentForTWName(pre_proc_data, resource_->typeword);

    	// 特别类型需要将括号内的内容添加到规范名称中
    	if (startswith(pre_proc_data->m_soso_type, "2612")
    			|| (pre_proc_data->m_soso_type == ""
    					&& startswith(pre_proc_data->m_type, "2612"))) {
    		pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
    				pre_proc_data->m_bracket_vec.begin(),
    				pre_proc_data->m_bracket_vec.end());
    		pre_proc_data->m_bracket_vec.clear();
    	}

    	if (startswith(pre_proc_data->m_soso_type, "2410")
    			|| (pre_proc_data->m_soso_type == ""
    					&& startswith(pre_proc_data->m_type, "2410"))) {
    		int in_size = pre_proc_data->m_bracket_vec.size();
    		if (in_size > 0
    				&& endswith(pre_proc_data->m_bracket_vec[in_size - 1], "学院")) {
    			pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
    					pre_proc_data->m_bracket_vec.begin(),
    					pre_proc_data->m_bracket_vec.end());
    			pre_proc_data->m_bracket_vec.clear();
    		}
    	}
        
        return true;
    };
};

REGINTER_RULE( AddBracketNamePre )