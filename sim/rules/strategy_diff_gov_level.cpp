#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class DiffGovLevelStrateyRule : public ResourceRule<DiffGovLevelStrateyRule> {
    
public:
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_del_punc_name == inc_data->m_del_punc_name) {
    		// ������ȫһ������Ϊ���͵ı��磺������û�������������ʶ�������ɼ������
    		sim_temp_cache->m_rule_result_map.insert(
    				make_pair("diff_gov_level", false));
    		return false;
            
    	} else if ((base_data->m_gov_level > 4 && inc_data->m_gov_level > 4)
    			|| (base_data->m_gov_level <= 4 && inc_data->m_gov_level <= 4)) {
    		if (base_data->m_gov_lvl_kw == "" ^ inc_data->m_gov_lvl_kw == "") {
    			if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3
    					&& sim_temp_cache->m_addr_sim < 0.52) {
    				// ��һ��û��ʶ������������ҵ�ַ���Բ�ͬ
    				return setResult("diff_gov_level", -1.0, true, sim_temp_cache);
    			}
    		} else if (base_data->m_gov_level != inc_data->m_gov_level) {
    			return setResult("diff_gov_level", -1.0, true, sim_temp_cache);
    		} else if (base_data->m_gov_lvl_kw != inc_data->m_gov_lvl_kw) {
    			return setResult("diff_gov_level", -1.0, true, sim_temp_cache);
    		}
    	}

    	sim_temp_cache->m_rule_result_map.insert(
    			make_pair("diff_gov_level", false));
        return false;
    }
};

REGINTER_RULE( DiffGovLevelStratey )