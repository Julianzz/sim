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


class NamePreRule : public ResourceRule<NamePreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {

    	//===================================================
    	//====================   名称处理 ===================
    	// 全角转半角
    	// 需要识别末尾是否有后缀括号内容时，使用做过全角处理的name比较方便
    	pre_proc_data->m_name = sbc2dbc(pre_proc_data->m_name);

    	// 名称做完简单的预处理后当成原始名称使用
    	pre_proc_data->m_format_name = pre_proc_data->m_name;

    	// 繁体转简体
    	pre_proc_data->m_format_name = tools_->m_trad2simp->func(
    			pre_proc_data->m_format_name);

    	// 字母大写转小写
    	lower(pre_proc_data->m_format_name);
        
        return true;
    };
};

REGINTER_RULE( NamePre )