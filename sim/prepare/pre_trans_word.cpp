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


class WordTransPreRule : public ResourceRule<WordTransPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {

    	// 获取名称中非类型词数量
    	int count = 0;
    	countNotType(pre_proc_data->m_name_vec, count, resource_->typeword);

    	// 同义词替换
    	transWord(pre_proc_data->m_name_vec, resource_->transWord);
    	transWord(pre_proc_data->m_bracket_vec, resource_->transWord);
    	transWord(pre_proc_data->m_accurate_kw_vec, resource_->transWord);

        return true;
    };
};

REGINTER_RULE( WordTransPre )