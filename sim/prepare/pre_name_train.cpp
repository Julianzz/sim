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


class NameTrainPreRule : public ResourceRule<NameTrainPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
    
    	/////////////////////////
    	// ��ȫ �����ξ� Ϊ ���������ξ�
    	// �Ϻ�վ  �Ϻ���վ(������վ �п��ܱ���ɱ�������վ�������Ҫ����)
                
    	if (pre_proc_data->m_type != "271016") {
            return true;
        }
        
    	if (pre_proc_data->m_format_name.size() > 6) {

    		if (!startswith(pre_proc_data->m_format_name, "��վ")) {
    			pre_proc_data->m_format_name = replaceStr( pre_proc_data->m_format_name, "��վ", "վ");
    		}
    	}

        return true;
    };
};

REGINTER_RULE( NameTrainPre )