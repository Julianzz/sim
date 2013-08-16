#include "process_rule.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

class ChainStrateyRule : public ResourceRule<ChainStrateyRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                
    	if (base_data->m_obj_name_set.size() == 0
    			&& inc_data->m_obj_name_set.size() == 0) {
    		// ��Ϊ�ղ�������
    	} else if (base_data->m_obj_name_set.size() == 0
    			^ inc_data->m_obj_name_set.size() == 0) {
    		// ��һ��Ϊ�գ���Ϊ��ͬ
    		return setResult("chain", -1.0, true, sim_temp_cache);
    	} else {
    		// ������ͬ
    		if (!checkSetInclude(base_data->m_obj_name_set,
    				inc_data->m_obj_name_set)
    				&& !checkSetInclude(inc_data->m_obj_name_set,
    						base_data->m_obj_name_set)) {
    			// ������������Ϊ��ͬ
    			return setResult("chain", -1.0, true, sim_temp_cache);
    		} else {
    			// ���ڰ�����ϵ
    			if (sim_temp_cache->m_spec_obj_name_set.size() > 0) {
    				// ����������
    				if (sim_temp_cache->m_is_mode_self) {
    					// �Ծ�ģʽ��ַ���꣬�����Զ�����Ǿۺ�
    					if (sim_temp_cache->m_addr_sim < 0.52
    							&& sim_temp_cache->m_distance > 50) {
    						return setResult("chain", -1.0, true, sim_temp_cache);
    					}
    				}

    				if (sim_temp_cache->m_spec_obj_name_set.find("ͣ����")
    						!= sim_temp_cache->m_spec_obj_name_set.end()
    						|| sim_temp_cache->m_spec_obj_name_set.find("����վ")
    								!= sim_temp_cache->m_spec_obj_name_set.end()) {
    					// ͣ���� ���� ����վ
    					if (base_data->m_name_vec.size() > 0
    							&& inc_data->m_name_vec.size() > 0) {
    						// ������ͣ����	��������ͣ����
    						if (sim_temp_cache->m_distance >= 300) {
    							if (sim_temp_cache->m_same_tel_set.size() == 0
    									&& !isSameAddr(base_data, inc_data,
    											sim_temp_cache)) {
    								return setResult("chain", -1.0, true,
    										sim_temp_cache);
    							}
    						}
    					}
    				} else {
    					// ��������������
    					if (!((sim_temp_cache->m_distance
    							< sim_temp_cache->m_max_spec_obj_distance)
    							|| (base_data->m_addr_level < 3
    									&& inc_data->m_addr_level < 3
    									&& sim_temp_cache->m_addr_sim > 0.52)
    							|| sim_temp_cache->m_same_tel_set.size() > 0)) {
    						// ����̫Զ��û�е�ַ�绰��Ϊ�����ο�ʱ��Ϊ��ͬ
    						return setResult("chain", -1.0, true, sim_temp_cache);
    					} else if (sim_temp_cache->m_distance > 1000) {
    						return setResult("chain", -1.0, true, sim_temp_cache);
    					}
    				}
    			} else {
    				// ��ͨ������
    				if (base_data->m_obj_name_set.size()
    						!= inc_data->m_obj_name_set.size()) {
    					// ������������һ��
    					return setResult("chain", -1.0, true, sim_temp_cache);
    				}

    				if (isConflict(base_data, inc_data, resource_)) {
    					// �����г�ͻ����Ϊ�������������жϱ�׼�������ж�
    					sim_temp_cache->m_rule_result_map.insert(
    							make_pair("chain", false));
    					return false;
    				} else if (sim_temp_cache->m_distance > 600
    						&& sim_temp_cache->m_addr_sim < 0.52
    						&& sim_temp_cache->m_same_tel_set.size() == 0
    						&& !nameIsSame(base_data, inc_data, resource_)) {
    					// ����̫Զ��û�е�ַ�绰��Ϊ�����ο�ʱ��Ϊ��ͬ
    					return setResult("chain", -1.0, true, sim_temp_cache);
    				} else if ((isSameFenDian(base_data, inc_data, sim_temp_cache)
    						< 1) && base_data->m_addr_level < 3
    						&& inc_data->m_addr_level < 3
    						&& sim_temp_cache->m_addr_sim < 0.18
    						&& sim_temp_cache->m_distance > 30) {
    					// ��ϸ��ַ��һ��
    					return setResult("chain", -1.0, true, sim_temp_cache);
    				} else if ((isSameAddr(base_data, inc_data, sim_temp_cache)
    						&& sim_temp_cache->m_distance < 200)
    						|| (sim_temp_cache->m_distance
    								< sim_temp_cache->m_max_obj_distance)) {
    					return setResult("chain", 1.0, true, sim_temp_cache);
    				} else if (isSameFenDian(base_data, inc_data, sim_temp_cache)
    						> 0) {
    					if ((isSameAddr(base_data, inc_data, sim_temp_cache)
    							&& sim_temp_cache->m_distance < 300)
    							|| (sim_temp_cache->m_distance
    									< sim_temp_cache->m_max_obj_distance)) {
    						return setResult("chain", 1.0, true, sim_temp_cache);
    					}
    				}
    			}
    		}
    	}

    	if (sim_temp_cache->m_spec_obj_name_set.size() > 0
    			&& sim_temp_cache->m_distance
    					< sim_temp_cache->m_max_obj_distance) {
    		// ��ͨ���������ܽ�
    		return setResult("chain", 1.0, true, sim_temp_cache);
    	}

    	sim_temp_cache->m_rule_result_map.insert(make_pair("chain", false));
    	return false;
    }
};

REGINTER_RULE( ChainStratey )