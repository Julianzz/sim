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


class NameFilterPreRule : public ResourceRule<NameFilterPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {

    	// ���˳�����Ӣ���ַ�
    	filteLongAlp(pre_proc_data->m_name_vec);

    	// ����00�ʱ��еĴ�
    	filte(pre_proc_data->m_name_vec, resource_);

        
        return true;
    };
};

REGINTER_RULE( NameFilterPre )