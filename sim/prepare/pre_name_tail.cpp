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


class NameTailPreRule : public ResourceRule<NameTailPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {

    	// ���Һ�׺���ͣ�����soso_type
    	findTailType(pre_proc_data->m_format_name, pre_proc_data->m_soso_type,
    			resource_->nameTailMap);

    	if (pre_proc_data->m_soso_type.size() == 6) {
    		pre_proc_data->m_soso_main_type = pre_proc_data->m_soso_type.substr(0,
    				2);
    		pre_proc_data->m_soso_sub_type = pre_proc_data->m_soso_type.substr(0,
    				4);
    	}

    	// ����
    	if (pre_proc_data->m_format_name == "����") {
    		pre_proc_data->m_format_name = pre_proc_data->m_del_punc_name;
    		pre_proc_data->m_raw_bracket_vec.clear();
    	}

    	// ȥ�����ƺ����**���׺
    	findAreaTail(pre_proc_data, resource_);

    	// ȥ��**���׺���ٶ�û���ҵ���׺���͵������Һ�׺����
    	if (pre_proc_data->m_soso_type == "") {
    		findTailType(pre_proc_data->m_format_name, pre_proc_data->m_soso_type,
    				resource_->nameTailMap);
    	}

    	// ��ȡ�����е�������׺
    	if (pre_proc_data->m_main_type == "26") {
    		getGovTail(pre_proc_data->m_format_name, pre_proc_data->m_gov_tail,
    				resource_->typeword);
    	}

    	// ��ȡ�����е��Ե��β�Ŀ��ܵķֵ���Ϣ
    	findFendian(pre_proc_data, resource_);

    	// ȥ�����ʹ����ƺ���������֣����
    	formatTypeName(pre_proc_data, resource_);
        
        return true;
    };
};

REGINTER_RULE( NameTailPre )