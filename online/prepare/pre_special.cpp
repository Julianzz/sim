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


class SpecialPreRule : public ResourceRule<SpecialPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	// �������ر���
    	if (pre_proc_data->m_obj_name_set.size() != 0
    			|| pre_proc_data->m_type == "251100") {
    		pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
    				pre_proc_data->m_bracket_vec.begin(),
    				pre_proc_data->m_bracket_vec.end());

    		if (pre_proc_data->m_bracket_vec.size() > 0) {
    			_mapSet::const_iterator it = resource_->typeword.find("01");
    			vector<string>::reverse_iterator word_it =
    					pre_proc_data->m_bracket_vec.rbegin();
    			for (; word_it != pre_proc_data->m_bracket_vec.rend(); ++word_it) {
    				if (it->second.find(*word_it) == it->second.end()) {
    					// �����ʹʣ�����ֵ��
    					pre_proc_data->m_fendian = *word_it;
    				}
    			}
    		}

    		pre_proc_data->m_bracket_vec.clear();
    	}

    	// ҽԺ�����ҵ��ر��� ���������͸���ҽԺ(����)
    	if (pre_proc_data->m_main_type == "20"
    			&& pre_proc_data->m_bracket_vec.size() > 0
    			&& endswith(pre_proc_data->m_del_punc_name, "��")) {
    		pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
    				pre_proc_data->m_bracket_vec.begin(),
    				pre_proc_data->m_bracket_vec.end());
    		pre_proc_data->m_bracket_vec.clear();
    	}

    	// ��� �й�԰(����) �й�԰(һ��)��۵����
    	// ����ر����ͣ������е�������Ҫ�������ƶȵ�ʱ����
    	// ���� ��ܰ��԰ ��ܰ��԰ ��ܰ��԰(��԰��԰����) ������������Է�λ��β�����ǽ����ƶȼ���
    	if (((pre_proc_data->m_main_type == "22"
    			&& pre_proc_data->m_sub_type != "2221")
    			|| pre_proc_data->m_type == "281010")
    			&& pre_proc_data->m_bracket_vec.size() > 0
    			&& pre_proc_data->m_raw_bracket_vec.size() > 0) {
    		if (resource_->directionSet.find(
    				pre_proc_data->m_bracket_vec[pre_proc_data->m_bracket_vec.size()
    						- 1]) == resource_->directionSet.end()
    				&& resource_->road.find(
    						pre_proc_data->m_raw_bracket_vec[pre_proc_data->m_raw_bracket_vec.size()
    								- 1]) == resource_->road.end())
    			pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
    					pre_proc_data->m_bracket_vec.begin(),
    					pre_proc_data->m_bracket_vec.end());
    		pre_proc_data->m_bracket_vec.clear();
    	}

        return true;
    };
};

REGINTER_RULE( SpecialPre )