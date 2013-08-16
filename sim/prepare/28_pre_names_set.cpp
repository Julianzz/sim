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
              
    	// 别名标记
    	_mapStrIntSet::const_iterator sm_it = resource_->smNameMap.find(
    			pre_proc_data->m_name);
    	if (sm_it != resource_->smNameMap.end()) {
    		pre_proc_data->m_smname_group_set = sm_it->second;
    	}
      
    	// 带数字的名称的处理
    	filteDoor(pre_proc_data->m_num_name_set, pre_proc_data->m_door_set);

    	// 规范门类信息
    	formatDoorSet(pre_proc_data->m_door_set);

    	// 处理分词就在一起的数量词
    	findNumWord(pre_proc_data, resource_);

    	//处理 门 数字相关的内容
    	transZhNum(pre_proc_data->m_door_set, resource_->number, resource_->number2,
    			0);

    	// 不可缺失词识别
    	//findUnlackWord(pre_proc_data, resource_->unlackword);

    	// 将分店词中分店规范
    	if (pre_proc_data->m_fendian != "") {
    		pre_proc_data->m_fendian = replaceStr(pre_proc_data->m_fendian, "分店",
    				"店");
    	}   
                     
    	// 计算delpuncName的长度
    	pre_proc_data->m_del_punc_name_len = len(pre_proc_data->m_del_punc_name);

    	// 将学校类的**中 复制到名称容器中
    	// 解决 怀柔一中  怀柔第一中学  相同部分没有内容的问题
    	if (pre_proc_data->m_main_type == "24"
    			|| pre_proc_data->m_soso_main_type == "24") {
    		getNumWord(pre_proc_data->m_num_name_set, pre_proc_data->m_name_vec);
    	}

    	// 初始化名称相关的set容器
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