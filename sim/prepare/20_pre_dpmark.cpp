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


class DpMarkPreRule : public ResourceRule<DpMarkPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
        
        std::vector<std::string> tempVec;
    	// 获取poi不同的特征词
    	filte(pre_proc_data->m_name_vec, tempVec, pre_proc_data->m_dpmark_set, resource_->DPMark);
    	if (tempVec.size() > 0) {
    		pre_proc_data->m_name_vec = tempVec;
    	} else {
    		pre_proc_data->m_dpmark_set.clear();
    	}
    	if (pre_proc_data->m_bracket_vec.size() < 2) {
    		filte(pre_proc_data->m_bracket_vec, tempVec, pre_proc_data->m_dpmark_set, resource_->DPMark);
    		pre_proc_data->m_bracket_vec = tempVec;
    	}

    	//处理数字相关的内容
    	transZhNum(pre_proc_data->m_dpmark_set, resource_->number, resource_->number2,0);
                        
        return true;
    };
};

REGINTER_RULE( DpMarkPre )