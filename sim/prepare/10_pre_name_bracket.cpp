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


class NameBracketPreRule : public ResourceRule<NameBracketPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	// 去掉括号及其内部的内容
        std::string oldName = pre_proc_data->m_format_name;
    	removeBracket(pre_proc_data->m_format_name,
    			pre_proc_data->m_raw_bracket_vec);
        //std::cout<<"++++++"<<oldName<<","<<pre_proc_data->m_format_name<<std::endl;
        return true;
    };
};

REGINTER_RULE( NameBracketPre )