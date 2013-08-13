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


class NameTransPreRule : public ResourceRule<NameTransPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {

        	transWord(pre_proc_data->m_raw_bracket_vec, resource_->transWord);

        	vector<string>::iterator it = pre_proc_data->m_raw_bracket_vec.begin();
        	vector<string> temp;
        	for (; it != pre_proc_data->m_raw_bracket_vec.end(); ++it) {
        		if (len(*it) > 3) {
        			tools_->m_wordsplit->split(*it, temp);
        		} else {
        			temp.push_back(*it);
        		}
        		pre_proc_data->m_bracket_vec.insert(pre_proc_data->m_bracket_vec.end(),
        				temp.begin(), temp.end());
        	}

        	if (len(pre_proc_data->m_accurate_keyword) > 3) {
        		tools_->m_wordsplit->split(pre_proc_data->m_accurate_keyword,
        				pre_proc_data->m_accurate_kw_vec);
        	} else {
        		pre_proc_data->m_accurate_kw_vec.push_back(
        				pre_proc_data->m_accurate_keyword);
        	}

        	// ����ǰ׺
        	addPrefix(pre_proc_data->m_name_vec, resource_->needPrefix);
        	addPrefix(pre_proc_data->m_bracket_vec, resource_->needPrefix);

        	// ��������
        	// ����ոۿƼ�����a �е�a����չΪa��
        	addSuffix(pre_proc_data);

        	// ������������ͬ�������Ĵ�
        	transSpecWord(pre_proc_data);

        	// �����β�ĵ�
        	if (pre_proc_data->m_name_vec.size() > 2) {
        		it = pre_proc_data->m_name_vec.end() - 1;
        		if (*it == "��") {
        			pre_proc_data->m_name_vec.erase(it);
        		}
        	}

        	if (pre_proc_data->m_bracket_vec.size() > 1) {
        		it = pre_proc_data->m_bracket_vec.end() - 1;
        		if (*it == "��") {
        			pre_proc_data->m_bracket_vec.erase(it);
        		}
        	}

        
        return true;
    };
};

REGINTER_RULE( NameTransPre )