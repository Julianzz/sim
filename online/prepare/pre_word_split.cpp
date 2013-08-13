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


class WordSplitPreRule : public ResourceRule<WordSplitPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
    	// Ãû³Æ·Ö´Ê
    	if (len(pre_proc_data->m_format_name) > 3) {
    		tools_->m_wordsplit->split(pre_proc_data->m_format_name,
    				pre_proc_data->m_name_vec);
    	} else {
    		pre_proc_data->m_name_vec.push_back(pre_proc_data->m_format_name);
    	}

        return true;
    };
};

REGINTER_RULE( WordSplitPre )