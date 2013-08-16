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


class NamesSetPreRule : public ResourceRule<NamesSetPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
              
    	// �������
    	_mapStrIntSet::const_iterator sm_it = resource_->smNameMap.find(
    			pre_proc_data->m_name);
    	if (sm_it != resource_->smNameMap.end()) {
    		pre_proc_data->m_smname_group_set = sm_it->second;
    	}
      
    	// �����ֵ����ƵĴ���
    	filteDoor(pre_proc_data->m_num_name_set, pre_proc_data->m_door_set);

    	// �淶������Ϣ
    	formatDoorSet(pre_proc_data->m_door_set);

    	// ����ִʾ���һ���������
    	findNumWord(pre_proc_data, resource_);

    	//���� �� ������ص�����
    	transZhNum(pre_proc_data->m_door_set, resource_->number, resource_->number2,
    			0);

    	// ����ȱʧ��ʶ��
    	//findUnlackWord(pre_proc_data, resource_->unlackword);

    	// ���ֵ���зֵ�淶
    	if (pre_proc_data->m_fendian != "") {
    		pre_proc_data->m_fendian = replaceStr(pre_proc_data->m_fendian, "�ֵ�",
    				"��");
    	}   
                     
    	// ����delpuncName�ĳ���
    	pre_proc_data->m_del_punc_name_len = len(pre_proc_data->m_del_punc_name);

    	// ��ѧУ���**�� ���Ƶ�����������
    	// ��� ����һ��  �����һ��ѧ  ��ͬ����û�����ݵ�����
    	if (pre_proc_data->m_main_type == "24"
    			|| pre_proc_data->m_soso_main_type == "24") {
    		getNumWord(pre_proc_data->m_num_name_set, pre_proc_data->m_name_vec);
    	}

    	// ��ʼ��������ص�set����
    	pre_proc_data->m_name_set.insert(pre_proc_data->m_name_vec.begin(),
    			pre_proc_data->m_name_vec.end());
    	if (pre_proc_data->m_bracket_vec.size() > 0) {
    		pre_proc_data->m_bracket_set.insert(
    				pre_proc_data->m_bracket_vec.begin(),
    				pre_proc_data->m_bracket_vec.end());
    	}

    	if (pre_proc_data->m_accurate_kw_vec.size() > 0) {
    		pre_proc_data->m_accurate_kw_set.insert(
    				pre_proc_data->m_accurate_kw_vec.begin(),
    				pre_proc_data->m_accurate_kw_vec.end());
    	}

        return true;
    };
};

REGINTER_RULE( NamesSetPre )