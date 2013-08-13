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


class RecordPreRule : public ResourceRule<RecordPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {

    	// ��ȡ���ݵ���������
    	if ((pre_proc_data->m_main_type == "12"
    			|| pre_proc_data->m_soso_main_type == "12")
    			&& pre_proc_data->m_soso_main_type != "11") {
    		findGovLevel(pre_proc_data, resource_);
    	}
        
        int count = 0;
    	// ȥ������������
    	filteDistrict(pre_proc_data, resource_, count);

    	// ��ȡ��������
    	findObjName(pre_proc_data, resource_);

        return true;
    };
};

REGINTER_RULE( RecordPre )