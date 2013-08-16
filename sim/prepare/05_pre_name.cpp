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
    	//====================   ���ƴ��� ===================
    	// ȫ��ת���
    	// ��Ҫʶ��ĩβ�Ƿ��к�׺��������ʱ��ʹ������ȫ�Ǵ����name�ȽϷ���
    	pre_proc_data->m_name = sbc2dbc(pre_proc_data->m_name);

    	// ��������򵥵�Ԥ����󵱳�ԭʼ����ʹ��
    	pre_proc_data->m_format_name = pre_proc_data->m_name;

    	// ����ת����
    	pre_proc_data->m_format_name = tools_->m_trad2simp->func(
    			pre_proc_data->m_format_name);

    	// ��ĸ��дתСд
    	lower(pre_proc_data->m_format_name);
        
        return true;
    };
};

REGINTER_RULE( NamePre )