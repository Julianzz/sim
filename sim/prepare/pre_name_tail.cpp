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

    	// 查找后缀类型，即：soso_type
    	findTailType(pre_proc_data->m_format_name, pre_proc_data->m_soso_type,
    			resource_->nameTailMap);

    	if (pre_proc_data->m_soso_type.size() == 6) {
    		pre_proc_data->m_soso_main_type = pre_proc_data->m_soso_type.substr(0,
    				2);
    		pre_proc_data->m_soso_sub_type = pre_proc_data->m_soso_type.substr(0,
    				4);
    	}

    	// 商铺
    	if (pre_proc_data->m_format_name == "商铺") {
    		pre_proc_data->m_format_name = pre_proc_data->m_del_punc_name;
    		pre_proc_data->m_raw_bracket_vec.clear();
    	}

    	// 去掉名称后面的**店后缀
    	findAreaTail(pre_proc_data, resource_);

    	// 去掉**店后缀后，再对没有找到后缀类型的数据找后缀类型
    	if (pre_proc_data->m_soso_type == "") {
    		findTailType(pre_proc_data->m_format_name, pre_proc_data->m_soso_type,
    				resource_->nameTailMap);
    	}

    	// 获取地名中的行政后缀
    	if (pre_proc_data->m_main_type == "26") {
    		getGovTail(pre_proc_data->m_format_name, pre_proc_data->m_gov_tail,
    				resource_->typeword);
    	}

    	// 获取括号中的以店结尾的可能的分店信息
    	findFendian(pre_proc_data, resource_);

    	// 去掉类型词名称后面的无用字，如店
    	formatTypeName(pre_proc_data, resource_);
        
        return true;
    };
};

REGINTER_RULE( NameTailPre )