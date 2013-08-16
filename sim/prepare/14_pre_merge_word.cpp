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


class MergeWordPreRule : public ResourceRule<MergeWordPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	//	合并单个字
    	vector<string> tempVec;
    	if (mergeWord(pre_proc_data->m_name_vec, tempVec, *resource_) == -1)
    		return false;
    	pre_proc_data->m_name_vec = tempVec;

    	if (mergeWord(pre_proc_data->m_bracket_vec, tempVec) == -1)
    		return false;
    	pre_proc_data->m_bracket_vec = tempVec;

    	if (mergeWord(pre_proc_data->m_accurate_kw_vec, tempVec) == -1)
    		return false;
    	pre_proc_data->m_accurate_kw_vec = tempVec;

        return true;
    };
};

REGINTER_RULE( MergeWordPre )