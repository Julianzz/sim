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


class RemovePuncPreRule : public ResourceRule<RemovePuncPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    // remove puctuation,
    // translat words
    // others
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
        
        std::string oldName = pre_proc_data->m_format_name;
    	pre_proc_data->m_del_punc_name = pre_proc_data->m_format_name;

    	// 去掉标点符号
    	removePunc(pre_proc_data->m_format_name, true);
    	removePunc(pre_proc_data->m_del_punc_name, false);             
        
    	// 中文数字转阿拉伯数字
    	transZhNum(pre_proc_data->m_format_name, resource_->number,
    			resource_->number2, 1);

    	transZhNum(pre_proc_data->m_del_punc_name, resource_->number,
    			resource_->number2, 0);

    	// 规范“第”
    	if (pre_proc_data->m_main_type == "20"
    			|| pre_proc_data->m_main_type == "24") {
    		formatNumName(pre_proc_data->m_format_name);
    	}
        
        //std::cout<<"====="<<oldName<<","<<pre_proc_data->m_del_punc_name<<","<<
        //     pre_proc_data->m_format_name <<std::endl;
        
        return true;
    };
};

REGINTER_RULE( RemovePuncPre )