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
                
    	// 连锁店特别处理
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
    					// 是类型词，不算分店词
    					pre_proc_data->m_fendian = *word_it;
    				}
    			}
    		}

    		pre_proc_data->m_bracket_vec.clear();
    	}

    	// 医院带科室的特别处理 北京新世纪妇儿医院(儿科)
    	if (pre_proc_data->m_main_type == "20"
    			&& pre_proc_data->m_bracket_vec.size() > 0
    			&& endswith(pre_proc_data->m_del_punc_name, "科")) {
    		pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
    				pre_proc_data->m_bracket_vec.begin(),
    				pre_proc_data->m_bracket_vec.end());
    		pre_proc_data->m_bracket_vec.clear();
    	}

    	// 解决 行宫园(三里) 行宫园(一里)误聚的情况
    	// 针对特别类型，括号中的内容需要计算相似度的时候考虑
    	// 对于 怡馨家园 怡馨家园 怡馨家园(怡园公园西北) 情况，括号中以方位结尾不考虑进相似度计算
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