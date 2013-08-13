/*
 * prepare.cpp
 *
 *  Created on: 2012-10-10
 *      Author: stefanyang
 */

#include "prepare.h"
#include <unistd.h>
#include <pthread.h>
#include <cmath>
#include "common.h"
#include "rules.h"
#include "init.h"
#include "utils/computeAddressSimilarity.h"
#include "utils/classify_data/classify_data.h"
#include "utils/classify_data/string_process.h"

//===============================================
// 函数实现
//===============================================
bool isBadPreProcData(PreProcData *pre_proc_data) {
	if (pre_proc_data->m_is_null || pre_proc_data->m_id == 0
			|| pre_proc_data->m_name == "") {
		return true;
	}
	return false;
}

bool isBadCsvIndexCfg(const CsvIndexCfg *csv_index_cfg) {
	if (csv_index_cfg->m_delim == "") {
		return true;
	}

	return false;
}

//=================================================
// 生成record
//=================================================
// 根据配置文件生成预处理的输入数据结构
// 如果需要的属性对应的索引为负数，则认为该字段属性未进行配置，使用默认值
void getRecFromCsv(const vector<string> &vec_data, Record *record,
		const CsvIndexCfg *csv_index_cfg) {
	record->clear();

	if (vec_data.size() < csv_index_cfg->m_max_index + 1) {
		record->m_is_null = true;
		return;
	}

	if (csv_index_cfg->m_id >= 0) {
		record->m_id = strtoull(vec_data[csv_index_cfg->m_id].c_str(), NULL, 0);
	}

	if (csv_index_cfg->m_lat >= 0) {
		record->m_lat = Str2Double(vec_data[csv_index_cfg->m_lat]);
	}

	if (csv_index_cfg->m_lng >= 0) {
		record->m_lng = Str2Double(vec_data[csv_index_cfg->m_lng]);
	}

	if (csv_index_cfg->m_name >= 0) {
		record->m_name = vec_data[csv_index_cfg->m_name];
	}

	if (csv_index_cfg->m_addr >= 0) {
		record->m_addr = vec_data[csv_index_cfg->m_addr];
	}

	if (csv_index_cfg->m_split_addr >= 0) {
		record->m_split_addr = vec_data[csv_index_cfg->m_split_addr];
	}

	if (csv_index_cfg->m_addr_level >= 0) {
		record->m_addr_level = atoi(
				vec_data[csv_index_cfg->m_addr_level].c_str());
	}

	if (csv_index_cfg->m_type >= 0) {
		record->m_type = vec_data[csv_index_cfg->m_type];
	}

	if (csv_index_cfg->m_type_name >= 0) {
		record->m_type_name = vec_data[csv_index_cfg->m_type_name];
	}

	if (csv_index_cfg->m_source >= 0) {
		record->m_source = vec_data[csv_index_cfg->m_source];
	}

	if (csv_index_cfg->m_province >= 0) {
		record->m_province = vec_data[csv_index_cfg->m_province];
	}

	if (csv_index_cfg->m_city >= 0) {
		record->m_city = vec_data[csv_index_cfg->m_city];
	}

	if (csv_index_cfg->m_district >= 0) {
		record->m_district = vec_data[csv_index_cfg->m_district];
	}

	if (csv_index_cfg->m_accurate_type >= 0) {
		record->m_accurate_type = vec_data[csv_index_cfg->m_accurate_type];
	}

	if (csv_index_cfg->m_tel >= 0) {
		record->m_tel = vec_data[csv_index_cfg->m_tel];
	}

	if (csv_index_cfg->m_status >= 0) {
		if (vec_data[csv_index_cfg->m_status] == "0"
				|| vec_data[csv_index_cfg->m_status] == "1"
				|| vec_data[csv_index_cfg->m_status] == "2") {
			record->m_status = 0;
		} else if (vec_data[csv_index_cfg->m_status] == "16") {
			record->m_status = 1;
		} else {
			record->m_status = 0;
		}
	}

	record->m_is_null = false;
}

void getRecFromCsv(const string &str_data, Record *record,
		const CsvIndexCfg *csv_index_cfg) {
	vector<string> temp_vec;
	split(str_data, csv_index_cfg->m_delim, temp_vec, "");
	getRecFromCsv(temp_vec, record, csv_index_cfg);
}

//=================================================
// 生成预处理结果
//=================================================
void makePreProcFromRec(const Record *record, PreProcData *pre_proc_data,
		const Resource *resource, const Tools *tools) {
	pre_proc_data->clear();
	if (record->m_is_null) {
		return;
	}

	// 保存原始信息到预处理结果中
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

	// 识别子点类型
	ClassifyData(pre_proc_data->m_name, record->m_type_name,
			pre_proc_data->m_accurate_keyword, resource->cateCodeMap);

	if (record->m_tel != "") {
		split(record->m_tel, ";", pre_proc_data->m_tel_set, "");
	}

	///////////////////////////////////
	// processed
	///////////////////////////////////
	// 地址切分
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
   
	// 获取精细分类
	if (pre_proc_data->m_accurate_keyword != "") {
		std::vector<std::string> temp_accurate_type_vec;
		if (split(pre_proc_data->m_accurate_keyword, "#",
				temp_accurate_type_vec, "") == -1) {
			return;
		}

		if (temp_accurate_type_vec.size() == 4) {
			pre_proc_data->m_accurate_type = temp_accurate_type_vec[1];
			pre_proc_data->m_accurate_keyword = temp_accurate_type_vec[3];

			if (pre_proc_data->m_accurate_keyword != "") {
				// 同义词替换
				_map::const_iterator transIt = resource->transWord.find(
						pre_proc_data->m_accurate_keyword);
				if (transIt != resource->transWord.end()) {
					pre_proc_data->m_accurate_keyword = transIt->second;
				}
			}
		}
	}

	// 获取大中小类
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
	//====================   名称处理 ===================
	// 全角转半角
	// 需要识别末尾是否有后缀括号内容时，使用做过全角处理的name比较方便
	pre_proc_data->m_name = sbc2dbc(pre_proc_data->m_name);

	// 名称做完简单的预处理后当成原始名称使用
	pre_proc_data->m_format_name = pre_proc_data->m_name;

	// 繁体转简体
	pre_proc_data->m_format_name = tools->m_trad2simp->func(
			pre_proc_data->m_format_name);

	// 字母大写转小写
	lower(pre_proc_data->m_format_name);

	// 正对订餐小秘书名称特征的特别处理
	if (pre_proc_data->m_source == "dingcanxiaomishu_hezuo"
			&& pre_proc_data->m_format_name.find(" ") != string::npos) {
		formatName(pre_proc_data->m_format_name);
	}

	/////////////////////////
	// 补全 市旅游局 为 北京市旅游局
	// 上海站  上海火车站(北京西站 有可能被替成北京西火车站，这个需要处理)
	if (pre_proc_data->m_main_type == "12" || pre_proc_data->m_main_type == "20"
			|| pre_proc_data->m_main_type == "24") {
		extendGov(pre_proc_data);
	} else if (pre_proc_data->m_type == "271016") {
		extendTrain(pre_proc_data);
	}

	pre_proc_data->m_del_punc_name = pre_proc_data->m_format_name;

	// 去掉标点符号
	removePunc(pre_proc_data->m_format_name, true);
	removePunc(pre_proc_data->m_del_punc_name, false);

	// 中文数字转阿拉伯数字
	transZhNum(pre_proc_data->m_format_name, resource->number,
			resource->number2, 1);

	transZhNum(pre_proc_data->m_del_punc_name, resource->number,
			resource->number2, 0);

	// 规范“第”
	if (pre_proc_data->m_main_type == "20"
			|| pre_proc_data->m_main_type == "24") {
		formatNumName(pre_proc_data->m_format_name);
	}

	cout << pre_proc_data->m_format_name << endl;

	// 去掉括号及其内部的内容
	removeBracket(pre_proc_data->m_format_name,
			pre_proc_data->m_raw_bracket_vec);

	// 查找后缀类型，即：soso_type
	findTailType(pre_proc_data->m_format_name, pre_proc_data->m_soso_type,
			resource->nameTailMap);

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
	findAreaTail(pre_proc_data, resource);

	// 去掉**店后缀后，再对没有找到后缀类型的数据找后缀类型
	if (pre_proc_data->m_soso_type == "") {
		findTailType(pre_proc_data->m_format_name, pre_proc_data->m_soso_type,
				resource->nameTailMap);
	}

	// 获取地名中的行政后缀
	if (pre_proc_data->m_main_type == "26") {
		getGovTail(pre_proc_data->m_format_name, pre_proc_data->m_gov_tail,
				resource->typeword);
	}

	// 获取括号中的以店结尾的可能的分店信息
	findFendian(pre_proc_data, resource);

	// 去掉类型词名称后面的无用字，如店
	formatTypeName(pre_proc_data, resource);

	// 名称分词
	if (len(pre_proc_data->m_format_name) > 3) {
		tools->m_wordsplit->split(pre_proc_data->m_format_name,
				pre_proc_data->m_name_vec);
	} else {
		pre_proc_data->m_name_vec.push_back(pre_proc_data->m_format_name);
	}

	transWord(pre_proc_data->m_raw_bracket_vec, resource->transWord);

	vector<string>::iterator it = pre_proc_data->m_raw_bracket_vec.begin();
	vector<string> temp;
	for (; it != pre_proc_data->m_raw_bracket_vec.end(); ++it) {
		if (len(*it) > 3) {
			tools->m_wordsplit->split(*it, temp);
		} else {
			temp.push_back(*it);
		}
		pre_proc_data->m_bracket_vec.insert(pre_proc_data->m_bracket_vec.end(),
				temp.begin(), temp.end());
	}

	if (len(pre_proc_data->m_accurate_keyword) > 3) {
		tools->m_wordsplit->split(pre_proc_data->m_accurate_keyword,
				pre_proc_data->m_accurate_kw_vec);
	} else {
		pre_proc_data->m_accurate_kw_vec.push_back(
				pre_proc_data->m_accurate_keyword);
	}

	// 增加前缀
	addPrefix(pre_proc_data->m_name_vec, resource->needPrefix);
	addPrefix(pre_proc_data->m_bracket_vec, resource->needPrefix);

	// 增加量词
	// 处理空港科技大厦a 中的a，扩展为a座
	addSuffix(pre_proc_data);

	// 处理容易引起同义词歧义的词
	transSpecWord(pre_proc_data);

	// 处理结尾的店
	if (pre_proc_data->m_name_vec.size() > 2) {
		it = pre_proc_data->m_name_vec.end() - 1;
		if (*it == "店") {
			pre_proc_data->m_name_vec.erase(it);
		}
	}

	if (pre_proc_data->m_bracket_vec.size() > 1) {
		it = pre_proc_data->m_bracket_vec.end() - 1;
		if (*it == "店") {
			pre_proc_data->m_bracket_vec.erase(it);
		}
	}

	//	合并单个字
	vector<string> tempVec;
	if (mergeWord(pre_proc_data->m_name_vec, tempVec, *resource) == -1)
		return;
	pre_proc_data->m_name_vec = tempVec;

	if (mergeWord(pre_proc_data->m_bracket_vec, tempVec) == -1)
		return;
	pre_proc_data->m_bracket_vec = tempVec;

	if (mergeWord(pre_proc_data->m_accurate_kw_vec, tempVec) == -1)
		return;
	pre_proc_data->m_accurate_kw_vec = tempVec;

	// 获取名称中非类型词数量
	int count = 0;
	countNotType(pre_proc_data->m_name_vec, count, resource->typeword);

	// 同义词替换
	transWord(pre_proc_data->m_name_vec, resource->transWord);
	transWord(pre_proc_data->m_bracket_vec, resource->transWord);
	transWord(pre_proc_data->m_accurate_kw_vec, resource->transWord);

	// 获取数据的行政级别
	if ((pre_proc_data->m_main_type == "12"
			|| pre_proc_data->m_soso_main_type == "12")
			&& pre_proc_data->m_soso_main_type != "11") {
		findGovLevel(pre_proc_data, resource);
	}

	// 去掉行政区划词
	filteDistrict(pre_proc_data, resource, count);

	// 获取连锁店名
	findObjName(pre_proc_data, resource);

	// 名称只为类型词时，将括号内的内容添加到规范名称中
	addContentForTWName(pre_proc_data, resource->typeword);

	// 特别类型需要将括号内的内容添加到规范名称中
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
				&& endswith(pre_proc_data->m_bracket_vec[in_size - 1], "学院")) {
			pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
					pre_proc_data->m_bracket_vec.begin(),
					pre_proc_data->m_bracket_vec.end());
			pre_proc_data->m_bracket_vec.clear();
		}
	}

	// 规范化数字
	transNum(pre_proc_data->m_name_vec);
	transNum(pre_proc_data->m_bracket_vec);

	// 合并数字,字母内容
	mergeNumAlp(pre_proc_data, resource);

	// 合并结尾的特别单字
	mergeTailSglWord(pre_proc_data);

	// 获取 门 相关信息
	filteDoor2(pre_proc_data->m_raw_bracket_vec, pre_proc_data->m_door_set,
			*resource);
	filteDoorInBrac(pre_proc_data->m_bracket_vec, pre_proc_data->m_door_set,
			*resource);
	filteDoor(pre_proc_data->m_name_vec, pre_proc_data->m_door_set, *resource);

	// 小区数量词的规范
	if (pre_proc_data->m_main_type == "28") {
		vector<string>::iterator it = pre_proc_data->m_name_vec.begin();
		for (; it != pre_proc_data->m_name_vec.end(); ++it) {
			transZhNum(*it, resource->number, resource->number2, 0);
		}
	}

	// 获取poi不同的特征词
	filte(pre_proc_data->m_name_vec, tempVec, pre_proc_data->m_dpmark_set,
			resource->DPMark);
	if (tempVec.size() > 0) {
		pre_proc_data->m_name_vec = tempVec;
	} else {
		pre_proc_data->m_dpmark_set.clear();
	}
	if (pre_proc_data->m_bracket_vec.size() < 2) {
		filte(pre_proc_data->m_bracket_vec, tempVec,
				pre_proc_data->m_dpmark_set, resource->DPMark);
		pre_proc_data->m_bracket_vec = tempVec;
	}

	//处理数字相关的内容
	transZhNum(pre_proc_data->m_dpmark_set, resource->number, resource->number2,
			0);

	// 过滤长串的英文字符
	filteLongAlp(pre_proc_data->m_name_vec);

	// 过滤00词表中的词
	filte(pre_proc_data->m_name_vec, resource);

	// 获取地支的数据
	if (pre_proc_data->m_main_type == "28") {
		findDizhi(pre_proc_data, resource);
	}

	//	第二次 合并单个字
	if (mergeWord(pre_proc_data->m_name_vec, tempVec, *resource) == -1)
		return;
	pre_proc_data->m_name_vec = tempVec;

	if (mergeWord(pre_proc_data->m_bracket_vec, tempVec) == -1)
		return;
	pre_proc_data->m_bracket_vec = tempVec;

	if (mergeWord(pre_proc_data->m_accurate_kw_vec, tempVec) == -1)
		return;
	pre_proc_data->m_accurate_kw_vec = tempVec;

	// 识别不可缺失词
	findUnlackWord(pre_proc_data, resource);

	// 生成bigram名称
	vec2str(pre_proc_data->m_name_vec, pre_proc_data->m_bigram_name);
	if (startswith(pre_proc_data->m_type, "26")) {
		pre_proc_data->m_bigram_name = pre_proc_data->m_del_punc_name;
	} else if (pre_proc_data->m_bigram_name == "") {
		pre_proc_data->m_bigram_name = pre_proc_data->m_del_punc_name;
	} else if (resource->xzqh.find(pre_proc_data->m_bigram_name)
			!= resource->xzqh.end()) {
		pre_proc_data->m_bigram_name = pre_proc_data->m_del_punc_name;
	}

	// 连锁店特别处理
	if (pre_proc_data->m_obj_name_set.size() != 0
			|| pre_proc_data->m_type == "251100") {
		pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
				pre_proc_data->m_bracket_vec.begin(),
				pre_proc_data->m_bracket_vec.end());

		if (pre_proc_data->m_bracket_vec.size() > 0) {
			_mapSet::const_iterator it = resource->typeword.find("01");
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
		if (resource->directionSet.find(
				pre_proc_data->m_bracket_vec[pre_proc_data->m_bracket_vec.size()
						- 1]) == resource->directionSet.end()
				&& resource->road.find(
						pre_proc_data->m_raw_bracket_vec[pre_proc_data->m_raw_bracket_vec.size()
								- 1]) == resource->road.end())
			pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
					pre_proc_data->m_bracket_vec.begin(),
					pre_proc_data->m_bracket_vec.end());
		pre_proc_data->m_bracket_vec.clear();
	}

	// 别名标记
	_mapStrIntSet::const_iterator sm_it = resource->smNameMap.find(
			pre_proc_data->m_name);
	if (sm_it != resource->smNameMap.end()) {
		pre_proc_data->m_smname_group_set = sm_it->second;
	}

	// 带数字的名称的处理
	filteDoor(pre_proc_data->m_num_name_set, pre_proc_data->m_door_set);

	// 规范门类信息
	formatDoorSet(pre_proc_data->m_door_set);

	// 处理分词就在一起的数量词
	findNumWord(pre_proc_data, resource);

	//处理 门 数字相关的内容
	transZhNum(pre_proc_data->m_door_set, resource->number, resource->number2,
			0);

	// 不可缺失词识别
	//findUnlackWord(pre_proc_data, resource->unlackword);

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
		strToPY(pre_proc_data->m_format_name, temp_py_vec, resource->pinyin);
		pre_proc_data->m_grid_name_set.insert(temp_py_vec.begin(),
				temp_py_vec.end());
	}

	if (pre_proc_data->m_name.size() <= 8) {
		temp_py_vec.clear();
		strToPY(pre_proc_data->m_name, temp_py_vec, resource->pinyin);
		pre_proc_data->m_grid_name_set.insert(temp_py_vec.begin(),
				temp_py_vec.end());
	}

	if (pre_proc_data->m_bigram_name.size() <= 8) {
		temp_py_vec.clear();
		strToPY(pre_proc_data->m_bigram_name, temp_py_vec, resource->pinyin);
		pre_proc_data->m_grid_name_set.insert(temp_py_vec.begin(),
				temp_py_vec.end());
	}

	//========================================================================
	// 标记是处理好的预处理数据
	pre_proc_data->m_is_null = false;
}

void makePreProcFromCsv(const string &str_data, PreProcData *pre_proc_data,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg) {
	Record *record = new Record();
	getRecFromCsv(str_data, record, csv_index_cfg);
	makePreProcFromRec(record, pre_proc_data, resource, tools);
	delete record;
}

void makePreProcFromCsv(const string &str_data, PreProcData *pre_proc_data,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg, Record *record) {
	getRecFromCsv(str_data, record, csv_index_cfg);
	makePreProcFromRec(record, pre_proc_data, resource, tools);
}

//=================================================
// 从文件生成预处理结果
//=================================================
void _initMapFromCsvFileSgl(ifstream &fin, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg) {
	std::string line("");
	Record *record = new Record();
	PreProcData *pre_proc_data = new PreProcData();

	while (true) {
		if (!getline(fin, line)) {
			break;
		}

		makePreProcFromCsv(line, pre_proc_data, resource, tools, csv_index_cfg,
				record);
		if (isBadPreProcData(pre_proc_data)) {
            std::cerr<<"errori in line "<<line<<std::endl;
			continue;
		} else {
			pre_proc_map->insert(
					make_pair(pre_proc_data->m_id, *pre_proc_data));
		}

		if (csv_index_cfg->m_debug) {
			print(pre_proc_data);
		}
	}

	delete record;
	delete pre_proc_data;
}

void _initMapFromCsvFileMulti(ifstream &fin, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg, pthread_mutex_t &prepare_read_mutex,
		pthread_mutex_t &prepare_setmap_mutex) {
	Record *record = new Record();
	PreProcData *pre_proc_data = new PreProcData();

	string line("");
	while (true) {
		pthread_mutex_lock(&prepare_read_mutex);
		if (!getline(fin, line)) {
			pthread_mutex_unlock(&prepare_read_mutex);
			break;
		}
		pthread_mutex_unlock(&prepare_read_mutex);

		makePreProcFromCsv(line, pre_proc_data, resource, tools, csv_index_cfg,
				record);
		if (isBadPreProcData(pre_proc_data)) {
			continue;
		} else {
			pthread_mutex_lock(&prepare_setmap_mutex);
			pre_proc_map->insert(
					make_pair(pre_proc_data->m_id, *pre_proc_data));
			if (csv_index_cfg->m_debug) {
				print(pre_proc_data);
			}
			pthread_mutex_unlock(&prepare_setmap_mutex);
		}
	}

	delete record;
	delete pre_proc_data;
}

void *initMapFromCsvFileWraper(void *param) {
	PrepareFromCsvParam *prepare_param = (PrepareFromCsvParam *) param;
	_initMapFromCsvFileMulti(*(prepare_param->fin), prepare_param->pre_proc_map,
			prepare_param->resource, prepare_param->tools,
			prepare_param->csv_index_cfg, *(prepare_param->prepare_read_mutex),
			*(prepare_param->prepare_setmap_mutex));
}

void initMapFromCsvFile(const char *path, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg) {
	if (isBadCsvIndexCfg(csv_index_cfg)) {
		cout << "[Err]: csvIndexCfg has some err" << endl;
	}

	std::ifstream fin(path, std::ios::in);
	if (fin.fail()) {
		cout << "[ERR]: File open error in " << __FILE__ << endl;
		return;
	}

	//time_t tbegin = time(NULL);

	if (csv_index_cfg->m_thread_num == 1) {
		_initMapFromCsvFileSgl(fin, pre_proc_map, resource, tools,
				csv_index_cfg);
	} else if (csv_index_cfg->m_thread_num > 1) {
		// 多线程锁定义
		pthread_mutex_t prepare_read_mutex;
		pthread_mutex_t prepare_setmap_mutex;
		pthread_mutex_init(&prepare_read_mutex, NULL);
		pthread_mutex_init(&prepare_setmap_mutex, NULL);

		// creat thread
		vector<pthread_t> tid_vec;
		vector<PrepareFromCsvParam *> prepare_param_vec;
		for (int i = 0; i < csv_index_cfg->m_thread_num; ++i) {
			pthread_t tid;
			int error;

			// 参数初始化
			PrepareFromCsvParam *prepare_param = new PrepareFromCsvParam();
			prepare_param->fin = &fin;
			prepare_param->pre_proc_map = pre_proc_map;
			prepare_param->resource = resource;
			prepare_param->csv_index_cfg = csv_index_cfg;
			prepare_param->prepare_read_mutex = &prepare_read_mutex;
			prepare_param->prepare_setmap_mutex = &prepare_setmap_mutex;
			prepare_param->tools = tools;

			error = pthread_create(&tid, NULL, initMapFromCsvFileWraper,
					(void *) prepare_param);
			if (error) {
				cout << "[Err]: phread is not created..." << endl;
				return;
			}
			tid_vec.push_back(tid);

			prepare_param_vec.push_back(prepare_param);
		}

		for (int i = 0; i < csv_index_cfg->m_thread_num; ++i) {
			pthread_join(tid_vec[i], NULL);
		}

		// 合并PreProcMap
		for (int i = 0; i < csv_index_cfg->m_thread_num; ++i) {
			delete prepare_param_vec[i];
		}
	} else {
		cout << "thread num is err:" << csv_index_cfg->m_thread_num
				<< ". need >= 1." << endl;
	}

	fin.close();
}

//=================================================
// 统计函数
//=================================================
void statIdf(const PreProcData &pre_proc_data,
		map<string, double> &stat_idf_map) {
	vector<string>::const_iterator it = pre_proc_data.m_name_vec.begin();
	for (; it != pre_proc_data.m_name_vec.end(); ++it) {
		if (stat_idf_map.find(*it) != stat_idf_map.end()) {
			stat_idf_map[*it] += 1;
		} else {
			stat_idf_map[*it] = 1;
		}
	}
}

void statTel(const PreProcData &pre_proc_data, map<string, int> &stat_tel_map) {
	set<string>::const_iterator it = pre_proc_data.m_tel_set.begin();
	for (; it != pre_proc_data.m_tel_set.end(); ++it) {
		if (stat_tel_map.find(*it) != stat_tel_map.end()) {
			stat_tel_map[*it] += 1;
		} else {
			stat_tel_map[*it] = 1;
		}
	}
}

void makeIdf(unsigned long number, map<string, double> &stat_idf_map,
		double &max_idf, double &min_idf) {
	_mapStrDblit it = stat_idf_map.begin();
	for (; it != stat_idf_map.end(); ++it) {
		it->second = log10(number / (it->second));
		if (it->second > max_idf) {
			max_idf = it->second;
		}
		if (it->second < min_idf) {
			min_idf = it->second;
		}
	}
	cout << "idf: " << max_idf << "\t" << min_idf << endl;
}

void makeUselessTel(map<string, int> &stat_tel_map, int threshold) {
	map<string, int>::iterator it = stat_tel_map.begin();
	while (it != stat_tel_map.end()) {
		if (it->second < threshold) {
			stat_tel_map.erase(it);
		}
		++it;
	}

}

//=================================================
// 周边检索相关函数
//=================================================
int getLatKey(double lat) {
	return int(lat * 500) / 15;
}

int getLngKey(double lng) {
	return int(lng * 500) / 10;
}

// 将数据插入到周边grid结构中
void setGridMap(const GridKey *key, const unsigned long long value,
		GridMap *grid_map) {
	GridMap::iterator it = grid_map->find(*key);
	if (it != grid_map->end()) {
		it->second.insert(value);
	} else {
		set<unsigned long long> temp_set;
		temp_set.insert(value);
		grid_map->insert(make_pair(*key, temp_set));
	}
}

void setGridMap(const PreProcData *pre_proc_data, const Resource *resource,
		GridMap *grid_map) {
	GridKey *temp_key = new GridKey();
	temp_key->lat = getLatKey(pre_proc_data->m_lat);
	temp_key->lng = getLngKey(pre_proc_data->m_lng);

	set<string>::const_iterator wordsit =
			pre_proc_data->m_grid_name_set.begin();
	for (; wordsit != pre_proc_data->m_grid_name_set.end(); ++wordsit) {
		if (!startswith(pre_proc_data->m_type, "27")
				&& resource->delBigram.find(*wordsit)
						!= resource->delBigram.end()) {
			continue;
		}
		temp_key->keyword = *wordsit;
		setGridMap(temp_key, pre_proc_data->m_id, grid_map);
	}

	delete temp_key;
}

void initGridMap(const PreProcMap *pre_proc_map, const Resource *resource,
		GridMap *grid_map) {
	PreProcMap::const_iterator it = pre_proc_map->begin();
	for (; it != pre_proc_map->end(); ++it) {
		setGridMap(&(it->second), resource, grid_map);
	}
}

// 根据key获取周边数据
void getAroundGridMap(const PreProcData &pre_proc_data, const GridKey &key,
		const GridMap &grid_map, set<unsigned long long> &result_set,
		int level) {
	GridKey *temp_key = new GridKey();

	temp_key->lat = key.lat - level;
	temp_key->lng = key.lng - level;
	temp_key->keyword = key.keyword;

	set<unsigned long long> temp_set;

	GridMap::const_iterator it;
	for (int i = 0; i <= 2 * level; i++) {
		temp_key->lng = key.lng - level;
		for (int j = 0; j <= 2 * level; j++) {
			it = grid_map.find(*temp_key);
			if (it != grid_map.end()) {
				temp_set.insert(it->second.begin(), it->second.end());
			}
			temp_key->lng += 1;
		}
		temp_key->lat += 1;
	}

	if (temp_set.size() < 500 || pre_proc_data.m_del_punc_name_len <= 4) {
		result_set.insert(temp_set.begin(), temp_set.end());
	}

	delete temp_key;
}

void getAroundGridMap(const PreProcData &pre_proc_data, const GridMap &grid_map,
		set<unsigned long long> &result_set, int level) {
	result_set.clear();

	GridKey *temp_key = new GridKey();
	temp_key->lat = getLatKey(pre_proc_data.m_lat);
	temp_key->lng = getLngKey(pre_proc_data.m_lng);

	set<string>::const_iterator wordsit = pre_proc_data.m_grid_name_set.begin();
	for (; wordsit != pre_proc_data.m_grid_name_set.end(); ++wordsit) {
		if (*wordsit != "") {
			temp_key->keyword = *wordsit;
			getAroundGridMap(pre_proc_data, *temp_key, grid_map, result_set,
					level);
		}
	}

	delete temp_key;
}

