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


class NameGovPreRule : public ResourceRule<NameGovPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	/////////////////////////
    	// ��ȫ �����ξ� Ϊ ���������ξ�
    	// �Ϻ�վ  �Ϻ���վ(������վ �п��ܱ���ɱ�������վ�������Ҫ����)
    	if (pre_proc_data->m_main_type != "12" && 
                pre_proc_data->m_main_type != "20" &&
                pre_proc_data->m_main_type != "24") {
            return true;
        }
        
    	if (startswith(pre_proc_data->m_format_name, "ʡ")
    			&& pre_proc_data->m_province != "") {
                    
    		pre_proc_data->m_format_name = pre_proc_data->m_province
    				+ pre_proc_data->m_format_name.substr(2, pre_proc_data->m_format_name.size() - 2);
                            
    	} else if (startswith(pre_proc_data->m_format_name, "��")
    			&& pre_proc_data->m_city != "") {
    		pre_proc_data->m_format_name = pre_proc_data->m_city
    				+ pre_proc_data->m_format_name.substr(2, pre_proc_data->m_format_name.size() - 2);
            
    	} else if (startswith(pre_proc_data->m_format_name, "��")
    			&& pre_proc_data->m_district != ""
    			&& endswith(pre_proc_data->m_district, "��")) {
                    
    		        pre_proc_data->m_format_name = pre_proc_data->m_district
    				+ pre_proc_data->m_format_name.substr(2, pre_proc_data->m_format_name.size() - 2);
    	}
        
        return true;
    };
};

REGINTER_RULE( NameGovPre )