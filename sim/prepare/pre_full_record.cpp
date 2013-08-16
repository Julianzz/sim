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


class FullPreRule : public ResourceRule<FullPreRule> {
    
public:
    
    virtual bool config(Json::Value& items){
        return true;
    }
    
    virtual bool processRecord( PreProcData* pre_proc_data,
            const Record* record ) {
                
    	pre_proc_data->clear();
    	if (record->m_is_null) {
    		return false;
    	}

    	// ����ԭʼ��Ϣ��Ԥ��������
    	pre_proc_data->m_id = record->m_id;
    	pre_proc_data->m_lat = record->m_lat;
    	pre_proc_data->m_lng = record->m_lng;
    	pre_proc_data->m_name = record->m_name;
    	pre_proc_data->m_addr = record->m_addr;
    	pre_proc_data->m_split_addr = record->m_split_addr;
    	pre_proc_data->m_addr_level = record->m_addr_level;
    	pre_proc_data->m_type = record->m_type;
    	pre_proc_data->m_source = record->m_source;
    	pre_proc_data->m_province = record->m_province;
    	pre_proc_data->m_city = record->m_city;
    	pre_proc_data->m_district = record->m_district;
    	pre_proc_data->m_status = record->m_status;

    	// ʶ���ӵ�����
    	ClassifyData(pre_proc_data->m_name, record->m_type_name,
    			pre_proc_data->m_accurate_keyword, resource_->cateCodeMap);

    	if (record->m_tel != "") {
    		split(record->m_tel, ";", pre_proc_data->m_tel_set, "");
    	}

    	///////////////////////////////////
    	// processed
    	///////////////////////////////////
    	// ��ַ�з�
    	string temp_correct_addr = "";
    	splitAddressCorrection(pre_proc_data->m_split_addr.c_str(),
    			temp_correct_addr);

    	getCorrectionAddressPart(temp_correct_addr.c_str(),
    			pre_proc_data->m_addrsim_xzqh_map,
    			pre_proc_data->m_addrsim_road_map,
    			pre_proc_data->m_addrsim_community_set,
    			pre_proc_data->m_addrsim_poi_set, pre_proc_data->m_addrsim_hn_set,
    			pre_proc_data->m_addrsim_home_set,
    			pre_proc_data->m_addrsim_floor_set);

    	// ��ȡ��ϸ����
    	if (pre_proc_data->m_accurate_keyword != "") {
    		std::vector<std::string> temp_accurate_type_vec;
    		if (split(pre_proc_data->m_accurate_keyword, "#",
    				temp_accurate_type_vec, "") == -1) {
    			return false;
    		}

    		if (temp_accurate_type_vec.size() == 4) {
    			pre_proc_data->m_accurate_type = temp_accurate_type_vec[1];
    			pre_proc_data->m_accurate_keyword = temp_accurate_type_vec[3];

    			if (pre_proc_data->m_accurate_keyword != "") {
    				// ͬ����滻
    				_map::const_iterator transIt = resource_->transWord.find(
    						pre_proc_data->m_accurate_keyword);
    				if (transIt != resource_->transWord.end()) {
    					pre_proc_data->m_accurate_keyword = transIt->second;
    				}
    			}
    		}
    	}

    	// ��ȡ����С��
    	if (pre_proc_data->m_type.size() == 6) {
    		pre_proc_data->m_main_type = pre_proc_data->m_type.substr(0, 2);
    		pre_proc_data->m_sub_type = pre_proc_data->m_type.substr(0, 4);
    	}

    	if (pre_proc_data->m_accurate_type.size() == 6) {
    		pre_proc_data->m_accurate_main_type =
    				pre_proc_data->m_accurate_type.substr(0, 2);
    		pre_proc_data->m_accurate_sub_type =
    				pre_proc_data->m_accurate_type.substr(0, 4);
    	}

    	//===================================================
    	//====================   ���ƴ��� ===================
    	// ȫ��ת���
    	// ��Ҫʶ��ĩβ�Ƿ��к�׺��������ʱ��ʹ������ȫ�Ǵ����name�ȽϷ���
    	pre_proc_data->m_name = sbc2dbc(pre_proc_data->m_name);

    	// ��������򵥵�Ԥ����󵱳�ԭʼ����ʹ��
    	pre_proc_data->m_format_name = pre_proc_data->m_name;

    	// ����ת����
    	pre_proc_data->m_format_name = tools_->m_trad2simp->func(
    			pre_proc_data->m_format_name);

    	// ��ĸ��дתСд
    	lower(pre_proc_data->m_format_name);

    	// ���Զ���С���������������ر���
    	if (pre_proc_data->m_source == "dingcanxiaomishu_hezuo"
    			&& pre_proc_data->m_format_name.find(" ") != string::npos) {
    		formatName(pre_proc_data->m_format_name);
    	}

    	/////////////////////////
    	// ��ȫ �����ξ� Ϊ ���������ξ�
    	// �Ϻ�վ  �Ϻ���վ(������վ �п��ܱ���ɱ�������վ�������Ҫ����)
    	if (pre_proc_data->m_main_type == "12" || pre_proc_data->m_main_type == "20"
    			|| pre_proc_data->m_main_type == "24") {
    		extendGov(pre_proc_data);
    	} else if (pre_proc_data->m_type == "271016") {
    		extendTrain(pre_proc_data);
    	}

    	pre_proc_data->m_del_punc_name = pre_proc_data->m_format_name;

    	// ȥ��������
    	removePunc(pre_proc_data->m_format_name, true);
    	removePunc(pre_proc_data->m_del_punc_name, false);

    	// ��������ת����������
    	transZhNum(pre_proc_data->m_format_name, resource_->number,
    			resource_->number2, 1);

    	transZhNum(pre_proc_data->m_del_punc_name, resource_->number,
    			resource_->number2, 0);

    	// �淶���ڡ�
    	if (pre_proc_data->m_main_type == "20"
    			|| pre_proc_data->m_main_type == "24") {
    		formatNumName(pre_proc_data->m_format_name);
    	}

    	// ȥ�����ż����ڲ�������
    	removeBracket(pre_proc_data->m_format_name,
    			pre_proc_data->m_raw_bracket_vec);

    	// ���Һ�׺���ͣ�����soso_type
    	findTailType(pre_proc_data->m_format_name, pre_proc_data->m_soso_type,
    			resource_->nameTailMap);

    	if (pre_proc_data->m_soso_type.size() == 6) {
    		pre_proc_data->m_soso_main_type = pre_proc_data->m_soso_type.substr(0,
    				2);
    		pre_proc_data->m_soso_sub_type = pre_proc_data->m_soso_type.substr(0,
    				4);
    	}

    	// ����
    	if (pre_proc_data->m_format_name == "����") {
    		pre_proc_data->m_format_name = pre_proc_data->m_del_punc_name;
    		pre_proc_data->m_raw_bracket_vec.clear();
    	}

    	// ȥ�����ƺ����**���׺
    	findAreaTail(pre_proc_data, resource_);

    	// ȥ��**���׺���ٶ�û���ҵ���׺���͵������Һ�׺����
    	if (pre_proc_data->m_soso_type == "") {
    		findTailType(pre_proc_data->m_format_name, pre_proc_data->m_soso_type,
    				resource_->nameTailMap);
    	}

    	// ��ȡ�����е�������׺
    	if (pre_proc_data->m_main_type == "26") {
    		getGovTail(pre_proc_data->m_format_name, pre_proc_data->m_gov_tail,
    				resource_->typeword);
    	}

    	// ��ȡ�����е��Ե��β�Ŀ��ܵķֵ���Ϣ
    	findFendian(pre_proc_data, resource_);

    	// ȥ�����ʹ����ƺ���������֣����
    	formatTypeName(pre_proc_data, resource_);

    	// ���Ʒִ�
    	if (len(pre_proc_data->m_format_name) > 3) {
    		tools_->m_wordsplit->split(pre_proc_data->m_format_name,
    				pre_proc_data->m_name_vec);
    	} else {
    		pre_proc_data->m_name_vec.push_back(pre_proc_data->m_format_name);
    	}

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

    	//	�ϲ�������
    	vector<string> tempVec;
    	if (mergeWord(pre_proc_data->m_name_vec, tempVec, *resource_) == -1)
    		return false;
    	pre_proc_data->m_name_vec = tempVec;

    	if (mergeWord(pre_proc_data->m_bracket_vec, tempVec) == -1)
    		return false;
    	pre_proc_data->m_bracket_vec = tempVec;

    	if (mergeWord(pre_proc_data->m_accurate_kw_vec, tempVec) == -1)
    		return false;
    	pre_proc_data->m_accurate_kw_vec = tempVec;

    	// ��ȡ�����з����ʹ�����
    	int count = 0;
    	countNotType(pre_proc_data->m_name_vec, count, resource_->typeword);

    	// ͬ����滻
    	transWord(pre_proc_data->m_name_vec, resource_->transWord);
    	transWord(pre_proc_data->m_bracket_vec, resource_->transWord);
    	transWord(pre_proc_data->m_accurate_kw_vec, resource_->transWord);

    	// ��ȡ���ݵ���������
    	if ((pre_proc_data->m_main_type == "12"
    			|| pre_proc_data->m_soso_main_type == "12")
    			&& pre_proc_data->m_soso_main_type != "11") {
    		findGovLevel(pre_proc_data, resource_);
    	}

    	// ȥ������������
    	filteDistrict(pre_proc_data, resource_, count);

    	// ��ȡ��������
    	findObjName(pre_proc_data, resource_);

    	// ����ֻΪ���ʹ�ʱ���������ڵ�������ӵ��淶������
    	addContentForTWName(pre_proc_data, resource_->typeword);

    	// �ر�������Ҫ�������ڵ�������ӵ��淶������
    	if (startswith(pre_proc_data->m_soso_type, "2612")
    			|| (pre_proc_data->m_soso_type == ""
    					&& startswith(pre_proc_data->m_type, "2612"))) {
    		pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
    				pre_proc_data->m_bracket_vec.begin(),
    				pre_proc_data->m_bracket_vec.end());
    		pre_proc_data->m_bracket_vec.clear();
    	}

    	if (startswith(pre_proc_data->m_soso_type, "2410")
    			|| (pre_proc_data->m_soso_type == ""
    					&& startswith(pre_proc_data->m_type, "2410"))) {
    		int in_size = pre_proc_data->m_bracket_vec.size();
    		if (in_size > 0
    				&& endswith(pre_proc_data->m_bracket_vec[in_size - 1], "ѧԺ")) {
    			pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
    					pre_proc_data->m_bracket_vec.begin(),
    					pre_proc_data->m_bracket_vec.end());
    			pre_proc_data->m_bracket_vec.clear();
    		}
    	}

    	// �淶������
    	transNum(pre_proc_data->m_name_vec);
    	transNum(pre_proc_data->m_bracket_vec);

    	// �ϲ�����,��ĸ����
    	mergeNumAlp(pre_proc_data, resource_);

    	// �ϲ���β���ر���
    	mergeTailSglWord(pre_proc_data);

    	// ��ȡ �� �����Ϣ
    	filteDoor2(pre_proc_data->m_raw_bracket_vec, pre_proc_data->m_door_set,
    			*resource_);
    	filteDoorInBrac(pre_proc_data->m_bracket_vec, pre_proc_data->m_door_set,
    			*resource_);
    	filteDoor(pre_proc_data->m_name_vec, pre_proc_data->m_door_set, *resource_);

    	// С�������ʵĹ淶
    	if (pre_proc_data->m_main_type == "28") {
    		vector<string>::iterator it = pre_proc_data->m_name_vec.begin();
    		for (; it != pre_proc_data->m_name_vec.end(); ++it) {
    			transZhNum(*it, resource_->number, resource_->number2, 0);
    		}
    	}

    	// ��ȡpoi��ͬ��������
    	filte(pre_proc_data->m_name_vec, tempVec, pre_proc_data->m_dpmark_set,
    			resource_->DPMark);
    	if (tempVec.size() > 0) {
    		pre_proc_data->m_name_vec = tempVec;
    	} else {
    		pre_proc_data->m_dpmark_set.clear();
    	}
    	if (pre_proc_data->m_bracket_vec.size() < 2) {
    		filte(pre_proc_data->m_bracket_vec, tempVec,
    				pre_proc_data->m_dpmark_set, resource_->DPMark);
    		pre_proc_data->m_bracket_vec = tempVec;
    	}

    	//����������ص�����
    	transZhNum(pre_proc_data->m_dpmark_set, resource_->number, resource_->number2,
    			0);

    	// ���˳�����Ӣ���ַ�
    	filteLongAlp(pre_proc_data->m_name_vec);

    	// ����00�ʱ��еĴ�
    	filte(pre_proc_data->m_name_vec, resource_);

    	// ��ȡ��֧������
    	if (pre_proc_data->m_main_type == "28") {
    		findDizhi(pre_proc_data, resource_);
    	}

    	//	�ڶ��� �ϲ�������
    	if (mergeWord(pre_proc_data->m_name_vec, tempVec, *resource_) == -1)
    		return false;
    	pre_proc_data->m_name_vec = tempVec;

    	if (mergeWord(pre_proc_data->m_bracket_vec, tempVec) == -1)
    		return false;
    	pre_proc_data->m_bracket_vec = tempVec;

    	if (mergeWord(pre_proc_data->m_accurate_kw_vec, tempVec) == -1)
    		return false;
    	pre_proc_data->m_accurate_kw_vec = tempVec;

    	// ʶ�𲻿�ȱʧ��
    	findUnlackWord(pre_proc_data, resource_);

    	// ����bigram����
    	vec2str(pre_proc_data->m_name_vec, pre_proc_data->m_bigram_name);
    	if (startswith(pre_proc_data->m_type, "26")) {
    		pre_proc_data->m_bigram_name = pre_proc_data->m_del_punc_name;
    	} else if (pre_proc_data->m_bigram_name == "") {
    		pre_proc_data->m_bigram_name = pre_proc_data->m_del_punc_name;
    	} else if (resource_->xzqh.find(pre_proc_data->m_bigram_name)
    			!= resource_->xzqh.end()) {
    		pre_proc_data->m_bigram_name = pre_proc_data->m_del_punc_name;
    	}

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

    	// ����gridʱ��Ҫ�ķִ�����
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
    		// ��û�зִʵ�ʱ��Ϊ�˼����ٻص�������Ҫ��bigram�з�
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

    	//========================================================================
    	// ����Ǵ���õ�Ԥ��������
    	pre_proc_data->m_is_null = false;

    	          
        return true;
    };
};

REGINTER_RULE( FullPre )