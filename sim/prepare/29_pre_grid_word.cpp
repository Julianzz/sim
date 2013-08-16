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


class GridWordPreRule : public ResourceRule<GridWordPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {              
        
        std::vector<std::string> tempVec;
    	// 建立grid时需要的分词内容
    	pre_proc_data->m_grid_name_set.insert(pre_proc_data->m_name_vec.begin(),
    			pre_proc_data->m_name_vec.end());
    	pre_proc_data->m_grid_name_set.insert(pre_proc_data->m_num_name_set.begin(),
    			pre_proc_data->m_num_name_set.end());

    	setGridKeyWord(pre_proc_data->m_name_vec, pre_proc_data->m_grid_name_set);

    	pre_proc_data->m_grid_name_set.insert(pre_proc_data->m_format_name);
    	pre_proc_data->m_grid_name_set.insert(pre_proc_data->m_name);
    	pre_proc_data->m_grid_name_set.insert(pre_proc_data->m_del_punc_name);
    	pre_proc_data->m_grid_name_set.insert(pre_proc_data->m_bigram_name);

    	if (pre_proc_data->m_name_vec.size() == 1) {
    		// 当没有分词的时候为了减少召回的问题需要做bigram切分
    		spliteBigram(pre_proc_data->m_format_name, tempVec);
    		pre_proc_data->m_grid_name_set.insert(tempVec.begin(), tempVec.end());
    	}

    	vector<string> temp_py_vec;

    	if (pre_proc_data->m_format_name.size() <= 8) {
    		temp_py_vec.clear();
    		strToPY(pre_proc_data->m_format_name, temp_py_vec, resource_->pinyin);
    		pre_proc_data->m_grid_name_set.insert(temp_py_vec.begin(),
    				temp_py_vec.end());
    	}

    	if (pre_proc_data->m_name.size() <= 8) {
    		temp_py_vec.clear();
    		strToPY(pre_proc_data->m_name, temp_py_vec, resource_->pinyin);
    		pre_proc_data->m_grid_name_set.insert(temp_py_vec.begin(),
    				temp_py_vec.end());
    	}

    	if (pre_proc_data->m_bigram_name.size() <= 8) {
    		temp_py_vec.clear();
    		strToPY(pre_proc_data->m_bigram_name, temp_py_vec, resource_->pinyin);
    		pre_proc_data->m_grid_name_set.insert(temp_py_vec.begin(),
    				temp_py_vec.end());
    	}
        
        return true;
    };
};

REGINTER_RULE( GridWordPre )