/*
 * sim.cpp
 *
 *  Created on: 2012-10-26
 *      Author: stefanyang
 */

#include "sim.h"
#include <algorithm>
#include <cmath>
#include "rules.h"
#include "tools.h"
#include "init.h"
#include "prepare.h"
#include "utils/MapTools.h"
#include "utils/computeAddressSimilarity.h"
#include "utils/classify_data/classify_data.h"
#include "utils/classify_data/string_process.h"

void loadResource(const PoiSimCfg *poi_sim_cfg, Resource *resource) {
	string path = poi_sim_cfg->m_source_dir;
	string tempPath("");
	_vector tempVec;

	init_group_score(resource->m_group_score, resource->m_sigle_score);

	// pinyin
	tempPath = path + std::string("/pinyin.dic");
	if (load(tempPath, resource->pinyin, "\t") == -1) {
		std::cout << "pinyin init failed." << std::endl;
		return;
	}

	// some special words that need filte
	tempVec.clear();
	tempVec.push_back(path + std::string("/measure.dic"));
	tempVec.push_back(path + std::string("/states.dic"));
	if (load(tempVec, resource->filter) == -1) {
		std::cout << "filter init failed" << std::endl;
		return;
	}

	// measure
	tempVec.clear();
	tempVec.push_back(path + std::string("/measure.dic"));
	if (load(tempVec, resource->measure) == -1) {
		std::cout << "measure init failed" << std::endl;
		return;
	}

	// need prefix
	tempVec.clear();
	tempVec.push_back(path + std::string("/needPrefix.dic"));
	if (load(tempVec, resource->needPrefix) == -1) {
		std::cout << "needPrefix init failed" << std::endl;
		return;
	}

	// dp mark
	tempVec.clear();
	tempVec.push_back(path + std::string("/DPMark.dic"));
	if (load(tempVec, resource->DPMark) == -1) {
		std::cout << "DPMark word init failed" << std::endl;
		return;
	}

	//====================
	// door
	tempVec.clear();
	tempVec.push_back(path + std::string("/door.dic"));
	if (load(tempVec, resource->doorSet) == -1) {
		std::cout << "door init failed" << std::endl;
		return;
	}

	// for strategy to find xzqh word
	tempVec.clear();
	tempVec.push_back(path + std::string("/xzqh.dic"));
	if (load(tempVec, resource->xzqh) == -1) {
		std::cout << "xzqh init failed" << std::endl;
		return;
	}

	// idf mapping
	if (poi_sim_cfg->m_use_local_idf) {
		tempPath = path + std::string("/idf.dic");
		if (load(tempPath, resource->idf, "\t", false) == -1) {
			std::cout << "idf init failed." << std::endl;
			return;
		}
		findMaxMin(resource->idf, resource->max_idf, resource->min_idf);
		cout << "max idf:" << resource->max_idf << "\t" << resource->min_idf
				<< endl;
		cout << "idf init:" << resource->idf.size() << endl;
	}

	// trans name mapping
	tempVec.clear();
	tempVec.push_back(path + std::string("/trans.name.dic"));
	if (loadTrans(tempVec, resource->transName, "\t", true) == -1) {
		std::cout << "transName init failed." << std::endl;
		return;
	}

	// trans type mapping
	tempVec.clear();
	tempVec.push_back(path + std::string("/trans.type.dic"));
	if (loadTrans(tempVec, resource->transType, "\t", true) == -1) {
		std::cout << "transType init failed." << std::endl;
		return;
	}

	// trans word mapping
	tempVec.clear();
	tempVec.push_back(path + std::string("/trans.word.dic"));
	tempVec.push_back(path + std::string("/trans.number.dic"));
	if (loadTrans(tempVec, resource->transWord, "\t", true) == -1) {
		std::cout << "transWord init failed." << std::endl;
		return;
	}

	// for strategy to find endwith
	tempPath = path + std::string("/endwith.dic");
	if (load(tempPath, resource->endList) == -1) {
		std::cout << "endList init failed." << std::endl;
	}

	// for strategy to find mark word in same part
	tempVec.clear();
	tempVec.push_back(path + std::string("/mark.dic"));
	tempVec.push_back(path + std::string("/school.dic"));
	tempVec.push_back(path + std::string("/office.dic"));
	if (load(tempVec, resource->markWord) == -1) {
		std::cout << "markWord init failed" << std::endl;
		return;
	}

	// for strategy to find district word in same part
	tempVec.clear();
	tempVec.push_back(path + std::string("/road.dic"));
	if (load(tempVec, resource->district) == -1) {
		std::cout << "district init failed" << std::endl;
		return;
	}

	// for strategy to find mark word in bracket
	tempVec.clear();
	tempVec.push_back(path + std::string("/inBracketMark.dic"));
	if (load(tempVec, resource->inBracketMark) == -1) {
		std::cout << "inBracketMark word init failed" << std::endl;
		return;
	}

	// for strategy to find road word
	tempVec.clear();
	tempVec.push_back(path + std::string("/road.dic"));
	if (load(tempVec, resource->road) == -1) {
		std::cout << "road word init failed" << std::endl;
		return;
	}

	// for word end with endlist but not need to remove
	tempVec.clear();
	tempVec.push_back(path + std::string("/undel.dic"));
	if (load(tempVec, resource->undel) == -1) {
		std::cout << "undel word init failed" << std::endl;
		return;
	}

	// for number
	tempVec.clear();
	tempVec.push_back(path + std::string("/number.dic"));
	if (load(tempVec, resource->number) == -1) {
		std::cout << "number word init failed" << std::endl;
		return;
	}

	tempVec.clear();
	tempVec.push_back(path + std::string("/number2.dic"));
	if (load(tempVec, resource->number2) == -1) {
		std::cout << "number2 word init failed" << std::endl;
		return;
	}

	// for word end with delBigram but not need to remove
	tempVec.clear();
	tempVec.push_back(path + std::string("/delBigram.dic"));
	if (load(tempVec, resource->delBigram) == -1) {
		std::cout << "delBigram word init failed" << std::endl;
		return;
	}

	// for word end with delBigram but not need to remove
	tempVec.clear();
	tempVec.push_back(path + std::string("/spec.dic"));
	if (load(tempVec, resource->spec) == -1) {
		std::cout << "spec word init failed" << std::endl;
		return;
	}

	_hashset tempSet;
	transWord(resource->spec, tempSet, resource->transWord);
	resource->spec = tempSet;

	// for word end with delBigram but not need to remove
	tempVec.clear();
	tempVec.push_back(path + std::string("/suffix.dic"));
	if (load(tempVec, resource->suffix) == -1) {
		std::cout << "spec word init failed" << std::endl;
		return;
	}

	// for word end with delBigram but not need to remove
	tempPath = path + std::string("/typeword");
	if (load(tempPath, resource->typeword) == -1) {
		std::cout << "type word init failed" << std::endl;
		return;
	}

	// name only have type word
	_mapSetit tempSetit;
	_hashsetit temphashit;
	for (tempSetit = resource->typeword.begin();
			tempSetit != resource->typeword.end(); ++tempSetit) {
		if (tempSetit->first != "00") {
			temphashit = tempSetit->second.begin();
			for (; temphashit != tempSetit->second.end(); ++temphashit) {
				resource->typeNameMap.insert(make_pair(*temphashit, 100.0));
			}
		}
	}

	// word that must both in two poi
	tempPath = path + std::string("/unlackword");
	if (load(tempPath, resource->unlackword) == -1) {
		std::cout << "unlack word init failed" << std::endl;
		return;
	}

	_mapSet tempMapSet;
	transWord(resource->unlackword, tempMapSet, resource->transWord);
	resource->unlackword = tempMapSet;

	// obj name map
	// must init after transword
	tempVec.clear();
	tempVec.push_back(path + std::string("/obj.dic"));
	if (load(tempVec, resource->objNameMap, "\t", false) == -1) {
		std::cout << "obj init failed" << std::endl;
		return;
	}

	_mapStrDbl tempMapStrDbl;
	transWord(resource->objNameMap, tempMapStrDbl, resource->transWord);
	resource->objNameMap = tempMapStrDbl;

	// obj type
	if (loadObjType(tempVec, resource->objTypeMap, "\t") == -1) {
		std::cout << "obj type init failed" << std::endl;
		return;
	}

	_map tempObjTypeMap;
	transWord(resource->objTypeMap, tempObjTypeMap, resource->transWord);
	resource->objTypeMap = tempObjTypeMap;

	// spec obj name map
	// must init after transword
	tempVec.clear();
	tempVec.push_back(path + std::string("/specObj.dic"));
	if (load(tempVec, resource->specObjMap, "\t", false) == -1) {
		std::cout << "obj init failed" << std::endl;
		return;
	}

	tempMapStrDbl.clear();
	transWord(resource->specObjMap, tempMapStrDbl, resource->transWord);
	resource->specObjMap = tempMapStrDbl;

	// name tail
	tempVec.clear();
	tempVec.push_back(path + std::string("/nameTail.dic"));
	if (load(tempVec, resource->nameTailMap, "\t", false) == -1) {
		std::cout << "name tail init failed." << std::endl;
		return;
	}

	// bigram no mean name tail
	tempVec.clear();
	tempVec.push_back(path + std::string("/noMeanBigramTail.dic"));
	if (load(tempVec, resource->noMeanBgmTailSet) == -1) {
		std::cout << "no mean bigram tail init failed." << std::endl;
		return;
	}

	_mapit name_tail_it = resource->nameTailMap.begin();
	for (; name_tail_it != resource->nameTailMap.end(); ++name_tail_it) {
		resource->noMeanBgmTailSet.insert(name_tail_it->first);
	}

	//====================
	// area tail
	tempVec.clear();
	tempVec.push_back(path + std::string("/areaTail.dic"));
	if (load(tempVec, resource->areaTailSet) == -1) {
		std::cout << "area tail init failed." << std::endl;
		return;
	}

	// type map
	tempVec.clear();
	tempVec.push_back(path + std::string("/typemap.dic"));
	if (loadTypeMap(tempVec, resource->typeMap, "\t") == -1) {
		std::cout << "type map init failed" << std::endl;
		return;
	}

	// diff word
	// must init after trans word
	tempVec.clear();
	tempVec.push_back(path + std::string("/diff.dic"));
	if (load(tempVec, resource->diff) == -1) {
		std::cout << "diff word init failed" << std::endl;
		return;
	}

	tempSet.clear();
	transWord(resource->diff, tempSet, resource->transWord);
	resource->diff = tempSet;

	// same mean name
	tempPath = path + std::string("/smName.dic");
	if (load(tempPath, resource->smNameMap, "\t") == -1) {
		std::cout << "same mean name init failed" << std::endl;
		return;
	}

	// remove type word name data
	tempVec.clear();
	tempVec.push_back(path + std::string("/removeTypeName.dic"));
	if (load(tempVec, resource->removeTypeNameSet) == -1) {
		std::cout << "removeTypeNameSet init failed" << std::endl;
		return;
	}

	// district trans map
	tempVec.clear();
	tempVec.push_back(path + std::string("/trans.district.dic"));
	if (load(tempVec, resource->districtTransMap, "\t", false) == -1) {
		std::cout << "district trans map init failed." << std::endl;
		return;
	}

	// classity category
	tempPath = path + std::string("/classify/cate_code.txt");
	GetCateCode1(tempPath.c_str(), resource->cateCodeMap);

	tempPath = path + std::string("/classify/");
	GetKeyCategory(tempPath);

	// state
	tempVec.clear();
	tempVec.push_back(path + std::string("/states.dic"));
	if (load(tempVec, resource->stateSet) == -1) {
		std::cout << "state init failed" << std::endl;
		return;
	}

	// province
	tempVec.clear();
	tempVec.push_back(path + std::string("/province.dic"));
	if (load(tempVec, resource->provinceSet) == -1) {
		std::cout << "province init failed" << std::endl;
		return;
	}

	// area type
	resource->areaTypeSet.insert("26");
	resource->areaTypeSet.insert("28");
	resource->areaTypeSet.insert("12");

	// direction
	resource->directionSet.insert("��");
	resource->directionSet.insert("��");
	resource->directionSet.insert("��");
	resource->directionSet.insert("��");
	resource->directionSet.insert("����");
	resource->directionSet.insert("����");
	resource->directionSet.insert("����");
	resource->directionSet.insert("����");

	// gov tail
	resource->govTailSet.insert("��");
	resource->govTailSet.insert("��");
	resource->govTailSet.insert("��");
	resource->govTailSet.insert("��");
	resource->govTailSet.insert("��");
	resource->govTailSet.insert("��");
	resource->govTailSet.insert("��");
	resource->govTailSet.insert("·");
	resource->govTailSet.insert("��");
	resource->govTailSet.insert("��");

	// ��֧
	resource->dizhiSet.insert("��");
	resource->dizhiSet.insert("��");
	resource->dizhiSet.insert("��");
	resource->dizhiSet.insert("��");
	resource->dizhiSet.insert("��");
	resource->dizhiSet.insert("��");
	resource->dizhiSet.insert("��");
	resource->dizhiSet.insert("��");
	resource->dizhiSet.insert("��");
	resource->dizhiSet.insert("��");
}

void statResource(const PreProcMap *pre_proc_map, Resource *resource) {
	PreProcMap::const_iterator it = pre_proc_map->begin();
	for (; it != pre_proc_map->end(); ++it) {
		statIdf(it->second, resource->idf);
		statTel(it->second, resource->uselessTelMap);
	}
}

void updateResourse(long poi_num, int threshold, Resource *resource) {
	_mapStrDblit it = resource->idf.begin();
	for (; it != resource->idf.end(); ++it) {
		it->second = log10(poi_num / (it->second));
		if (it->second > resource->max_idf) {
			resource->max_idf = it->second;
		}
		if (it->second < resource->min_idf) {
			resource->min_idf = it->second;
		}
	}

	_mapInt temp_tel_map = resource->uselessTelMap;
	resource->uselessTelMap.clear();
	_mapIntit tel_it = temp_tel_map.begin();
	for (; tel_it != temp_tel_map.end(); ++tel_it) {
		if (tel_it->second >= threshold) {
			resource->uselessTelMap.insert(
					make_pair(tel_it->first, tel_it->second));
		}
	}
}

//=================================================
// װ��tools��������
//=================================================
void loadTools(const PoiSimCfg *poi_sim_cfg, Tools *tools) {
	tools->m_wordsplit->load(poi_sim_cfg->m_wordsplit_dic_vec);
}

//==================================================
// �����ͺ���
//==================================================
bool setResult(const string &step_name, const double &score,
		const bool return_value, SimTempCache *sim_temp_cache) {
	sim_temp_cache->m_rule_result_map.insert(make_pair(step_name, true));
	sim_temp_cache->m_score = score;
	return return_value;
}

bool nameIsSame(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource) {
	if (base_data->m_name == inc_data->m_name) {
		return true;
	} else if (base_data->m_del_punc_name == inc_data->m_del_punc_name) {
		return true;
	} else if (pyIsSame(base_data->m_del_punc_name, inc_data->m_del_punc_name,
			resource->pinyin, 0)) {
		return true;
	}

	return false;
}

bool isValidCfg(const PoiSimCfg *poi_sim_cfg) {
	if (poi_sim_cfg->m_base_inc_sim_func == NULL) {
		return false;
	} else if (poi_sim_cfg->m_inc_inc_sim_func == NULL) {
		return false;
	}

	return true;
}

bool isDiffFenDian(const PoiSimCfg *poi_sim_cfg) {
	if (poi_sim_cfg->m_base_inc_sim_func == NULL) {
		return false;
	} else if (poi_sim_cfg->m_inc_inc_sim_func == NULL) {
		return false;
	}

	return true;
}

bool isSameAddr(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache) {
	if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3
			&& sim_temp_cache->m_addr_sim > 0.52) {
		return true;
	}
	return false;
}

int isSameFenDian(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache) {
	string base_fendian = base_data->m_fendian;
	string inc_fendian = inc_data->m_fendian;

	if (base_fendian == "" && inc_fendian == "") {
		return 0;
	} else if (wordIsSame(base_fendian, inc_fendian)) {
		return 1;
	} else if (sim_temp_cache->m_bracket_score > 0) {
		return 1;
	} else {
		return -1;
	}

	return 0;
}

bool hasSpecObj(const PreProcData *pre_proc_data, const Resource *resource) {
	if (pre_proc_data->m_obj_name_set.size() > 0) {
		_mapStrDbl::const_iterator spec_it;
		set<string>::const_iterator it = pre_proc_data->m_obj_name_set.begin();
		for (; it != pre_proc_data->m_obj_name_set.end(); ++it) {
			spec_it = resource->specObjMap.find(*it);
			if (spec_it != resource->specObjMap.end()) {
				return true;
			}
		}
	}

	return false;
}

//==================================================
// һ��һ�������㺯��
//==================================================
void getDistance(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache) {
	sim_temp_cache->m_distance = MapTools::get_distance(base_data->m_lat,
			base_data->m_lng, inc_data->m_lat, inc_data->m_lng);
}

void getLenDiff(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache) {
//	sim_temp_cache->m_len_diff = abs(
//			(int) base_data->m_del_punc_name_len
//					- (int) inc_data->m_del_punc_name_len);
	sim_temp_cache->m_len_diff = (int) levDis(base_data->m_del_punc_name,
			inc_data->m_del_punc_name);
}

void getAddrSim(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache) {
//	sim_temp_cache->m_addr_sim = computeAddressSimilarity(
//			base_data->m_addrsim_xzqh_map, inc_data->m_addrsim_xzqh_map,
//			base_data->m_addrsim_road_map, inc_data->m_addrsim_road_map,
//			base_data->m_addrsim_poi_set, inc_data->m_addrsim_poi_set);

	sim_temp_cache->m_addr_sim = computeAddressSimilarity(
			base_data->m_addrsim_xzqh_map, inc_data->m_addrsim_xzqh_map,
			base_data->m_addrsim_road_map, inc_data->m_addrsim_road_map,
			base_data->m_addrsim_community_set,
			inc_data->m_addrsim_community_set, base_data->m_addrsim_poi_set,
			inc_data->m_addrsim_poi_set, base_data->m_addrsim_hn_set,
			inc_data->m_addrsim_hn_set, base_data->m_addrsim_home_set,
			inc_data->m_addrsim_home_set, base_data->m_addrsim_floor_set,
			inc_data->m_addrsim_floor_set, sim_temp_cache->m_addr_sim_value,
			resource->m_group_score, resource->m_sigle_score);
}

void getSameTel(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache) {
	if (base_data->m_tel_set.size() > 0 && inc_data->m_tel_set.size() > 0) {
		set<string>::const_iterator base_it, inc_it;

		for (inc_it = inc_data->m_tel_set.begin();
				inc_it != inc_data->m_tel_set.end(); ++inc_it) {
			for (base_it = base_data->m_tel_set.begin();
					base_it != base_data->m_tel_set.end(); ++base_it) {
				if (tailIsSame(*base_it, *inc_it, 7)
						&& !startswith(*inc_it, "400")
						&& !startswith(*inc_it, "800")
						&& resource->uselessTelMap.find(*inc_it)
								== resource->uselessTelMap.end()) {
					sim_temp_cache->m_same_tel_set.insert(*inc_it);
				}
			}
		}
	}
}

void getSameType(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache) {
	findType(base_data, inc_data, resource->typeMap, sim_temp_cache);

	if (sim_temp_cache->m_same_type_set.size() == 1) {
		sim_temp_cache->m_same_type_str =
				*(sim_temp_cache->m_same_type_set.begin());
	}
}

void getDiffName(const PreProcData *base_data, const PreProcData *inc_data,
		SimTempCache *sim_temp_cache) {
	//both in base and inc
	set_intersection(base_data->m_name_set.begin(), base_data->m_name_set.end(),
			inc_data->m_name_set.begin(), inc_data->m_name_set.end(),
			insert_iterator<_vector>(sim_temp_cache->m_same_words_vec,
					sim_temp_cache->m_same_words_vec.begin()));

	//only in base
	set_difference(base_data->m_name_set.begin(), base_data->m_name_set.end(),
			inc_data->m_name_set.begin(), inc_data->m_name_set.end(),
			insert_iterator<_vector>(sim_temp_cache->m_diff_words_base_vec,
					sim_temp_cache->m_diff_words_base_vec.begin()));

	//only in inc
	set_difference(inc_data->m_name_set.begin(), inc_data->m_name_set.end(),
			base_data->m_name_set.begin(), base_data->m_name_set.end(),
			insert_iterator<_vector>(sim_temp_cache->m_diff_words_inc_vec,
					sim_temp_cache->m_diff_words_inc_vec.begin()));
}

void getObj(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache) {
	set_intersection(base_data->m_obj_name_set.begin(),
			base_data->m_obj_name_set.end(), inc_data->m_obj_name_set.begin(),
			inc_data->m_obj_name_set.end(),
			insert_iterator<set<string> >(sim_temp_cache->m_obj_name_set,
					sim_temp_cache->m_obj_name_set.begin()));

	_mapStrDbl::const_iterator spec_it, obj_it;
	set<string>::const_iterator it = sim_temp_cache->m_obj_name_set.begin();
	for (; it != sim_temp_cache->m_obj_name_set.end(); ++it) {
		// ��ȡ��������ž���
		obj_it = resource->objNameMap.find(*it);
		if (obj_it != resource->objNameMap.end()) {
			if (obj_it->second < sim_temp_cache->m_max_obj_distance) {
				sim_temp_cache->m_max_obj_distance = obj_it->second;
			}
		}

		// ����Ƿ����������������վ��������
		spec_it = resource->specObjMap.find(*it);
		if (spec_it != resource->specObjMap.end()) {
			sim_temp_cache->m_spec_obj_name_set.insert(spec_it->first);
			if (spec_it->second < sim_temp_cache->m_max_spec_obj_distance) {
				sim_temp_cache->m_max_spec_obj_distance = spec_it->second;
			}
		}
	}
}

void getBracketScore(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache) {
	if (base_data->m_bracket_set.size() > 0
			&& inc_data->m_bracket_set.size() > 0) {
		getSetSim(base_data->m_bracket_set, inc_data->m_bracket_set,
				sim_temp_cache->m_bracket_score,
				sim_temp_cache->m_bracket_same_words_vec,
				sim_temp_cache->m_bracket_diff_words_base_vec,
				sim_temp_cache->m_bracket_diff_words_inc_vec, resource);
	} else {
		sim_temp_cache->m_bracket_score = 0.0;
	}
}

void getIncludeName(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, SimTempCache *sim_temp_cache) {
	if (base_data->m_format_name.find(inc_data->m_format_name) != string::npos
			|| inc_data->m_format_name.find(base_data->m_format_name)
					!= string::npos) {
		sim_temp_cache->m_name_is_include = true;
	}
}

//==================================================
// һ��һ�����жϵĹ�����
// ����ֵ��
// 	false: ��ȷ�����ƻ�����
//	true: ȷ�����ƻ�����
//==================================================
bool ruleSameID(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (base_data->m_id == inc_data->m_id) {
		return setResult("same_id", 1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("same_id", false));
	return false;
}

bool ruleSpecSource(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	const RuleSpecSourceCfg *temp_cfg = (RuleSpecSourceCfg *) cfg;
	vector<string>::const_iterator it =
			(*(temp_cfg->m_source_name_vec)).begin();

	for (; it != (*(temp_cfg->m_source_name_vec)).end(); ++it) {
		if (base_data->m_source == *it || inc_data->m_source == *it) {
			return setResult("spec_source", -1.0, true, sim_temp_cache);
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("spec_source", false));
	return false;
}

//bool ruleDiffDoor(const PreProcData *base_data, const PreProcData *inc_data,
//		const void *cfg, const Resource *resource,
//		SimTempCache *sim_temp_cache) {
//	const RuleDiffDoorCfg *temp_cfg = (RuleDiffDoorCfg *) cfg;
//	if (base_data->m_door_set.size() > 0 && inc_data->m_door_set.size() > 0
//			&& sim_temp_cache->m_distance <= temp_cfg->m_distance) {
//		if (isSpecDoor(base_data->m_door_set, resource)
//				&& isSpecDoor(inc_data->m_door_set, resource)
//				&& (!checkSetInclude(base_data->m_door_set,
//						inc_data->m_door_set)
//						|| !checkSetInclude(inc_data->m_door_set,
//								base_data->m_door_set))) {
//			return setResult("diff_door", -1.0, true, sim_temp_cache);
//		}
//		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_door", false));
//		return false;
//	} else if ((!checkSetInclude(base_data->m_door_set, inc_data->m_door_set)
//			|| !checkSetInclude(inc_data->m_door_set, base_data->m_door_set))
//			&& base_data->m_del_punc_name != inc_data->m_del_punc_name) {
//		return setResult("diff_door", -1.0, true, sim_temp_cache);
//	}
//
//	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_door", false));
//	return false;
//}

bool ruleDiffDoor(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	const RuleDiffDoorCfg *temp_cfg = (RuleDiffDoorCfg *) cfg;
	if (base_data->m_door_set.size() > 0 && inc_data->m_door_set.size() > 0
			&& sim_temp_cache->m_distance <= temp_cfg->m_distance) {
		if (isSpecDoor(base_data->m_door_set, resource)
				&& isSpecDoor(inc_data->m_door_set, resource)
				&& (!checkInclude(base_data->m_door_set, inc_data->m_door_set,
						inc_data->m_del_punc_name)
						|| !checkInclude(inc_data->m_door_set,
								base_data->m_door_set,
								base_data->m_del_punc_name))) {
			return setResult("diff_door", -1.0, true, sim_temp_cache);
		}
		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_door", false));
		return false;
	} else if ((!checkInclude(base_data->m_door_set, inc_data->m_door_set,
			inc_data->m_del_punc_name)
			|| !checkInclude(inc_data->m_door_set, base_data->m_door_set,
					base_data->m_del_punc_name))
			&& base_data->m_del_punc_name != inc_data->m_del_punc_name) {
		return setResult("diff_door", -1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_door", false));
	return false;
}

bool ruleAlias(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {

	vector<int> same;
	set_intersection(base_data->m_smname_group_set.begin(),
			base_data->m_smname_group_set.end(),
			inc_data->m_smname_group_set.begin(),
			inc_data->m_smname_group_set.end(),
			insert_iterator<vector<int> >(same, same.begin()));
	if (same.size() > 0) {
		return setResult("same_mean", 1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("same_mean", false));
	return false;
}

bool ruleDiffDpMark(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	const RuleDiffDpmarkCfg *temp_cfg = (RuleDiffDpmarkCfg *) cfg;

	if (base_data->m_dpmark_set.size() > 0
			^ inc_data->m_dpmark_set.size() > 0) {
//		if (sim_temp_cache->m_distance > temp_cfg->m_distance
//				|| startswith(sim_temp_cache->m_same_type_str, "28")) {
//			return setResult("diff_dpmark", -1.0, true, sim_temp_cache);
//		}
		return setResult("diff_dpmark", -1.0, true, sim_temp_cache);
	} else if (base_data->m_dpmark_set.size() > 0
			&& inc_data->m_dpmark_set.size() > 0) {
		if (!checkSetInclude(base_data->m_dpmark_set, inc_data->m_dpmark_set)
				|| !checkSetInclude(inc_data->m_dpmark_set,
						base_data->m_dpmark_set)) {
			// �г�ͻ�Ĵ�
			return setResult("diff_dpmark", -1.0, true, sim_temp_cache);
		} else {
			sim_temp_cache->m_rule_result_map.insert(
					make_pair("diff_dpmark", false));
			return false;
		}
	} else if (base_data->m_dpmark_set.size() == 0
			^ inc_data->m_dpmark_set.size() == 0) {
		if (base_data->m_door_set.size() > 0 && inc_data->m_door_set.size() > 0
				&& (sim_temp_cache->m_distance < temp_cfg->m_distance
						|| sim_temp_cache->m_addr_sim > temp_cfg->m_addr_sim)) {
			sim_temp_cache->m_rule_result_map.insert(
					make_pair("diff_dpmark", false));
			return false;
		} else {
			return setResult("diff_dpmark", -1.0, true, sim_temp_cache);
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_dpmark", false));
	return false;
}

bool ruleDiffGovTail(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (base_data->m_del_punc_name != inc_data->m_del_punc_name) {
		const RuleDiffGovTailCfg *temp_cfg = (RuleDiffGovTailCfg *) cfg;
		if (base_data->m_main_type == inc_data->m_main_type
				&& (*(temp_cfg->m_type_set)).find(base_data->m_main_type)
						!= (*(temp_cfg->m_type_set)).end()) {
			if (base_data->m_gov_tail != "" && inc_data->m_gov_tail != ""
					&& base_data->m_gov_tail != inc_data->m_gov_tail) {
				return setResult("diff_gov_tail", -1.0, true, sim_temp_cache);
			} else if ((base_data->m_gov_tail != "" ^ inc_data->m_gov_tail != "")
					&& base_data->m_sub_type != inc_data->m_sub_type
					&& (base_data->m_name_vec.size() == 1
							&& inc_data->m_name_vec.size() == 1
							&& base_data->m_name_vec[0]
									!= inc_data->m_name_vec[0])) {
				return setResult("diff_gov_tail", -1.0, true, sim_temp_cache);
			} else if (base_data->m_sub_type != inc_data->m_sub_type
					&& base_data->m_sub_type != "2699"
					&& inc_data->m_sub_type != "2699"
					&& base_data->m_format_name != inc_data->m_format_name) {
				return setResult("diff_gov_tail", -1.0, true, sim_temp_cache);
			}
		}

	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_gov_tail", false));
	return false;
}

bool ruleTypeName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (sim_temp_cache->m_spec_obj_name_set.size() > 0) {
		// �������������͵����ʹʲ�ʹ�øò���
		sim_temp_cache->m_rule_result_map.insert(make_pair("type_name", false));
		return false;
	}

	_mapStrDbl::const_iterator base_it, inc_it, end_it;
	base_it = resource->typeNameMap.find(base_data->m_format_name);
	inc_it = resource->typeNameMap.find(inc_data->m_format_name);
	end_it = resource->typeNameMap.end();

	if (base_it != end_it && inc_it != end_it) {
		if (base_it->first == inc_it->first) {
			if (sim_temp_cache->m_addr_sim < 0.15) {
				// ��ַ��ͻ���ۺ�
				return setResult("type_name", -1.0, true, sim_temp_cache);
			} else if (sim_temp_cache->m_same_tel_set.size() > 0
					|| (sim_temp_cache->m_addr_sim > 0.52
							&& base_data->m_addr_level < 3
							&& inc_data->m_addr_level < 3)
					|| (base_it->second > sim_temp_cache->m_distance)) {
				// �绰��ͬ ��ַ���� ����ܽ�
				return setResult("type_name", 1.0, true, sim_temp_cache);
			} else {
				return setResult("type_name", -1.0, true, sim_temp_cache);
			}
		}
	} else if (base_it != end_it ^ inc_it != end_it) {
		return setResult("type_name", -1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("type_name", false));
	return false;
}

//bool ruleChain(const PreProcData *base_data, const PreProcData *inc_data,
//		const void *cfg, const Resource *resource,
//		SimTempCache *sim_temp_cache) {
//	if (base_data->m_obj_name != ""
//			&& base_data->m_obj_name == inc_data->m_obj_name) {
//		sim_temp_cache->m_obj_name = base_data->m_obj_name;
//		// ����Ƿ����������������վ��������
//		_mapStrDbl::const_iterator spec_it = resource->specObjMap.find(
//				base_data->m_obj_name);
//		if (spec_it != resource->specObjMap.end()) {
//			if (sim_temp_cache->m_distance > spec_it->second
//					&& sim_temp_cache->m_addr_sim < 0.5
//					&& sim_temp_cache->m_same_tel_set.size() == 0) {
//				// ����̫Զ��û�е�ַ�绰��Ϊ�����ο�ʱ��Ϊ��ͬ
//				return setResult("chain", -1.0, true, sim_temp_cache);
//			}
//		}
//
//		// ͬ��������������
//		_mapStrDbl::const_iterator it = resource->objNameMap.find(
//				base_data->m_obj_name);
//		if (it != resource->objNameMap.end()) {
//			if (it->second >= sim_temp_cache->m_distance) {
//				// ����ܽ���������
//				return setResult("chain", 1.0, true, sim_temp_cache);
//			}
//		}
//	} else if (base_data->m_obj_name != inc_data->m_obj_name) {
//		return setResult("chain", -1.0, true, sim_temp_cache);
//	}
//
//	sim_temp_cache->m_rule_result_map.insert(make_pair("chain", false));
//	return false;
//}

//bool ruleChain(const PreProcData *base_data, const PreProcData *inc_data,
//		const void *cfg, const Resource *resource,
//		SimTempCache *sim_temp_cache) {
//	if (base_data->m_obj_name_set.size() != inc_data->m_obj_name_set.size()) {
//		// �в�ͬ������
//		return setResult("chain", -1.0, true, sim_temp_cache);
//	} else if (base_data->m_obj_name_set.size() != 0) {
//		// ������������ͬ
//		if (checkSetInclude(base_data->m_obj_name_set, inc_data->m_obj_name_set)
//				&& checkSetInclude(inc_data->m_obj_name_set,
//						base_data->m_obj_name_set)) {
//			// ����ͬ������
//			int distance_count = 0;
//			_mapStrDbl::const_iterator spec_it;
//			set<string>::const_iterator it = base_data->m_obj_name_set.begin();
//			for (; it != base_data->m_obj_name_set.end(); ++it) {
//				sim_temp_cache->m_obj_name_set.insert(*it);
//				// ����Ƿ����������������վ��������
//				spec_it = resource->specObjMap.find(*it);
//				if (spec_it != resource->specObjMap.end()) {
//					if (sim_temp_cache->m_distance > spec_it->second
//							&& sim_temp_cache->m_addr_sim < 0.51
//							&& sim_temp_cache->m_same_tel_set.size() == 0) {
//						// ����̫Զ��û�е�ַ�绰��Ϊ�����ο�ʱ��Ϊ��ͬ
//						return setResult("chain", -1.0, true, sim_temp_cache);
//					}
//				} else {
//					if (sim_temp_cache->m_distance > 600
//							&& sim_temp_cache->m_addr_sim < 0.5
//							&& sim_temp_cache->m_same_tel_set.size() == 0
//							&& !nameIsSame(base_data, inc_data, resource)) {
//						// ����̫Զ��û�е�ַ�绰��Ϊ�����ο�ʱ��Ϊ��ͬ
//						return setResult("chain", -1.0, true, sim_temp_cache);
//					}
//				}
//
//				if (sim_temp_cache->m_distance
//						<= base_data->m_obj_max_distance) {
//					distance_count += 1;
//				}
//			}
//
//			if (distance_count == base_data->m_obj_name_set.size()) {
//				// ��ͨ���������ܽ�
//				return setResult("chain", 1.0, true, sim_temp_cache);
//			}
//		} else {
//			return setResult("chain", -1.0, true, sim_temp_cache);
//		}
//	}
//
//	sim_temp_cache->m_rule_result_map.insert(make_pair("chain", false));
//	return false;
//}

bool ruleChain(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
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

				if (isConflict(base_data, inc_data, resource)) {
					// �����г�ͻ����Ϊ�������������жϱ�׼�������ж�
					sim_temp_cache->m_rule_result_map.insert(
							make_pair("chain", false));
					return false;
				} else if (sim_temp_cache->m_distance > 600
						&& sim_temp_cache->m_addr_sim < 0.52
						&& sim_temp_cache->m_same_tel_set.size() == 0
						&& !nameIsSame(base_data, inc_data, resource)) {
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

bool ruleSameName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	const RuleSameNameCfg *temp_cfg = (RuleSameNameCfg *) cfg;

	// ���ƴ����ͬ �������Ͳ�ͬ��ɵ����
	if (sim_temp_cache->m_same_type_str == ""
			&& base_data->m_del_punc_name != inc_data->m_del_punc_name) {
		sim_temp_cache->m_rule_result_map.insert(make_pair("same_name", false));
		return false;
	}

	if ((base_data->m_obj_name_set.size() > 0
			|| inc_data->m_obj_name_set.size() > 0)
			&& nameIsSame(base_data, inc_data, resource)) {
		// �������п���������ͬ�����ǵ�ַ��һ�����ܾۺϵ���������Բ��ڸýڵ㴦��
		// �˷�ɽׯ �� ��Ϊ������ʶ�������⣬���Ͳ�һ������һ�������ݣ����ܾۺϲ���
		if (!hasSpecObj(base_data, resource)
				&& !hasSpecObj(inc_data, resource)) {
			if (sim_temp_cache->m_same_tel_set.size() > 0
					|| (base_data->m_addr_level < 3
							&& inc_data->m_addr_level < 3
							&& sim_temp_cache->m_addr_sim
									>= temp_cfg->m_addr_sim)
					|| (sim_temp_cache->m_distance
							< base_data->m_obj_max_distance
							|| sim_temp_cache->m_distance
									< inc_data->m_obj_max_distance)) {
				return setResult("same_name", 1.0, true, sim_temp_cache);
			}
		}
	} else if (nameIsSame(base_data, inc_data, resource)) {
		if (sim_temp_cache->m_same_type_str == "271016"
				&& sim_temp_cache->m_addr_sim > 0.49
				&& sim_temp_cache->m_distance < 1000) {
			// ��վ���ͣ�������ȫһ��
			return setResult("same_name", 1.0, true, sim_temp_cache);
		} else if (sim_temp_cache->m_same_tel_set.size() > 0
				|| (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3
						&& sim_temp_cache->m_addr_sim > temp_cfg->m_addr_sim)
				|| sim_temp_cache->m_distance < temp_cfg->m_reliable_distance) {
			// �绰��ͬ���ߵ�ַ���ƻ����ܽ�
			return setResult("same_name", 1.0, true, sim_temp_cache);
		} else if (sim_temp_cache->m_same_tel_set.size() == 0
				&& sim_temp_cache->m_addr_sim <= temp_cfg->m_addr_sim
				&& sim_temp_cache->m_distance > temp_cfg->m_reliable_distance) {
			return setResult("same_name", -1.0, true, sim_temp_cache);
		} else if (sim_temp_cache->m_distance < temp_cfg->m_recall_distance) {
			// �������ٻط�Χ��ȷ�ŷ�Χ֮��
			if (base_data->m_type == "241000"
					&& base_data->m_type == inc_data->m_type) {
				// ѧУ��ѧ��
				return setResult("same_name", 1.0, true, sim_temp_cache);
			} else if (base_data->m_main_type == inc_data->m_main_type
					&& resource->areaTypeSet.find(base_data->m_main_type)
							!= resource->areaTypeSet.end()) {
				// �����͵ĵ�
				return setResult("same_name", 1.0, true, sim_temp_cache);
			}
		}
	}
//	else if (base_data->m_format_name == inc_data->m_format_name) {
//		// �淶����һ��
//		if ((sim_temp_cache->m_same_tel_set.size() > 0
//				|| sim_temp_cache->m_addr_sim > temp_cfg->m_addr_sim)
//				&& sim_temp_cache->m_distance <= temp_cfg->m_reliable_distance) {
//			// �绰��ͬ���ߵ�ַ����
//			return setResult("same_name", 0.85, true, sim_temp_cache);
//		}
//	}
	else {
		if (sim_temp_cache->m_distance > temp_cfg->m_reliable_distance) {
			return setResult("same_name", -1.0, true, sim_temp_cache);
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("same_name", false));
	return false;
}

bool ruleDiffType(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	//cout << base_data->m_accurate_type << "\t" << inc_data->m_accurate_type << endl;
	//cout << base_data->m_accurate_main_type << "\t" << inc_data->m_accurate_main_type << endl;
	//cout << base_data->m_accurate_sub_type << "\t" << inc_data->m_accurate_sub_type << endl;

	// �����������ͬ�����������͵�����
//	if (sim_temp_cache->m_obj_name_set.size() > 0) {
//		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_type", false));
//		return false;
//	}

	if(base_data->m_soso_type != ""
			&& inc_data->m_soso_type != ""
			&& !isTypeConflict(base_data->m_soso_type,
					inc_data->m_soso_type, resource)){
		// sosotype ��������ܾۺϵĿ��ܣ������ж����ͳ�ͻ
	}else if (sim_temp_cache->m_is_mode_self
			&& isConflict(base_data, inc_data, resource)
			&& base_data->m_del_punc_name != inc_data->m_del_punc_name) {
		return setResult("diff_type", -1.0, true, sim_temp_cache);
	}

	// ��׼����һ��û��ʶ����������ж�
	if (base_data->m_accurate_type == "000000"
			|| inc_data->m_accurate_type == "000000") {
		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_type", false));
		return false;
	}

	if (base_data->m_accurate_type != inc_data->m_accurate_type
			&& base_data->m_accurate_sub_type == inc_data->m_accurate_sub_type
			&& !endswith(base_data->m_accurate_type, "99")
			&& !endswith(inc_data->m_accurate_type, "99")) {
		return setResult("diff_type", -1.0, true, sim_temp_cache);
	} else if (base_data->m_accurate_sub_type != inc_data->m_accurate_sub_type
			&& base_data->m_accurate_main_type == inc_data->m_accurate_main_type
			&& !endswith(base_data->m_accurate_sub_type, "99")
			&& !endswith(inc_data->m_accurate_sub_type, "99")) {
		return setResult("diff_type", -1.0, true, sim_temp_cache);
	}

	bool is_not_same = false;
	if (base_data->m_accurate_main_type == inc_data->m_accurate_main_type
			|| base_data->m_accurate_sub_type == inc_data->m_accurate_sub_type
			|| base_data->m_accurate_type != inc_data->m_accurate_type) {
		is_not_same = false;
	} else if (base_data->m_accurate_main_type != inc_data->m_accurate_main_type
			&& !endswith(base_data->m_accurate_main_type, "99")
			&& !endswith(inc_data->m_accurate_main_type, "99")) {
		is_not_same = true;
	} else if (base_data->m_accurate_sub_type != inc_data->m_accurate_sub_type
			&& !endswith(base_data->m_accurate_sub_type, "99")
			&& !endswith(inc_data->m_accurate_sub_type, "99")) {
		is_not_same = true;
	} else if (base_data->m_accurate_type != inc_data->m_accurate_type
			&& !endswith(base_data->m_accurate_type, "99")
			&& !endswith(inc_data->m_accurate_type, "99")) {
		is_not_same = true;
	}

	if (base_data->m_accurate_type != "" && inc_data->m_accurate_type != "") {
		if (is_not_same) {
			// ��׼�಻��ͬ
			return setResult("diff_type", -1.0, true, sim_temp_cache);
		}
	} else if (sim_temp_cache->m_same_type_set.size() == 0) {
		return setResult("diff_type", -1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_type", false));
	return false;
}

bool ruleDistance(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (cfg == NULL) {
		cout << "cfg is null in ruleDistance" << endl;
		return true;
	}
	RuleDistanceCfg *temp_cfg = (RuleDistanceCfg *) cfg;

	if (base_data->m_type == "271020"
			&& base_data->m_type == inc_data->m_type) {
		// airport
		if (sim_temp_cache->m_distance <= temp_cfg->m_airport_recall_distance) {
			// ���ٻط�Χ��
			if (endswith(base_data->m_format_name, "����")
					&& endswith(inc_data->m_format_name, "����")) {
				if (base_data->m_door_set.size() == 0
						&& inc_data->m_door_set.size() == 0) {
					return setResult("distance", 1.0, true, sim_temp_cache);
				}
			} else if (endswith(base_data->m_format_name, "����")
					^ endswith(inc_data->m_format_name, "����")) {
				return setResult("distance", -1.0, true, sim_temp_cache);
			}
		} else {
			// �����ٻط�Χ��
			return setResult("distance", -1.0, true, sim_temp_cache);
		}
	}
//	else if (base_data->m_type == "271016"
//			&& base_data->m_type == inc_data->m_type) {
	// train
//		if (sim_temp_cache->m_distance <= temp_cfg->m_train_recall_distance) {
//			// ���ٻط�Χ��
//			if (sim_temp_cache->m_distance
//					<= temp_cfg->m_train_reliable_distance) {
//				if (base_data->m_door_set.size() == 0
//						&& inc_data->m_door_set.size() == 0
//						&& base_data->m_unlack_word_set.size() == 0
//						&& inc_data->m_unlack_word_set.size() == 0
//						&& base_data->m_dpmark_set.size() == 0
//						&& inc_data->m_dpmark_set.size() == 0
//						&& base_data->m_num_name_set.size() == 0
//						&& inc_data->m_num_name_set.size() == 0) {
//					return setResult("distance", 1.0, true, sim_temp_cache);
//				}
//			}
//		} else {
//			// �����ٻط�Χ��
//			return setResult("distance", -1.0, true, sim_temp_cache);
//		}
//	}
	else {
		// normal type data, use default cfg
		if (sim_temp_cache->m_distance > temp_cfg->m_default_recall_distance) {
			// �����ٻط�Χ��
			return setResult("distance", -1.0, true, sim_temp_cache);
		} else if (sim_temp_cache->m_distance
				> temp_cfg->m_default_reliable_distance) {
			// �������ŷ�Χ
			if (!nameIsSame(base_data, inc_data, resource)
					&& sim_temp_cache->m_same_tel_set.size() == 0
					&& sim_temp_cache->m_addr_sim < temp_cfg->m_addr_sim) {
				// ���Ʋ�ͬ �绰��ͬ ��ַ������
				return setResult("distance", -1.0, true, sim_temp_cache);
			}
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("distance", false));
	return false;
}

bool ruleBridge(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if ((endswith(base_data->m_format_name, "��")
			&& base_data->m_main_type == "26")
			^ (endswith(inc_data->m_format_name, "��")
					&& inc_data->m_main_type == "26")) {
		return setResult("bridge", -1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("bridge", false));
	return false;
}

bool ruleAtm(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if ((base_data->m_bigram_name.find("atm") != std::string::npos
			^ inc_data->m_bigram_name.find("atm") != std::string::npos)
			|| (base_data->m_num_name_set.find("atm")
					!= base_data->m_num_name_set.end()
					^ inc_data->m_num_name_set.find("atm")
							!= inc_data->m_num_name_set.end())) {
		return setResult("atm", -1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("atm", false));
	return false;
}

bool ruleInBracket(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (base_data->m_format_name == inc_data->m_format_name
			|| base_data->m_bigram_name == inc_data->m_bigram_name) {
		// ���  �����׶�԰(�����׶�԰����)	�����׶�԰ ���ܾۺϵ�����
	} else if (isInclude(base_data->m_format_name, inc_data->m_raw_bracket_vec)
			|| isInclude(inc_data->m_format_name,
					base_data->m_raw_bracket_vec)) {
//		int flag_count = 0;
//		if (sim_temp_cache->m_same_tel_set.size() > 0) {
//			flag_count += 1;
//		}
//		if (sim_temp_cache->m_addr_sim > 0.5) {
//			flag_count += 1;
//		}
//		if (sim_temp_cache->m_distance < 5) {
//			flag_count += 1;
//		}
//
//		if (flag_count > 1) {
//			// �绰��ͬ ��ַ���� ����ܽ� ������������
//			if (startswith(base_data->m_type, "1310")
//					^ startswith(inc_data->m_type, "1310")) {
//				return setResult("name_in_bracket", -1.0, true, sim_temp_cache);
//			} else {
//				return setResult("name_in_bracket", 1.0, true, sim_temp_cache);
//			}
//		} else {
//			return setResult("name_in_bracket", -1.0, true, sim_temp_cache);
//		}
		return setResult("name_in_bracket", -1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(
			make_pair("name_in_bracket", false));
	return false;
}

bool ruleRoad(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (base_data->m_type == "261200"
			&& base_data->m_type == inc_data->m_type) {
		RuleRoadCfg *temp_cfg = (RuleRoadCfg *) cfg;
		if (nameIsSame(base_data, inc_data, resource)
				&& sim_temp_cache->m_distance
						<= temp_cfg->m_reliable_distance) {
			return setResult("road", 1.0, true, sim_temp_cache);
		} else {
			return setResult("road", -1.0, true, sim_temp_cache);
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("road", false));
	return false;
}

bool ruleUnlackWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {

	if (base_data->m_unlack_word_set.size()
			!= inc_data->m_unlack_word_set.size()) {
		// ���ڲ�ͬ�Ĵ�
		return setResult("unlack_word", -1.0, true, sim_temp_cache);
	} else if (!checkInclude(base_data->m_unlack_word_set,
			inc_data->m_unlack_word_set, inc_data->m_name_set)
			&& !checkInclude(inc_data->m_unlack_word_set,
					base_data->m_unlack_word_set, base_data->m_name_set)) {
		// ���ڲ�ͬ�Ĵ�
		return setResult("unlack_word", -1.0, true, sim_temp_cache);
	}

//	if (base_data->m_unlack_word_set.size()
//			!= inc_data->m_unlack_word_set.size()) {
//		return setResult("unlack_word", -1.0, true, sim_temp_cache);
//	} else if (base_data->m_unlack_word_set.size() != 0) {
//		set<string>::const_iterator it = base_data->m_unlack_word_set.begin();
//		for (; it != base_data->m_unlack_word_set.end(); ++it) {
//			if (inc_data->m_unlack_word_set.find(*it)
//					== inc_data->m_unlack_word_set.end()) {
//				// ���ڲ�ͬ�Ĵ�
//				return setResult("unlack_word", -1.0, true, sim_temp_cache);
//			}
//		}
//	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("unlack_word", false));
	return false;
}

bool ruleDiffWordUnlack(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {

	string temp_main_type = "";
	set<string> temp_word_set;

	temp_word_set.insert(sim_temp_cache->m_diff_words_base_vec.begin(),
			sim_temp_cache->m_diff_words_base_vec.end());
	temp_word_set.insert(sim_temp_cache->m_diff_words_inc_vec.begin(),
			sim_temp_cache->m_diff_words_inc_vec.end());

	set<string>::iterator it = sim_temp_cache->m_same_type_set.begin();

	for (; it != sim_temp_cache->m_same_type_set.end(); ++it) {
		if ((*it).size() >= 2) {
			temp_main_type = (*it).substr(0, 2);

			_mapSet::const_iterator map_it = resource->unlackword.find(
					temp_main_type);
			if (map_it != resource->unlackword.end()) {
				set<string>::iterator word_it = temp_word_set.begin();
				for (; word_it != temp_word_set.end(); ++word_it) {
					if (map_it->second.find(*word_it) != map_it->second.end()) {
						// ���ڲ�ͬ�Ĵ�
						return setResult("diff_word_unlack", -1.0, true,
								sim_temp_cache);
					}
				}
			}
		}
	}

	sim_temp_cache->m_rule_result_map.insert(
			make_pair("diff_word_unlack", false));
	return false;
}

bool ruleDiffNum(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (sim_temp_cache->m_obj_name_set.size() > 0
			&& sim_temp_cache->m_addr_sim >= 0.52
			&& sim_temp_cache->m_distance <= sim_temp_cache->m_max_obj_distance
			&& sim_temp_cache->m_spec_obj_name_set.size() == 0) {
		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_num", false));
		return false;
	} else if (((base_data->m_num_name_set.size() == 1
			&& inc_data->m_num_name_set.size() == 0)
			|| (base_data->m_num_name_set.size() == 0
					&& inc_data->m_num_name_set.size() == 1))
			&& (startswith(sim_temp_cache->m_same_type_str, "10")
					|| startswith(sim_temp_cache->m_same_type_str, "13")
					|| startswith(sim_temp_cache->m_same_type_str, "14")
					|| startswith(sim_temp_cache->m_same_type_str, "16"))
			&& (sim_temp_cache->m_addr_sim >= 0.0
					|| sim_temp_cache->m_same_tel_set.size() > 0)) {
		// ֻ��һ���к��룬�ҵ�ַû�����Գ�ͻ���ر�����
		sim_temp_cache->m_rule_result_map.insert(make_pair("diff_num", false));
		return false;
	}

	if (base_data->m_num_name_set.size() != inc_data->m_num_name_set.size()) {
		// �������ݲ�ͬ
		if (sim_temp_cache->m_same_type_str == "201400"
				&& sim_temp_cache->m_distance <= 80) {
			// ��̩��ҽҩ��6ҩ��	��̩��ҽҩ��ʤ�����֣���úܽ����ù����ܶ϶��Ƿ���ͬ
		} else if (startswith(sim_temp_cache->m_same_type_str, "11")
				&& sim_temp_cache->m_distance <= 50) {
			// �����������ͿƼ��������ι�˾	�����������ͿƼ��������ι�˾�����ֹ�˾
		} else {
			if (base_data->m_num_name_set.size() == 1
					&& inc_data->m_num_name_set.size() == 0) {
				// ���ֻ�������������(NO.1019)	���ֻ�������NO.1019������
				if (inc_data->m_bracket_set.find(
						*(base_data->m_num_name_set.begin()))
						== inc_data->m_bracket_set.end()) {
					return setResult("diff_num", -1.0, true, sim_temp_cache);
				}
			} else if (base_data->m_num_name_set.size() == 0
					&& inc_data->m_num_name_set.size() == 1) {
				if (base_data->m_bracket_set.find(
						*(inc_data->m_num_name_set.begin()))
						== base_data->m_bracket_set.end()) {
					return setResult("diff_num", -1.0, true, sim_temp_cache);
				}
			} else {
				return setResult("diff_num", -1.0, true, sim_temp_cache);
			}
		}
	} else if (base_data->m_num_name_set.size() != 0) {
		if (!checkSetInclude(base_data->m_num_name_set,
				inc_data->m_num_name_set)
				&& !checkSetInclude(inc_data->m_num_name_set,
						base_data->m_num_name_set)) {
			// �������ݲ�ͬ
			return setResult("diff_num", -1.0, true, sim_temp_cache);
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_num", false));
	return false;
}

bool ruleDiffGovLevel(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
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

bool ruleDiffDizhi(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (startswith(sim_temp_cache->m_same_type_str, "28")) {
		if (base_data->m_dizhi != "" && inc_data->m_dizhi != ""
				&& base_data->m_dizhi != inc_data->m_dizhi) {
			return setResult("diff_dizhi", -1.0, true, sim_temp_cache);
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_dizhi", false));
	return false;
}

bool ruleDiffFendian(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (isSameFenDian(base_data, inc_data, sim_temp_cache) < 0) {
		if (((sim_temp_cache->m_addr_sim <= 0.52
				&& (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3))
				|| (base_data->m_addr_level >= 3 || inc_data->m_addr_level >= 3))
				&& (sim_temp_cache->m_distance >= 50
						&& sim_temp_cache->m_same_tel_set.size() == 0)) {
			return setResult("diff_fendian", -1.0, true, sim_temp_cache);
		}
	} else if (startswith(sim_temp_cache->m_same_type_str, "2410")) {
		// ������ѧ��***��   ������ѧ��***��  ������һ�������಻������
	} else if (sim_temp_cache->m_bracket_score < 0.0) {
		string base_tail =
				base_data->m_bracket_vec[base_data->m_bracket_vec.size() - 1];
		string inc_tail = inc_data->m_bracket_vec[inc_data->m_bracket_vec.size()
				- 1];
		if (base_tail != inc_tail) {
			if (!isSameAddr(base_data, inc_data, sim_temp_cache)
					&& (sim_temp_cache->m_distance >= 100
							&& sim_temp_cache->m_same_tel_set.size() == 0)) {
				return setResult("diff_fendian", -1.0, true, sim_temp_cache);
			}
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_dizhi", false));
	return false;
}

bool ruleDiffSoSoType(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
//	vector<string>::iterator it;
//	map<string, string>::const_iterator find_it;

//	it = sim_temp_cache->m_diff_words_base_vec.begin();
//	for (; it != sim_temp_cache->m_diff_words_base_vec.end(); ++it) {
//		find_it = resource->nameTailMap.find(*it);
//		if (find_it != resource->nameTailMap.end()) {
//			if (!isSameType(find_it->second, inc_data->m_type,
//					resource->typeMap)) {
//				return setResult("diff_soso_type", -1.0, true, sim_temp_cache);
//			}
//		}
//	}
//
//	it = sim_temp_cache->m_diff_words_inc_vec.begin();
//	for (; it != sim_temp_cache->m_diff_words_inc_vec.end(); ++it) {
//		find_it = resource->nameTailMap.find(*it);
//		if (find_it != resource->nameTailMap.end()) {
//			if (!isSameType(find_it->second, base_data->m_type,
//					resource->typeMap)) {
//				return setResult("diff_soso_type", -1.0, true, sim_temp_cache);
//			}
//		}
//	}

	if (base_data->m_soso_type != "" && inc_data->m_soso_type != "") {
		if (base_data->m_soso_main_type == inc_data->m_soso_main_type) {
			sim_temp_cache->m_rule_result_map.insert(
					make_pair("diff_soso_type", false));
			return false;
		}
	}

	if (base_data->m_soso_type != "" && base_data->m_soso_main_type != "11"
			&& !isSameType(base_data->m_soso_type, inc_data->m_type,
					resource->typeMap)) {
		return setResult("diff_soso_type", -1.0, true, sim_temp_cache);
	} else if (inc_data->m_soso_type != "" && inc_data->m_soso_main_type != "11"
			&& !isSameType(base_data->m_type, inc_data->m_soso_type,
					resource->typeMap)) {
		return setResult("diff_soso_type", -1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(
			make_pair("diff_soso_type", false));
	return false;
}

bool ruleDiffAccurateKeyWord(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	findDiffAccurateKeyword(sim_temp_cache->m_diff_words_base_vec,
			base_data->m_accurate_kw_set, resource,
			sim_temp_cache->m_accurate_kw_base_vec);
	findDiffAccurateKeyword(sim_temp_cache->m_diff_words_inc_vec,
			inc_data->m_accurate_kw_set, resource,
			sim_temp_cache->m_accurate_kw_inc_vec);

	if (sim_temp_cache->m_accurate_kw_base_vec.size() > 0
			|| sim_temp_cache->m_accurate_kw_inc_vec.size() > 0) {
		return setResult("diff_accurate_kw", -1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(
			make_pair("diff_accurate_kw", false));
	return false;
}

bool ruleDiffPark(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (startswith(sim_temp_cache->m_same_type_str, "1911")
			|| sim_temp_cache->m_obj_name_set.find("ͣ����")
					!= sim_temp_cache->m_obj_name_set.end()) {
		int base_count = 0, inc_count = 0;
		if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec, resource,
				3.0, base_count, false)
				|| hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
						resource, 3.0, inc_count, false)) {
			// �����ر�ķ�Ƶ����
			return setResult("diff_park", -1.0, true, sim_temp_cache);
		} else if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec,
				resource, 2.5, base_count, false)
				|| hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
						resource, 2.5, inc_count, false)) {
			// ���ڿ��ܵķ�Ƶ���� �� ��ַ �绰��ͬ
			if (sim_temp_cache->m_same_tel_set.size() == 0
					&& !isSameAddr(base_data, inc_data, sim_temp_cache)) {
				return setResult("diff_park", -1.0, true, sim_temp_cache);
			}
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_park", false));
	return false;
}

bool ruleDiffBuildType(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	bool base_is_building = false, inc_is_building = false;
	if (base_data->m_sub_type == "2812"
			|| base_data->m_soso_sub_type == "2812") {
		base_is_building = true;
	}

	if (inc_data->m_sub_type == "2812" || inc_data->m_soso_sub_type == "2812") {
		inc_is_building = true;
	}

	if (sim_temp_cache->m_same_type_str != "2812"
			&& (base_is_building ^ inc_is_building)) {
		return setResult("diff_build_type", -1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(
			make_pair("diff_build_type", false));
	return false;
}

bool ruleTrain(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	bool base_is_building = false, inc_is_building = false;
	if (base_data->m_type == "271016" && inc_data->m_type == "271016") {
		string short_name = inc_data->m_format_name;
		string long_name = base_data->m_format_name;
		if (base_data->m_format_name.size() < inc_data->m_format_name.size()) {
			short_name = base_data->m_format_name;
			long_name = inc_data->m_format_name;
		}

		string temp_name = short_name + "վ";
		if (temp_name == long_name) {

			return setResult("same_train_name", 1.0, true, sim_temp_cache);
		}
	}

	sim_temp_cache->m_rule_result_map.insert(
			make_pair("same_train_name", false));
	return false;
}

bool ruleSelfSim(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	double threshold = 0.52;

	sim_temp_cache->m_is_mode_self = true;

	if (nameIsSame(base_data, inc_data, resource)
			&& sim_temp_cache->m_same_type_str == "271016") {
		sim_temp_cache->m_rule_result_map.insert(make_pair("self_sim", false));
		return false;
	}

	if (startswith(sim_temp_cache->m_same_type_str, "2610")) {
		if (base_data->m_del_punc_name != inc_data->m_del_punc_name) {
			return setResult("self_sim", -1.0, true, sim_temp_cache);
		}
	}

	if (!nameIsSame(base_data, inc_data, resource)) {
		if (startswith(sim_temp_cache->m_same_type_str, "2410")) {
			if (sim_temp_cache->m_distance > 650) {
				return setResult("self_sim", -1.0, true, sim_temp_cache);
			}
		} else if ((sim_temp_cache->m_addr_sim < 0.52
				&& sim_temp_cache->m_same_tel_set.size() == 0)
				&& sim_temp_cache->m_distance > 150) {
			return setResult("self_sim", -1.0, true, sim_temp_cache);
		}
	}

	//cout << "1---" << endl;

	if (base_data->m_dpmark_set.size() > 0
			|| inc_data->m_dpmark_set.size() > 0) {
		if (!checkSetInclude(base_data->m_dpmark_set, inc_data->m_dpmark_set)
				|| !checkSetInclude(inc_data->m_dpmark_set,
						base_data->m_dpmark_set)) {
			// �г�ͻ�Ĵ�
			return setResult("self_sim", -1.0, true, sim_temp_cache);
		}
	}

	//cout << "2---" << endl;

	if (sim_temp_cache->m_spec_obj_name_set.size() > 0) {
		// ����������
		if (base_data->m_del_punc_name != inc_data->m_del_punc_name) {
			return setResult("self_sim", -1.0, true, sim_temp_cache);
		} else if (sim_temp_cache->m_distance > 10
				&& !isSameAddr(base_data, inc_data, sim_temp_cache)) {
			return setResult("self_sim", -1.0, true, sim_temp_cache);
		}
	}

	//cout << "3---" << endl;

	if ((base_data->m_source == "base" || base_data->m_source == "mapabc.com")
			&& (inc_data->m_source == "base"
					|| inc_data->m_source == "mapabc.com")) {
		if (sim_temp_cache->m_len_diff > 4 && sim_temp_cache->m_addr_sim < 0.52
				&& !sim_temp_cache->m_name_is_include
				&& sim_temp_cache->m_same_tel_set.size() == 0) {
			return setResult("self_sim", -1.0, true, sim_temp_cache);
		}
	}

	//cout << "4---" << endl;
	if (sim_temp_cache->m_same_type_str == ""
			&& !nameIsSame(base_data, inc_data, resource)) {
		sim_temp_cache->m_spec_score += -(pow(2.71828,
				(sim_temp_cache->m_distance / 150.0)) - 1)
				/ (pow(2.71828, sim_temp_cache->m_distance / 150.0) + 5.21)
				* sim_temp_cache->m_name_sub_same_score * 0.625;
	}

	//cout << "5---" << endl;

	map<string, string>::const_iterator it1, it2;
	it1 = resource->transWord.find(base_data->m_del_punc_name);
	it2 = resource->transWord.find(inc_data->m_del_punc_name);
	if (it1 != resource->transWord.end() && it2 != resource->transWord.end()
			&& it1->second == it2->second) {
		// ͬ��� ��������
		sim_temp_cache->m_rule_result_map.insert(make_pair("self_sim", false));
		return false;
	}

//	if (sim_temp_cache->m_len_diff >= 8) {
//		return setResult("self_sim", -1.0, true, sim_temp_cache);
//	}

	if (sim_temp_cache->m_len_diff >= 6 && sim_temp_cache->m_addr_sim < 0.52
			&& !sim_temp_cache->m_name_is_include
			&& sim_temp_cache->m_same_tel_set.size() == 0) {
		if (base_data->m_main_type == "13" && inc_data->m_main_type == "13"
				&& base_data->m_sub_type != "1399"
				&& inc_data->m_sub_type != "1399"
				&& base_data->m_sub_type != inc_data->m_sub_type) {
			if (base_data->m_del_punc_name != inc_data->m_del_punc_name) {
				return setResult("self_sim", -1.0, true, sim_temp_cache);
			}
		}
	}

	//cout << "6---" << endl;

	if (sim_temp_cache->m_len_diff >= 8 && sim_temp_cache->m_addr_sim < 0.52
			&& !sim_temp_cache->m_name_is_include
			&& sim_temp_cache->m_same_tel_set.size() == 0) {
//		if (!isSameAddr(base_data, inc_data, sim_temp_cache)) {
//			return setResult("self_sim", -1.0, true, sim_temp_cache);
//		} else if (startswith(sim_temp_cache->m_same_type_str, "11")) {
//			return setResult("self_sim", -1.0, true, sim_temp_cache);
//		}

		int lendiff_bigram = (int) levDis(base_data->m_bigram_name,
				inc_data->m_bigram_name);
		if (lendiff_bigram >= 4) {
			if (!isSameAddr(base_data, inc_data, sim_temp_cache)) {
				// ��ַ������
				sim_temp_cache->m_addr_score += -(pow(2.71828,
						(sim_temp_cache->m_distance / 300.0)) - 1)
						/ (pow(2.71828, sim_temp_cache->m_distance / 300.0)
								+ 5.21) * resource->max_idf * 2
						- resource->max_idf;
			}
		}
	}

	//cout << "7---" << endl;

	if (base_data->m_source == inc_data->m_source) {
		if (base_data->m_del_punc_name != inc_data->m_del_punc_name
				&& (!isSameAddr(base_data, inc_data, sim_temp_cache)
						&& sim_temp_cache->m_same_tel_set.size() == 0)) {
			sim_temp_cache->m_addr_score += -(pow(2.71828,
					(sim_temp_cache->m_distance / 300.0)) - 1)
					/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
					* resource->max_idf * 2;
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("self_sim", false));
	return false;
}

bool ruleDiffName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {

	if (sim_temp_cache->m_obj_name_set.size() == 0
			|| !isSameAddr(base_data, inc_data, sim_temp_cache)) {
		if (sim_temp_cache->m_name_score < 0) {
			return setResult("diff_name", -1.0, true, sim_temp_cache);
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_name", false));
	return false;
}

bool ruleNotFreqWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {

	if (startswith(sim_temp_cache->m_same_type_str, "2810")) {
		int base_count = 0, inc_count = 0;
		bool base_has_nofreq = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_base_vec, resource, 3.0,
				base_count, false);
		bool inc_has_nofreq = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_inc_vec, resource, 3.0, inc_count,
				false);
		if (base_has_nofreq || inc_has_nofreq) {
			return setResult("diff_not_freq", -1.0, true, sim_temp_cache);
		}
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("diff_not_freq", false));
	return false;
}

bool ruleChnSim(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	sim_temp_cache->m_is_mode_chn = true;

	if (base_data->m_format_name.find(inc_data->m_format_name) == string::npos
			&& inc_data->m_format_name.find(base_data->m_format_name)
					!= string::npos) {
		return setResult("chn_sim", -1.0, true, sim_temp_cache);
	}

	sim_temp_cache->m_rule_result_map.insert(make_pair("chn_sim", false));
	return false;
}

//==================================================
// ���꽻�洦���Ĺ�����
//==================================================
// ����ͬ���ֳ����ڶԷ������еĴʷŵ�����ͬ��
void dealInBracket(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	// ��﹫˾���ݽֵ�  ��﹫˾�����ݽ֣�
	filteIfNotNull(sim_temp_cache->m_diff_words_base_vec,
			sim_temp_cache->m_sub_same_words_base_vec, inc_data->m_bracket_set);

	filteIfNotNull(sim_temp_cache->m_diff_words_inc_vec,
			sim_temp_cache->m_sub_same_words_inc_vec, base_data->m_bracket_set);

	// use pinyin to filte word that find in other poi's brackets
	filteIfNotNull(sim_temp_cache->m_diff_words_base_vec,
			sim_temp_cache->m_sub_same_words_base_vec, inc_data->m_bracket_vec,
			resource->pinyin);
	filteIfNotNull(sim_temp_cache->m_diff_words_inc_vec,
			sim_temp_cache->m_sub_same_words_inc_vec, base_data->m_bracket_vec,
			resource->pinyin);
}

// ����ͬ����ƴ�������ڶԷ������еĴʷŵ�����ͬ��
void dealPinYin(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (startswith(sim_temp_cache->m_same_type_str, "2810")) {
		return;
	}

	pyVecIsSame(sim_temp_cache->m_diff_words_base_vec,
			sim_temp_cache->m_diff_words_inc_vec,
			sim_temp_cache->m_sub_same_words_base_vec,
			sim_temp_cache->m_sub_same_words_inc_vec, resource->pinyin, 0);

	if (startswith(sim_temp_cache->m_same_type_str, "22")
			|| startswith(sim_temp_cache->m_same_type_str, "23")) {
		pyVecIsSame(sim_temp_cache->m_diff_words_base_vec,
				sim_temp_cache->m_diff_words_inc_vec,
				sim_temp_cache->m_sub_diff_words_base_vec,
				sim_temp_cache->m_sub_diff_words_inc_vec, resource->pinyin, 0);
	} else {
		pyVecIsSame(sim_temp_cache->m_diff_words_base_vec,
				sim_temp_cache->m_diff_words_inc_vec,
				sim_temp_cache->m_sub_diff_words_base_vec,
				sim_temp_cache->m_sub_diff_words_inc_vec, resource->pinyin, 1);
	}
}

// ����ͬ����ȥ���������׺�ֺ���ͬ�Ĵʷŵ�����ͬ��
void dealInclude(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (sim_temp_cache->m_same_type_str == "261400") {
		return;
	} else if (startswith(sim_temp_cache->m_same_type_str, "2810")) {
		return;
	} else if (startswith(sim_temp_cache->m_same_type_str, "2210")) {
		return;
	}

	filteInclude(sim_temp_cache->m_diff_words_base_vec,
			sim_temp_cache->m_diff_words_inc_vec,
			sim_temp_cache->m_sub_same_words_base_vec,
			sim_temp_cache->m_sub_same_words_inc_vec, resource->endList);

	filteInclude(sim_temp_cache->m_diff_words_inc_vec,
			sim_temp_cache->m_diff_words_base_vec,
			sim_temp_cache->m_sub_same_words_inc_vec,
			sim_temp_cache->m_sub_same_words_base_vec, resource->endList);
}

// ����ͬ���ִʰ���������ͬ���ֵĴʷŵ�����ͬ��
void dealIncludeInSame(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	findSameInList(sim_temp_cache->m_same_words_vec,
			sim_temp_cache->m_diff_words_base_vec,
			sim_temp_cache->m_sub_same_words_base_vec);
	findSameInList(sim_temp_cache->m_same_words_vec,
			sim_temp_cache->m_diff_words_inc_vec,
			sim_temp_cache->m_sub_same_words_inc_vec);
}

// ���а����ķֵ���Ϣ�Ӳ�ͬ����ȥ��
void dealFendian(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	vector<string>::iterator it;
	string temp_word;

	if (inc_data->m_fendian != "") {
		temp_word = inc_data->m_fendian;
		it = sim_temp_cache->m_diff_words_base_vec.begin();
		while (it != sim_temp_cache->m_diff_words_base_vec.end()) {
			if (wordInclude(temp_word, *it, resource->endList)) {
				sim_temp_cache->m_sub_same_words_base_vec.push_back(*it);
				it = sim_temp_cache->m_diff_words_base_vec.erase(it);
			} else {
				++it;
			}
		}
	}

	if (base_data->m_fendian != "") {
		temp_word = base_data->m_fendian;
		it = sim_temp_cache->m_diff_words_inc_vec.begin();
		while (it != sim_temp_cache->m_diff_words_inc_vec.end()) {
			if (wordInclude(temp_word, *it, resource->endList)) {
				sim_temp_cache->m_sub_same_words_inc_vec.push_back(*it);
				it = sim_temp_cache->m_diff_words_inc_vec.erase(it);
			} else {
				++it;
			}
		}
	}

}

// ��ֻ��һ�ߵĲ�ͬ���ֳ��ֵ�·��Ϣ����Ϊ�õ�·��Ϣ���㲻ͬ���ŵ�����ͬ��
void dealOneSideRoad(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (base_data->m_main_type != "26" && inc_data->m_main_type != "26") {
		if (sim_temp_cache->m_diff_words_base_vec.size() == 1
				&& sim_temp_cache->m_diff_words_inc_vec.size() == 0) {
			if (resource->road.find(sim_temp_cache->m_diff_words_base_vec[0])
					!= resource->road.end()) {
				sim_temp_cache->m_sub_same_words_base_vec.push_back(
						sim_temp_cache->m_diff_words_base_vec[0]);
				sim_temp_cache->m_diff_words_base_vec.erase(
						sim_temp_cache->m_diff_words_base_vec.begin());
			}
		} else if (sim_temp_cache->m_diff_words_base_vec.size() == 0
				&& sim_temp_cache->m_diff_words_inc_vec.size() == 1) {
			if (resource->road.find(sim_temp_cache->m_diff_words_inc_vec[0])
					!= resource->road.end()) {
				sim_temp_cache->m_sub_same_words_inc_vec.push_back(
						sim_temp_cache->m_diff_words_inc_vec[0]);
				sim_temp_cache->m_diff_words_inc_vec.erase(
						sim_temp_cache->m_diff_words_inc_vec.begin());
			}
		}
	}
}

void dealTypeWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	set<string>::iterator it;

	// ��ͬ����
	if (sim_temp_cache->m_same_words_vec.size() > 1) {
		for (it = sim_temp_cache->m_same_type_set.begin();
				it != sim_temp_cache->m_same_type_set.end(); ++it) {
			_mapSet::const_iterator map_it = resource->typeword.find(*it);
			if (map_it != resource->typeword.end()) {
				vector<string>::iterator word_it =
						sim_temp_cache->m_same_words_vec.begin();
				while (word_it != sim_temp_cache->m_same_words_vec.end()) {
					if (map_it->second.find(*word_it) != map_it->second.end()) {
						// �����ʹ�
						sim_temp_cache->m_sub_same_words_base_vec.push_back(
								*word_it);
						sim_temp_cache->m_sub_same_words_inc_vec.push_back(
								*word_it);
						word_it = sim_temp_cache->m_same_words_vec.erase(
								word_it);
					} else {
						++word_it;
					}
				}
			}
		}
	}

	// ��ͬ����
	for (it = sim_temp_cache->m_same_type_set.begin();
			it != sim_temp_cache->m_same_type_set.end(); ++it) {
		_mapSet::const_iterator map_it = resource->typeword.find(*it);
		if (map_it != resource->typeword.end()) {
			vector<string>::iterator word_it =
					sim_temp_cache->m_diff_words_base_vec.begin();
			while (word_it != sim_temp_cache->m_diff_words_base_vec.end()) {
				if (map_it->second.find(*word_it) != map_it->second.end()) {
					// �����ʹ�
					sim_temp_cache->m_sub_diff_words_base_vec.push_back(
							*word_it);
					word_it = sim_temp_cache->m_diff_words_base_vec.erase(
							word_it);
				} else {
					++word_it;
				}
			}
		}
	}

	for (it = sim_temp_cache->m_same_type_set.begin();
			it != sim_temp_cache->m_same_type_set.end(); ++it) {
		_mapSet::const_iterator map_it = resource->typeword.find(*it);
		if (map_it != resource->typeword.end()) {
			vector<string>::iterator word_it =
					sim_temp_cache->m_diff_words_inc_vec.begin();
			while (word_it != sim_temp_cache->m_diff_words_inc_vec.end()) {
				if (map_it->second.find(*word_it) != map_it->second.end()) {
					// �����ʹ�
					sim_temp_cache->m_sub_diff_words_inc_vec.push_back(
							*word_it);
					word_it = sim_temp_cache->m_diff_words_inc_vec.erase(
							word_it);
				} else {
					++word_it;
				}
			}
		}
	}
}

void dealSameTypeWord(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	vector<string>::iterator it = sim_temp_cache->m_same_words_vec.begin();

	_mapSet::const_iterator map_it = resource->typeword.find("01");
	if (map_it != resource->typeword.end()) {
		while (it != sim_temp_cache->m_same_words_vec.end()) {
			if (map_it->second.find(*it) != map_it->second.end()
					&& sim_temp_cache->m_same_words_vec.size() > 1) {
				sim_temp_cache->m_sub_same_words_base_vec.push_back(*it);
				sim_temp_cache->m_sub_same_words_inc_vec.push_back(*it);
				it = sim_temp_cache->m_same_words_vec.erase(it);
			} else {
				++it;
			}
		}
	}

}

void dealChainName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (sim_temp_cache->m_obj_name_set.size() > 0
			&& sim_temp_cache->m_same_words_vec.size() > 1) {
		vector<string>::iterator it = sim_temp_cache->m_same_words_vec.begin();

		while (it != sim_temp_cache->m_same_words_vec.end()) {
			if (sim_temp_cache->m_obj_name_set.find(*it)
					!= sim_temp_cache->m_obj_name_set.end()) {
				sim_temp_cache->m_sub_same_words_base_vec.push_back(*it);
				sim_temp_cache->m_sub_same_words_inc_vec.push_back(*it);
				it = sim_temp_cache->m_same_words_vec.erase(it);
			} else {
				++it;
			}
		}
	}
}

void dealGovDistrict(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (sim_temp_cache->m_same_words_vec.size() < 2) {
		return;
	}
	sim_temp_cache->m_sub_same_words_base_vec.clear();
	sim_temp_cache->m_sub_same_words_inc_vec.clear();

	_vectorit srcit;

	string tempTail("");

	srcit = sim_temp_cache->m_same_words_vec.begin();
	while (srcit != sim_temp_cache->m_same_words_vec.end()) {
		if ((*srcit).length() < 2) {
			++srcit;
			continue;
		}

		tempTail = (*srcit).substr((*srcit).length() - 2, 2);
		if (resource->govTailSet.find(tempTail) != resource->govTailSet.end()) {
			//sim_temp_cache->m_sub_same_words_base_vec.push_back(*srcit);
			//sim_temp_cache->m_sub_same_words_inc_vec.push_back(*srcit);
			srcit = sim_temp_cache->m_same_words_vec.erase(srcit);
			if (sim_temp_cache->m_same_words_vec.size() < 2) {
				return;
			}
		} else {
			++srcit;
		}
	}

	if (startswith(sim_temp_cache->m_same_type_str, "12")
			&& sim_temp_cache->m_distance <= 150) {
		srcit = sim_temp_cache->m_diff_words_base_vec.begin();
		while (srcit != sim_temp_cache->m_diff_words_base_vec.end()) {
			if ((*srcit).length() < 2) {
				++srcit;
				continue;
			}

			tempTail = (*srcit).substr((*srcit).length() - 2, 2);
			if (resource->govTailSet.find(tempTail)
					!= resource->govTailSet.end()) {
				sim_temp_cache->m_sub_diff_words_base_vec.push_back(*srcit);
				srcit = sim_temp_cache->m_diff_words_base_vec.erase(srcit);
			} else {
				++srcit;
			}
		}

		srcit = sim_temp_cache->m_diff_words_inc_vec.begin();
		while (srcit != sim_temp_cache->m_diff_words_inc_vec.end()) {
			if ((*srcit).length() < 2) {
				++srcit;
				continue;
			}

			tempTail = (*srcit).substr((*srcit).length() - 2, 2);
			if (resource->govTailSet.find(tempTail)
					!= resource->govTailSet.end()) {
				sim_temp_cache->m_sub_diff_words_inc_vec.push_back(*srcit);
				srcit = sim_temp_cache->m_diff_words_inc_vec.erase(srcit);
			} else {
				++srcit;
			}
		}
	}
}

void dealInName(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	filter(base_data->m_format_name, sim_temp_cache->m_diff_words_inc_vec,
			sim_temp_cache->m_sub_same_words_inc_vec);
	filter(inc_data->m_format_name, sim_temp_cache->m_diff_words_base_vec,
			sim_temp_cache->m_sub_same_words_base_vec);
}

//==================================================
// �����������ƶȼ���
//==================================================
void computeNameScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	// same
	sim_temp_cache->m_name_same_score = 2
			* sumIDF(sim_temp_cache->m_same_words_vec, resource) * 2;

	// sub same
	sim_temp_cache->m_name_sub_same_score += sumIDF(
			sim_temp_cache->m_sub_same_words_base_vec, resource) * 2;
	sim_temp_cache->m_name_sub_same_score += sumIDF(
			sim_temp_cache->m_sub_same_words_inc_vec, resource) * 2;

	// diff
	sim_temp_cache->m_name_diff_score += sumIDF(
			sim_temp_cache->m_diff_words_base_vec, resource) * 2;
	sim_temp_cache->m_name_diff_score += sumIDF(
			sim_temp_cache->m_diff_words_inc_vec, resource) * 2;

	// sub diff
	sim_temp_cache->m_name_sub_diff_score += sumIDF(
			sim_temp_cache->m_sub_diff_words_base_vec, resource) * 2;
	sim_temp_cache->m_name_sub_diff_score += sumIDF(
			sim_temp_cache->m_sub_diff_words_inc_vec, resource) * 2;

	// �����������ƶ�
	PoiSimCfg *temp_cfg = (PoiSimCfg *) cfg;
	double temp_same_score = 0.0;
	double temp_diff_score = 0.0;

//	if (sim_temp_cache->m_name_same_score < 0.001) {
//		temp_same_score = sim_temp_cache->m_name_sub_same_score;
//	} else if (sim_temp_cache->m_name_sub_same_score < 2.5) {
//		temp_same_score = sim_temp_cache->m_name_same_score
//				+ sim_temp_cache->m_name_sub_same_score;
//	} else {
//		temp_same_score = temp_cfg->m_name_same_weight
//				* sim_temp_cache->m_name_same_score
//				+ temp_cfg->m_name_sub_same_weight
//						* sim_temp_cache->m_name_sub_same_score;
//	}

	temp_same_score = temp_cfg->m_name_same_weight
			* sim_temp_cache->m_name_same_score
			+ temp_cfg->m_name_sub_same_weight
					* sim_temp_cache->m_name_sub_same_score;

//	if (sim_temp_cache->m_name_diff_score < 0.001) {
//		temp_diff_score = sim_temp_cache->m_name_sub_diff_score;
//	} else if (sim_temp_cache->m_name_sub_diff_score < 2.5) {
//		temp_diff_score = sim_temp_cache->m_name_diff_score
//				+ sim_temp_cache->m_name_sub_diff_score;
//	} else {
//		temp_diff_score = temp_cfg->m_name_diff_weight
//				* sim_temp_cache->m_name_diff_score
//				+ temp_cfg->m_name_sub_diff_weight
//						* sim_temp_cache->m_name_sub_diff_score;
//	}

	temp_diff_score = temp_cfg->m_name_diff_weight
			* sim_temp_cache->m_name_diff_score
			+ temp_cfg->m_name_sub_diff_weight
					* sim_temp_cache->m_name_sub_diff_score;

	sim_temp_cache->m_name_score += temp_same_score - temp_diff_score;

	sim_temp_cache->m_total_same_score += temp_same_score;
	sim_temp_cache->m_total_diff_score += temp_diff_score;
}

void computeTelScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (sim_temp_cache->m_type_score < 0) {
		return;
	}

	if (sim_temp_cache->m_same_tel_set.size() > 0) {
		sim_temp_cache->m_tel_score += resource->max_idf * 2;
	}

	if (startswith(sim_temp_cache->m_same_type_str, "13")
			&& sim_temp_cache->m_same_type_str != "131000") {
		// ����
		sim_temp_cache->m_tel_score = sim_temp_cache->m_tel_score * 0.025;
	} else if (startswith(sim_temp_cache->m_same_type_str, "12")
			|| startswith(sim_temp_cache->m_same_type_str, "14")) {
		// �������������
		sim_temp_cache->m_tel_score = sim_temp_cache->m_tel_score * 0.025;
	} else if (startswith(sim_temp_cache->m_same_type_str, "11")) {
		// ��˾
		sim_temp_cache->m_tel_score = sim_temp_cache->m_tel_score * 0.05;
	}

	if (sim_temp_cache->m_obj_name_set.size() > 0
			&& sim_temp_cache->m_spec_obj_name_set.size() == 0
			&& sim_temp_cache->m_addr_sim >= 0.52) {
		sim_temp_cache->m_tel_score += resource->max_idf * 1.0;
	}

	if (sim_temp_cache->m_tel_score > 0) {
		sim_temp_cache->m_total_same_score += sim_temp_cache->m_tel_score;
	} else {
		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_tel_score;
	}
}

void computeTypeScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (sim_temp_cache->m_same_type_str == "13") {
		// ����
		if (base_data->m_sub_type != inc_data->m_sub_type) {
			sim_temp_cache->m_type_score += -(pow(2.71828,
					(sim_temp_cache->m_distance / 300.0)) - 1)
					/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
					* resource->max_idf * 0.625;
		}
	} else if (sim_temp_cache->m_same_type_str == "16") {
		// ����
		if (base_data->m_sub_type == "1699" || inc_data->m_sub_type == "1699") {
			sim_temp_cache->m_type_score += 0.0;
		} else if (base_data->m_sub_type != inc_data->m_sub_type) {
			sim_temp_cache->m_type_score += -(pow(2.71828,
					(sim_temp_cache->m_distance / 300.0)) - 1)
					/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
					* resource->max_idf * 0.8;
		}
	} else if (sim_temp_cache->m_same_type_str == "26") {
		// ������ַ
		if (base_data->m_sub_type == "2699" || inc_data->m_sub_type == "2699") {
			sim_temp_cache->m_type_score += 0.0;
		} else if (base_data->m_sub_type == "2613"
				^ inc_data->m_sub_type == "2613") {
			sim_temp_cache->m_type_score += -resource->max_idf * 1.5;
		}
	} else if (base_data->m_main_type == "11" ^ inc_data->m_main_type == "11") {
		// ��˾
		if (sim_temp_cache->m_len_diff >= 10
				&& ((base_data->m_main_type == "11"
						&& inc_data->m_main_type == "12")
						|| (base_data->m_main_type == "12"
								&& inc_data->m_main_type == "11"))) {
			sim_temp_cache->m_type_score += -resource->max_idf;
		}

		sim_temp_cache->m_type_score += -(pow(2.71828,
				(sim_temp_cache->m_distance / 300.0)) - 1)
				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
				* resource->max_idf * 0.25;
	} else if (base_data->m_soso_main_type != ""
			&& inc_data->m_soso_main_type != ""
			&& (base_data->m_soso_main_type == "11"
					^ inc_data->m_soso_main_type == "11")) {
		// ��˾
		sim_temp_cache->m_type_score += -(pow(2.71828,
				(sim_temp_cache->m_distance / 300.0)) - 1)
				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
				* resource->max_idf * 0.25;
	} else if (base_data->m_main_type == "99"
			|| inc_data->m_main_type == "99") {
		// ����
		sim_temp_cache->m_type_score += -(pow(2.71828,
				(sim_temp_cache->m_distance / 300.0)) - 1)
				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
				* resource->max_idf * 0.25;
	}

	if (sim_temp_cache->m_type_score > 0) {
		sim_temp_cache->m_total_same_score += sim_temp_cache->m_type_score;
	} else {
		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_type_score;
	}
}

//void computeAddrScore(const PreProcData *base_data, const PreProcData *inc_data,
//		const void *cfg, const Resource *resource,
//		SimTempCache *sim_temp_cache) {
//	if (sim_temp_cache->m_is_mode_chn) {
//		return;
//	}
//
//	double threshold = 0.15;
//
//	if (sim_temp_cache->m_distance > 300
//			&& !isSameAddr(base_data, inc_data, sim_temp_cache)) {
//		sim_temp_cache->m_addr_score += -(pow(2.71828,
//				(sim_temp_cache->m_distance / 300.0)) - 1)
//				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
//				* resource->max_idf * 4.5;
//	} else if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3) {
//		if (sim_temp_cache->m_obj_name_set.size() == 0) {
//			// �����������
//			if (sim_temp_cache->m_addr_sim > threshold) {
//				// ��ַ����
//				sim_temp_cache->m_addr_score += pow(2.71828,
//						(sim_temp_cache->m_distance / 300.0))
//						/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
//						* resource->max_idf * 0.875;
//			} else if (sim_temp_cache->m_addr_sim == threshold) {
//				// ��ַ��ȷ��
//				sim_temp_cache->m_addr_score += resource->max_idf * 0.125;
//			} else if (startswith(sim_temp_cache->m_same_type_str, "11")
//					&& sim_temp_cache->m_addr_sim < 0.48) {
//				// ��ַ������
//				sim_temp_cache->m_addr_score += -(pow(2.71828,
//						(sim_temp_cache->m_distance / 300.0)) - 1)
//						/ (pow(2.71828, sim_temp_cache->m_distance / 300.0)
//								+ 5.21) * resource->max_idf;
//			} else {
//				sim_temp_cache->m_addr_score += 0.0;
//			}
//		} else {
//			// ���������
//			if (sim_temp_cache->m_addr_sim < 0.0) {
//				sim_temp_cache->m_addr_score += 0.0;
//			} else if (sim_temp_cache->m_addr_sim < threshold) {
//				// ��ַ������
//				if (base_data->m_addr != "" && inc_data->m_addr != "") {
//					sim_temp_cache->m_addr_score += -(pow(2.71828,
//							(sim_temp_cache->m_distance / 300.0)) - 1)
//							/ (pow(2.71828, sim_temp_cache->m_distance / 300.0)
//									+ 5.21) * resource->max_idf * 1.5;
//				}
//			} else if (sim_temp_cache->m_addr_sim == threshold) {
//				// ��ַ��ȷ��
//				if (sim_temp_cache->m_distance
//						<= base_data->m_obj_max_distance) {
//					// ����ܽ�
//					sim_temp_cache->m_addr_score += pow(2.71828,
//							(sim_temp_cache->m_distance / 300.0))
//							/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
//							* resource->max_idf * 0.875;
//				}
//			} else {
//				// ��ַ����
//				sim_temp_cache->m_addr_score += pow(2.71828,
//						(sim_temp_cache->m_distance / 300.0))
//						/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
//						* resource->max_idf * 1.5;
//			}
//		}
//	}
//
//	if (sim_temp_cache->m_addr_sim < threshold && base_data->m_addr_level < 3
//			&& inc_data->m_addr_level < 3) {
//		if (sim_temp_cache->m_distance > 300.0
//				&& (base_data->m_bracket_vec.size() > 0
//						^ inc_data->m_bracket_vec.size())) {
//			sim_temp_cache->m_addr_score += -resource->max_idf * 1.5;
//		}
//	}
//
//	if (sim_temp_cache->m_addr_score > 0) {
//		sim_temp_cache->m_total_same_score += sim_temp_cache->m_addr_score;
//	} else {
//		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_addr_score;
//	}
//}

void computeAddrScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (sim_temp_cache->m_is_mode_chn) {
		return;
	}

	double threshold = 0.13;

	if (sim_temp_cache->m_distance > 300
			&& sim_temp_cache->m_addr_sim < threshold) {
		sim_temp_cache->m_addr_score += -(pow(2.71828,
				(sim_temp_cache->m_distance / 300.0)) - 1)
				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
				* resource->max_idf * 4.5;
	} else if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3) {
		if (sim_temp_cache->m_obj_name_set.size() == 0) {
			// �����������
			if (sim_temp_cache->m_addr_sim > threshold) {
				// ��ַ����
				sim_temp_cache->m_addr_score += pow(2.71828,
						(sim_temp_cache->m_distance / 300.0))
						/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
						* resource->max_idf * 0.875;
			} else if (sim_temp_cache->m_addr_sim == threshold) {
				// ��ַ��ȷ��
				sim_temp_cache->m_addr_score += resource->max_idf * 0.125;
			} else if (startswith(sim_temp_cache->m_same_type_str, "11")
					&& sim_temp_cache->m_addr_sim < 0.48) {
				// ��ַ������
				sim_temp_cache->m_addr_score += -(pow(2.71828,
						(sim_temp_cache->m_distance / 300.0)) - 1)
						/ (pow(2.71828, sim_temp_cache->m_distance / 300.0)
								+ 5.21) * resource->max_idf;
			} else {
				sim_temp_cache->m_addr_score += 0.0;
			}
		} else {
			// ���������
			if (sim_temp_cache->m_addr_sim < 0.0) {
				sim_temp_cache->m_addr_score += 0.0;
			} else if (sim_temp_cache->m_addr_sim < threshold) {
				// ��ַ������
				if (base_data->m_addr != "" && inc_data->m_addr != "") {
					sim_temp_cache->m_addr_score += -(pow(2.71828,
							(sim_temp_cache->m_distance / 300.0)) - 1)
							/ (pow(2.71828, sim_temp_cache->m_distance / 300.0)
									+ 5.21) * resource->max_idf * 1.5;
				}
			} else if (sim_temp_cache->m_addr_sim == threshold) {
				// ��ַ��ȷ��
				if (sim_temp_cache->m_distance
						<= base_data->m_obj_max_distance) {
					// ����ܽ�
					sim_temp_cache->m_addr_score += pow(2.71828,
							(sim_temp_cache->m_distance / 300.0))
							/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
							* resource->max_idf * 0.875;
				}
			} else {
				// ��ַ����
				sim_temp_cache->m_addr_score += pow(2.71828,
						(sim_temp_cache->m_distance / 300.0))
						/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
						* resource->max_idf * 1.5;
			}
		}
	}

	if (sim_temp_cache->m_addr_sim < threshold && base_data->m_addr_level < 3
			&& inc_data->m_addr_level < 3) {
		if (sim_temp_cache->m_distance > 300.0
				&& (base_data->m_bracket_vec.size() > 0
						^ inc_data->m_bracket_vec.size())) {
			sim_temp_cache->m_addr_score += -resource->max_idf * 1.5;
		}
	}

	if (sim_temp_cache->m_obj_name_set.size() > 0
			&& sim_temp_cache->m_spec_obj_name_set.size() == 0
			&& sim_temp_cache->m_addr_sim >= threshold) {
		sim_temp_cache->m_addr_score += resource->max_idf * 1.5;
	}

	if (sim_temp_cache->m_addr_score > 0) {
		sim_temp_cache->m_total_same_score += sim_temp_cache->m_addr_score;
	} else {
		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_addr_score;
	}
}

//==================================================
// �ر��������ƶȼ���
//==================================================
void computeSameTailScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (endswith(base_data->m_format_name, inc_data->m_format_name)
			|| endswith(inc_data->m_format_name, base_data->m_format_name)) {
		if (startswith(sim_temp_cache->m_same_type_str, "22")) {
			sim_temp_cache->m_tail_score += resource->max_idf
					* pow(2.71828,
							-(pow(6.01828, sim_temp_cache->m_distance / 1480.0)
									- 1));
		} else if (startswith(sim_temp_cache->m_same_type_str, "23")
				&& sim_temp_cache->m_distance <= 100) {
			sim_temp_cache->m_tail_score += resource->max_idf
					* pow(10.71828,
							-(pow(9.01828, sim_temp_cache->m_distance / 880.0)
									- 1));
		} else if (startswith(sim_temp_cache->m_same_type_str, "12")) {
			sim_temp_cache->m_tail_score += resource->max_idf
					* pow(3.71828,
							-(pow(3.51828, sim_temp_cache->m_distance / 2980.0)
									- 1));
		} else {
			sim_temp_cache->m_tail_score += resource->max_idf
					* pow(10.71828,
							-(pow(9.51828, sim_temp_cache->m_distance / 680.0)
									- 1));
		}
	} else if (endswith(base_data->m_bigram_name, inc_data->m_bigram_name)
			|| endswith(inc_data->m_bigram_name, base_data->m_bigram_name)) {
		sim_temp_cache->m_tail_score +=
				resource->max_idf
						* pow(3.71828,
								-(pow(3.51828,
										sim_temp_cache->m_distance / 2980.0) - 1));
	}

	if (sim_temp_cache->m_tail_score > 0) {
		sim_temp_cache->m_total_same_score += sim_temp_cache->m_tail_score;
	} else {
		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_tail_score;
	}
}

void computeAccurateTypeScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if ((base_data->m_accurate_keyword == ""
			&& inc_data->m_accurate_keyword != ""
			&& inc_data->m_accurate_keyword.find("��У") == std::string::npos
			&& inc_data->m_accurate_keyword.find("У��") == std::string::npos
			&& inc_data->m_accurate_keyword.find("�ֲ�") == std::string::npos)
			|| (base_data->m_accurate_keyword != ""
					&& inc_data->m_accurate_keyword == ""
					&& base_data->m_accurate_keyword.find("��У")
							== std::string::npos
					&& base_data->m_accurate_keyword.find("У��")
							== std::string::npos
					&& base_data->m_accurate_keyword.find("�ֲ�")
							== std::string::npos)) {
		sim_temp_cache->m_accurate_type_score +=
				-sim_temp_cache->m_total_diff_score
						* (pow(2.71828, (sim_temp_cache->m_distance / 150.0))
								- 1)
						/ (pow(2.71828, sim_temp_cache->m_distance / 150.0)
								+ 5.21) * 0.75;
	} else if (base_data->m_accurate_keyword != ""
			&& inc_data->m_accurate_keyword != ""
			&& base_data->m_accurate_keyword != inc_data->m_accurate_keyword) {
		sim_temp_cache->m_accurate_type_score += -(pow(2.71828,
				(sim_temp_cache->m_distance / 150.0)) - 1)
				/ (pow(2.71828, sim_temp_cache->m_distance / 150.0) + 5.21)
				* sim_temp_cache->m_total_same_score * 0.625;
	}

	if (sim_temp_cache->m_accurate_type_score > 0) {
		sim_temp_cache->m_total_same_score +=
				sim_temp_cache->m_accurate_type_score;
	} else {
		sim_temp_cache->m_total_diff_score +=
				-sim_temp_cache->m_accurate_type_score;
	}
}

void computeSpecTypeScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {

	if (sim_temp_cache->m_type_score < 0.0) {
		// ���Ͳ�һ�����ر���
		if ((!startswith(sim_temp_cache->m_same_type_str, "11")
				&& (base_data->m_main_type == "11"
						|| inc_data->m_main_type == "11"
						|| base_data->m_soso_main_type == "11"
						|| inc_data->m_soso_main_type == "11"))
				&& (!isSameAddr(base_data, inc_data, sim_temp_cache)
						|| sim_temp_cache->m_distance > 500)
				&& (sim_temp_cache->m_name_diff_score > 2.0
						|| sim_temp_cache->m_name_sub_diff_score > 2.0)) {
			sim_temp_cache->m_spec_score += -resource->max_idf * 0.375;
		} else if (sim_temp_cache->m_same_type_set.find("990000")
				!= sim_temp_cache->m_same_type_set.end()
				|| sim_temp_cache->m_same_type_str == "990000") {
			// �������� ���� ���͵����
			if (sim_temp_cache->m_addr_sim < 0.52
					|| sim_temp_cache->m_distance > 500) {
				sim_temp_cache->m_spec_score += -resource->max_idf;
			}
		}
	} else if (sim_temp_cache->m_is_mode_self
			&& startswith(sim_temp_cache->m_same_type_str, "11")) {
		int base_count = 0, inc_count = 0;
		bool base_has_nofreq = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_base_vec, resource, 0.8,
				base_count, false);
		bool inc_has_nofreq = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_inc_vec, resource, 0.8, inc_count,
				false);
		if ((base_has_nofreq || inc_has_nofreq)
				&& sim_temp_cache->m_len_diff >= 8) {
			sim_temp_cache->m_spec_score += -resource->max_idf * 2;
		}

		if (sim_temp_cache->m_spec_score < 0) {
			if (sim_temp_cache->m_addr_score > 0) {
				sim_temp_cache->m_total_same_score +=
						-sim_temp_cache->m_addr_score;
				sim_temp_cache->m_addr_score = 0;
			}

			if (sim_temp_cache->m_tel_score > 0) {
				sim_temp_cache->m_total_same_score +=
						-sim_temp_cache->m_tel_score;
				sim_temp_cache->m_addr_score = 0;
			}

		}
	} else if (startswith(sim_temp_cache->m_same_type_str, "11")
			&& isSameAddr(base_data, inc_data, sim_temp_cache)
			&& (sim_temp_cache->m_name_same_score
					+ sim_temp_cache->m_name_sub_same_score)
					> resource->max_idf * 0.875) {
		sim_temp_cache->m_spec_score += -(pow(2.71828,
				(sim_temp_cache->m_distance / 150.0)) - 1)
				/ (pow(2.71828, sim_temp_cache->m_distance / 150.0) + 5.21)
				* sim_temp_cache->m_name_sub_same_score * 0.625;
	}

	if (sim_temp_cache->m_spec_score > 0) {
		sim_temp_cache->m_total_same_score += sim_temp_cache->m_spec_score;
	} else {
		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_spec_score;
	}
}

void computeDistanceScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (sim_temp_cache->m_addr_score > 0 && sim_temp_cache->m_distance > 600) {
		sim_temp_cache->m_distance_score += -(pow(2.71828,
				(sim_temp_cache->m_distance / 300.0)) - 1)
				/ (pow(2.71828, sim_temp_cache->m_distance / 300.0) + 5.21)
				* resource->max_idf * 0.375;
	}

	if (sim_temp_cache->m_distance_score > 0) {
		sim_temp_cache->m_total_same_score += sim_temp_cache->m_distance_score;
	} else {
		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_distance_score;
	}
}

void computeInBracketScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (isInclude(base_data->m_format_name, inc_data->m_raw_bracket_vec)
			|| isInclude(inc_data->m_format_name,
					base_data->m_raw_bracket_vec)) {
		sim_temp_cache->m_in_bracket_score += -resource->max_idf * 2;
	}

	if (sim_temp_cache->m_in_bracket_score > 0) {
		sim_temp_cache->m_total_same_score +=
				sim_temp_cache->m_in_bracket_score;
	} else {
		sim_temp_cache->m_total_diff_score +=
				-sim_temp_cache->m_in_bracket_score;
	}
}

void computeSameNumScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	if (base_data->m_num_name_set.size() > 0) {
		sim_temp_cache->m_same_num_score += pow(2.71828,
				(sim_temp_cache->m_distance / 300.0))
				/ pow(9.71828, sim_temp_cache->m_distance / 300.0)
				* resource->max_idf * 2;
	}

	if (sim_temp_cache->m_same_num_score > 0) {
		sim_temp_cache->m_total_same_score += sim_temp_cache->m_same_num_score;
	} else {
		sim_temp_cache->m_total_diff_score += -sim_temp_cache->m_same_num_score;
	}
}

void computeSameAT(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {

	if (sim_temp_cache->m_name_is_include) {
		// soso���ͳ�ͻ����Ϊ�з���
		if (isConflict(base_data, inc_data, resource)) {
			return;
		}

		// ��˾���о������ۺ��з���
		if (startswith(sim_temp_cache->m_same_type_str, "11")
				|| startswith(sim_temp_cache->m_same_type_str, "12")) {
			return;
		} else if (sim_temp_cache->m_same_type_str == "") {
			set<string>::iterator it = sim_temp_cache->m_same_type_set.begin();
			for (; it != sim_temp_cache->m_same_type_set.end(); ++it) {
				if (startswith(*it, "11") || startswith(*it, "12")) {
					return;
				}
			}
		}

		// ���ʹʣ���Ȼ�а�����ϵ����ַ���Ǹ߾�����ͬ����Ϊ�ۺ��з���
		if (resource->typeNameMap.find(base_data->m_format_name)
				!= resource->typeNameMap.end()
				|| resource->typeNameMap.find(inc_data->m_format_name)
						!= resource->typeNameMap.end()) {
			if (sim_temp_cache->m_addr_sim < 0.52) {
				sim_temp_cache->m_include_addr_tel_score -= resource->max_idf
						* 2;
			}
		}

		if (sim_temp_cache->m_addr_sim >= 0.52) {
			sim_temp_cache->m_include_addr_tel_score += resource->max_idf * 2;
		}

		if (sim_temp_cache->m_same_tel_set.size() > 0) {
			sim_temp_cache->m_include_addr_tel_score += resource->max_idf;
		}
	}

	if (sim_temp_cache->m_include_addr_tel_score > 0) {
		sim_temp_cache->m_total_same_score +=
				sim_temp_cache->m_include_addr_tel_score;
	} else {
		sim_temp_cache->m_total_diff_score +=
				-sim_temp_cache->m_include_addr_tel_score;
	}
}

void computeAreaSubPoiScore(const PreProcData *base_data,
		const PreProcData *inc_data, const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
//	if (base_data->m_main_type != inc_data->m_main_type) {
//		return;
//	}

	int base_count = 0, inc_count = 0;

	if (sim_temp_cache->m_is_mode_self && !sim_temp_cache->m_name_is_include) {
		if (sim_temp_cache->m_addr_sim >= 0.52
				&& startswith(sim_temp_cache->m_same_type_str, "1310")) {
			// ��ַ��ͬ���̳�
		} else {
			bool base_has_nofreq = hasNotFreqTypeWord(
					sim_temp_cache->m_diff_words_base_vec, resource, 1.8,
					base_count, false);
			bool inc_has_nofreq = hasNotFreqTypeWord(
					sim_temp_cache->m_diff_words_inc_vec, resource, 1.8,
					inc_count, false);
			if (base_has_nofreq || inc_has_nofreq) {
				sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 8;
			}
		}
	}

	if (sim_temp_cache->m_same_type_set.size() > 1) {
		bool base_has_nofreq = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_base_vec, resource, 3.0,
				base_count, false);
		bool inc_has_nofreq = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_inc_vec, resource, 3.0, inc_count,
				false);

		if ((base_has_nofreq || inc_has_nofreq)
				&& !isSameAddr(base_data, inc_data, sim_temp_cache)
				&& sim_temp_cache->m_same_tel_set.size() == 0) {
			sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 4;
		}
	} else if (startswith(sim_temp_cache->m_same_type_str, "12")) {
		// ������������
		// ��������ˮĥ��ί���ۺ�����칫��	��������ˮĥ�����ίԱ�� ��Ƶ���ʳ�����diff��Ҫ����

		if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec, resource,
				3.5, base_count, true)
				|| hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
						resource, 3.5, inc_count, true)) {
			if (base_data->m_addr_level > 2 && inc_data->m_addr_level > 2
					&& sim_temp_cache->m_same_tel_set.size() == 0) {
				sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 6;
			} else if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3
					&& sim_temp_cache->m_addr_sim < 0.52) {
				sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 6;
			} else {
				sim_temp_cache->m_area_sub_poi_score -= resource->max_idf * 1.5
						* (sim_temp_cache->m_diff_words_base_vec.size()
								+ sim_temp_cache->m_diff_words_inc_vec.size());
			}
		}
	} else if (startswith(sim_temp_cache->m_same_type_str, "19")) {
		if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec, resource,
				3.1, base_count, false)
				&& hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
						resource, 3.1, inc_count, false)) {
			//cout << "-=-==" << base_count << "\t" << inc_count << endl;
			if (base_count > 0 && inc_count > 0) {
				sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 4;
				if (sim_temp_cache->m_tel_score > 0) {
					sim_temp_cache->m_total_same_score -=
							sim_temp_cache->m_tel_score;
				}

				if (sim_temp_cache->m_addr_score > 0) {
					sim_temp_cache->m_total_same_score -=
							sim_temp_cache->m_addr_score;
				}
			}
		}
	} else if (startswith(sim_temp_cache->m_same_type_str, "10")) {
		if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec, resource,
				3.5, base_count, false)
				&& hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
						resource, 3.5, inc_count, false)) {
			if (base_count > 0 || inc_count > 0) {
				sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 5;
				if (sim_temp_cache->m_tel_score > 0) {
					sim_temp_cache->m_total_same_score -=
							sim_temp_cache->m_tel_score;
				}

				if (sim_temp_cache->m_addr_score > 0) {
					sim_temp_cache->m_total_same_score -=
							sim_temp_cache->m_addr_score;
				}
			}
		}
	} else if (startswith(sim_temp_cache->m_same_type_str, "1313")) {
		if (hasNotFreqTypeWord(sim_temp_cache->m_diff_words_base_vec, resource,
				2.8, base_count, false)
				&& hasNotFreqTypeWord(sim_temp_cache->m_diff_words_inc_vec,
						resource, 2.8, inc_count, false)) {
			if (base_count > 0 || inc_count > 0) {
				sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 4;
			}
		}
	} else if (startswith(sim_temp_cache->m_same_type_str, "22")
			|| startswith(sim_temp_cache->m_same_type_str, "23")
			|| startswith(sim_temp_cache->m_same_type_str, "1425")) {
		bool base_has_nofreq = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_base_vec, resource, 2.8,
				base_count, false);
		bool inc_has_nofreq = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_inc_vec, resource, 2.8, inc_count,
				false);
		if (base_has_nofreq || inc_has_nofreq) {
			if (base_count > 0 || inc_count > 0) {
				sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 5;
				if (sim_temp_cache->m_tel_score > 0) {
					sim_temp_cache->m_total_same_score -=
							sim_temp_cache->m_tel_score;
				}

				if (sim_temp_cache->m_addr_score > 0) {
					sim_temp_cache->m_total_same_score -=
							sim_temp_cache->m_addr_score;
				}
			}
		}
	} else if (startswith(sim_temp_cache->m_same_type_str, "2810")) {
		bool base_has_nofreq = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_base_vec, resource, 1.2,
				base_count, false);
		bool inc_has_nofreq = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_inc_vec, resource, 1.2, inc_count,
				false);
		if (base_has_nofreq || inc_has_nofreq) {
			sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 4;
		}
	} else if (startswith(sim_temp_cache->m_same_type_str, "20")
			&& endswith(base_data->m_bigram_name, "����վ")
			&& endswith(inc_data->m_bigram_name, "����վ")) {
		// ���������������������������վ	����������������վ
		if (endswith(base_data->m_bigram_name, inc_data->m_bigram_name)
				|| endswith(inc_data->m_bigram_name,
						base_data->m_bigram_name)) {
			// �а�����ϵ
		} else if (isSameAddr(base_data, inc_data, sim_temp_cache)
				|| sim_temp_cache->m_same_tel_set.size() > 0) {
			// �绰����ַ��ͬ
		} else {
			sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 4;
		}
	} else {
		// ����������ʱ����ͬ���ֳ��ַ�Ƶ����ʱ����Ҫ����
		// ��ͨС������������   ��ͨС��
		bool base_has_nofreq_high = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_base_vec, resource, 3.2,
				base_count, false);
		bool inc_has_nofreq_high = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_inc_vec, resource, 3.2, inc_count,
				false);

		bool base_has_nofreq_low = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_base_vec, resource, 3.2,
				base_count, false);
		bool inc_has_nofreq_low = hasNotFreqTypeWord(
				sim_temp_cache->m_diff_words_inc_vec, resource, 3.2, inc_count,
				false);

		if (base_has_nofreq_high || inc_has_nofreq_high) {
			if ((base_data->m_addr_level > 2 || inc_data->m_addr_level > 2)) {
				// ��ַ����
				sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 4;
			} else if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3
					&& sim_temp_cache->m_addr_sim < 0.52) {
				// ��ַ���Բ�һ���������Զ�����ǹսǣ�
				sim_temp_cache->m_area_sub_poi_score += -resource->max_idf * 4;
			}
		} else if (base_has_nofreq_low || inc_has_nofreq_low) {
			if ((base_data->m_addr_level > 2 || inc_data->m_addr_level > 2)) {
				// ��ַ����
				if (sim_temp_cache->m_same_tel_set.size() == 0) {
					// �绰û����ͬ��
					sim_temp_cache->m_area_sub_poi_score += -resource->max_idf
							* 4;
				} else {
					sim_temp_cache->m_area_sub_poi_score += -resource->max_idf
							* 3;
				}
			} else if (base_data->m_addr_level < 3 && inc_data->m_addr_level < 3
					&& sim_temp_cache->m_addr_sim < 0.52
					&& sim_temp_cache->m_distance > 50) {
				// ��ַ���Բ�һ���������Զ�����ǹսǣ�
				if (sim_temp_cache->m_same_tel_set.size() == 0) {
					// �绰û����ͬ��
					sim_temp_cache->m_area_sub_poi_score += -resource->max_idf
							* 4;
				} else {
					sim_temp_cache->m_area_sub_poi_score += -resource->max_idf
							* 3;
				}
			}
		}
	}

	if (sim_temp_cache->m_area_sub_poi_score > 0) {
		sim_temp_cache->m_total_same_score +=
				sim_temp_cache->m_area_sub_poi_score;
	} else {
		sim_temp_cache->m_total_diff_score +=
				-sim_temp_cache->m_area_sub_poi_score;
	}
}

//==================================================
// ���ƶ��ּܷ���
//==================================================
void computeScore(const PreProcData *base_data, const PreProcData *inc_data,
		const void *cfg, const Resource *resource,
		SimTempCache *sim_temp_cache) {
	sim_temp_cache->m_score = (sim_temp_cache->m_total_same_score
			- sim_temp_cache->m_total_diff_score)
			/ (sim_temp_cache->m_total_same_score
					+ sim_temp_cache->m_total_diff_score);
}

//==================================================
// ������
//==================================================
//void outputResult(const SimTempCache *sim_temp_cache, bool is_base,
//		bool is_debug, map<unsigned long long, bool> &base_sgl_map,
//		map<unsigned long long, bool> &inc_sgl_map,
//		pthread_mutex_t &prepare_output_mutex) {
//	pthread_mutex_lock(&prepare_output_mutex);
//	if (is_base) {
//		cout << "[rel]:" << sim_temp_cache->m_base_id << "\t"
//				<< sim_temp_cache->m_inc_id << "\t" << sim_temp_cache->m_score
//				<< "\t" << "base" << "\n";
//	} else {
//		cout << "[rel]:" << sim_temp_cache->m_base_id << "\t"
//				<< sim_temp_cache->m_inc_id << "\t" << sim_temp_cache->m_score
//				<< "\t" << "increase" << "\n";
//	}
//
////	if (sim_temp_cache->m_score > 0) {
////		base_sgl_map[sim_temp_cache->m_base_id] = false;
////		inc_sgl_map[sim_temp_cache->m_inc_id] = false;
////	}
//
//	//if (is_debug) {
//	//print(sim_temp_cache);
//	//}
//	pthread_mutex_unlock(&prepare_output_mutex);
//}

void outputResult(const SimTempCache *sim_temp_cache, bool is_base,
		pthread_mutex_t &prepare_output_mutex) {
	pthread_mutex_lock(&prepare_output_mutex);
	if (is_base) {
		cout << "[rel]:" << sim_temp_cache->m_base_id << "\t"
				<< sim_temp_cache->m_inc_id << "\t" << sim_temp_cache->m_score
				<< "\t" << sim_temp_cache->m_len_diff << "\t"
				<< sim_temp_cache->m_addr_sim << "\t"
				<< sim_temp_cache->m_distance << "\t"
				<< sim_temp_cache->m_same_tel_set.size() << "\t" << "base"
				<< "\t" << sim_temp_cache->m_status << "\n";
	} else {
		cout << "[rel]:" << sim_temp_cache->m_base_id << "\t"
				<< sim_temp_cache->m_inc_id << "\t" << sim_temp_cache->m_score
				<< "\t" << sim_temp_cache->m_len_diff << "\t"
				<< sim_temp_cache->m_addr_sim << "\t"
				<< sim_temp_cache->m_distance << "\t"
				<< sim_temp_cache->m_same_tel_set.size() << "\t" << "increase"
				<< "\t" << sim_temp_cache->m_status << "\n";
	}

	pthread_mutex_unlock(&prepare_output_mutex);
}

//==================================================
// ���ƶȼ�����Ҫ����Ľ���Ĺ���
//==================================================
void initResultCache(const PreProcMap *data_map,
		map<unsigned long long, bool> &sgl_map) {
	PreProcMap::const_iterator it = data_map->begin();
	for (; it != data_map->end(); ++it) {
		sgl_map.insert(make_pair(it->first, true));
	}
}

void outputSgl(const map<unsigned long long, bool> &sgl_map, bool is_base) {
	map<unsigned long long, bool>::const_iterator it = sgl_map.begin();
	if (is_base) {
		for (; it != sgl_map.end(); ++it) {
			if (it->second) {
				cout << "[sgl]:" << it->first << "\n";
			}
		}
	} else {
		for (; it != sgl_map.end(); ++it) {
			if (it->second) {
				cout << "[sgl]:" << it->first << "\n";
			}
		}
	}
}

//==================================================
// ɾ��inc�д��ڵ�baseid
//==================================================
void delSameId(PreProcMap *base_map, PreProcMap *inc_map) {
	PreProcMap::iterator it = base_map->begin();
	for (; it != base_map->end(); ++it) {
		if (inc_map->find(it->first) != inc_map->end()) {
			inc_map->erase(it->first);
		}
	}
}

//==================================================
// �Ƚ����ƶȴ�С
//==================================================
bool cmpRecord(SimTempCache &r1, SimTempCache &r2) {
	double temp_score = r2.m_score - r1.m_score;

//	cout << "1===" << temp_score << endl;

	if (temp_score >= 0.005) {
		return true;
	} else if (temp_score <= -0.005) {
		return false;
	}

//���ƶ�����
//	cout << "2===" << r2.m_status << "\t" << r1.m_status << endl;

	if (r2.m_len_diff < r1.m_len_diff && r2.m_distance < 1000) {
		return true;
	} else if (r1.m_len_diff < r2.m_len_diff && r1.m_distance < 1000) {
		return false;
	}

//	cout << "3===" << r2.m_len_diff << "\t" << r1.m_len_diff << endl;
//	cout << r2.m_same_tel_set.size() << "\t" << r1.m_same_tel_set.size() << endl;

	if (r2.m_same_tel_set.size() > 0 && r1.m_same_tel_set.size() == 0
			&& r2.m_distance < 1000) {
		return true;
	} else if (r1.m_same_tel_set.size() > 0 && r2.m_same_tel_set.size() == 0
			&& r1.m_distance < 1000) {
		return false;
	}

//	cout << "4===" << r2.m_len_diff << "\t" << r1.m_len_diff << endl;

	if (r2.m_addr_sim > r1.m_addr_sim && r2.m_addr_sim >= 0.52) {
		return true;
	} else if (r1.m_addr_sim > r2.m_addr_sim && r1.m_addr_sim >= 0.52) {
		return false;
	}

//	cout << "5===" << r2.m_len_diff << "\t" << r1.m_len_diff << endl;

	if (r2.m_len_diff < r1.m_len_diff) {
		return true;
	} else if (r1.m_len_diff < r2.m_len_diff) {
		return false;
	}

	if (r2.m_distance < r1.m_distance) {
		return true;
	} else if (r1.m_distance < r2.m_distance) {
		return false;
	}

	if (r2.m_addr_sim > r1.m_addr_sim) {
		return true;
	} else if (r1.m_addr_sim < r2.m_addr_sim) {
		return false;
	}

	if (r2.m_same_tel_set.size() > 0 && r1.m_same_tel_set.size() == 0) {
		return true;
	} else if (r1.m_same_tel_set.size() > 0 && r2.m_same_tel_set.size() == 0) {
		return false;
	}

	if (r2.m_is_base && r1.m_is_base) {
		if (r2.m_status == 0 && r1.m_status == 1) {
			return true;
		} else if (r2.m_status == 1 && r1.m_status == 0) {
			return false;
		}
	}

	if (r2.m_is_base && !r1.m_is_base) {
		return true;
	} else if (!r2.m_is_base && r1.m_is_base) {
		return false;
	}

	if (r2.m_base_id < r1.m_base_id) {
		return true;
	} else if (r1.m_base_id < r2.m_base_id) {
		return false;
	}

	if (r2.m_inc_id < r1.m_inc_id) {
		return true;
	} else if (r1.m_inc_id < r2.m_inc_id) {
		return false;
	}

	return false;
}

void compareSimScore(SimTempCache *cur_max_cache, SimTempCache *temp_cache) {
	if (cur_max_cache->m_is_null) {
		*cur_max_cache = *temp_cache;
		cur_max_cache->m_is_null = false;
	} else {
//		cout << "&&&&&&&" << endl;
//		print(cur_max_cache);
//		print(temp_cache);
//		cout << "++>" << cmpRecord(*cur_max_cache, *temp_cache) << endl;
		if (cmpRecord(*cur_max_cache, *temp_cache)) {
			*cur_max_cache = *temp_cache;
			cur_max_cache->m_is_null = false;
		}
	}
}

//==================================================
// ���ƶȼ��㼰���
//==================================================
// һ��һ
void poiSim(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource, const PoiSimCfg *poi_sim_cfg,
		const vector<RuleNode> *rule_node_vec, SimTempCache *sim_temp_cache,
		bool is_base) {
	sim_temp_cache->clear();
	sim_temp_cache->m_base_id = base_data->m_id;
	sim_temp_cache->m_inc_id = inc_data->m_id;
	sim_temp_cache->m_base_name = base_data->m_name;
	sim_temp_cache->m_inc_name = inc_data->m_name;
	sim_temp_cache->m_status = base_data->m_status;
	sim_temp_cache->m_is_base = is_base;

	//////////////////////////////////////////////////
	// ��������
	getDistance(base_data, inc_data, sim_temp_cache);
	getLenDiff(base_data, inc_data, sim_temp_cache);
	getAddrSim(base_data, inc_data, resource, sim_temp_cache);
	getSameTel(base_data, inc_data, resource, sim_temp_cache);
	getSameType(base_data, inc_data, resource, sim_temp_cache);
	getObj(base_data, inc_data, resource, sim_temp_cache);
	getBracketScore(base_data, inc_data, resource, sim_temp_cache);
	getIncludeName(base_data, inc_data, resource, sim_temp_cache);

	//////////////////////////////////////////////////
	// �����жϹ���
	vector<RuleNode>::const_iterator it = rule_node_vec->begin();

	for (; it != rule_node_vec->end(); ++it) {
		if ((it->fp)(base_data, inc_data, it->cfg, resource, sim_temp_cache)) {
			return;
		}
	}

	//////////////////////////////////////////////////
	// ��Ҫʹ��base��inc���ݲ������Ľ��洦��
	getDiffName(base_data, inc_data, sim_temp_cache);

	//////////////////////////////////////////////////
	// ���洦�����Ҫ�������ݴ���
	dealInBracket(base_data, inc_data, NULL, resource, sim_temp_cache);
	dealPinYin(base_data, inc_data, NULL, resource, sim_temp_cache);
	dealInclude(base_data, inc_data, NULL, resource, sim_temp_cache);
	//dealIncludeInSame(base_data, inc_data, NULL, resource, sim_temp_cache);
	dealOneSideRoad(base_data, inc_data, NULL, resource, sim_temp_cache);
	dealTypeWord(base_data, inc_data, NULL, resource, sim_temp_cache);
	dealSameTypeWord(base_data, inc_data, NULL, resource, sim_temp_cache);
	dealChainName(base_data, inc_data, NULL, resource, sim_temp_cache);
	dealGovDistrict(base_data, inc_data, NULL, resource, sim_temp_cache);
	dealFendian(base_data, inc_data, NULL, resource, sim_temp_cache);
	dealInName(base_data, inc_data, NULL, resource, sim_temp_cache);

	//////////////////////////////////////////////////
	// ���洦���Ĺ�����
	if (ruleDiffAccurateKeyWord(base_data, inc_data, NULL, resource,
			sim_temp_cache)) {
		return;
	}

	if (ruleDiffPark(base_data, inc_data, NULL, resource, sim_temp_cache)) {
		return;
	}

	if (ruleDiffWordUnlack(base_data, inc_data, NULL, resource,
			sim_temp_cache)) {
		return;
	}

	//////////////////////////////////////////////////
	// ���������ƶ�ֵ����
	// �����������ƶ�
	computeNameScore(base_data, inc_data, poi_sim_cfg, resource,
			sim_temp_cache);

	if (ruleNotFreqWord(base_data, inc_data, NULL, resource, sim_temp_cache)) {
		return;
	}

	if (ruleDiffName(base_data, inc_data, NULL, resource, sim_temp_cache)) {
		return;
	}

	computeTypeScore(base_data, inc_data, NULL, resource, sim_temp_cache);
	// �绰���������ͷ�֮��
	computeTelScore(base_data, inc_data, NULL, resource, sim_temp_cache);
	// ��ַ���������ͷ�֮��
	computeAddrScore(base_data, inc_data, NULL, resource, sim_temp_cache);

	// �ر��������ƶ�
	computeSameTailScore(base_data, inc_data, NULL, resource, sim_temp_cache);
	computeAccurateTypeScore(base_data, inc_data, NULL, resource,
			sim_temp_cache);
	computeSpecTypeScore(base_data, inc_data, NULL, resource, sim_temp_cache);
	computeDistanceScore(base_data, inc_data, NULL, resource, sim_temp_cache);
	computeInBracketScore(base_data, inc_data, NULL, resource, sim_temp_cache);
	computeSameNumScore(base_data, inc_data, NULL, resource, sim_temp_cache);
	computeSameAT(base_data, inc_data, NULL, resource, sim_temp_cache);
	computeAreaSubPoiScore(base_data, inc_data, NULL, resource, sim_temp_cache);

	//////////////////////////////////////////////////
	// ���ƶ��ּܷ���
	computeScore(base_data, inc_data, NULL, resource, sim_temp_cache);
}

// һ�Զ�
void poiSimFirst(const PreProcData *pre_proc_data, const PreProcMap *data_map,
		const set<unsigned long long> *id_set, const Resource *resource,
		const PoiSimCfg *poi_sim_cfg, const vector<RuleNode> *rule_node_vec,
		bool is_base, SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex) {
	set<unsigned long long>::const_iterator it = id_set->begin();
	PreProcMap::const_iterator data_it;
	for (; it != id_set->end(); ++it) {
		data_it = data_map->find(*it);
		if (data_it != data_map->end()) {
			poiSim(&data_it->second, pre_proc_data, resource, poi_sim_cfg,
					rule_node_vec, sim_temp_cache, is_base);
			if (sim_temp_cache->m_score > 0.0) {
				outputResult(sim_temp_cache, is_base, prepare_output_mutex);
				if (poi_sim_cfg->m_debug) {
					pthread_mutex_lock(&prepare_output_mutex);
					print(sim_temp_cache);
					pthread_mutex_unlock(&prepare_output_mutex);
				}
				return;
			}
		}
	}
}

void poiSimAll(const PreProcData *pre_proc_data, const PreProcMap *data_map,
		const set<unsigned long long> *id_set, const Resource *resource,
		const PoiSimCfg *poi_sim_cfg, const vector<RuleNode> *rule_node_vec,
		bool is_base, SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex) {

	set<unsigned long long>::const_iterator it = id_set->begin();
	PreProcMap::const_iterator data_it;
	for (; it != id_set->end(); ++it) {
		data_it = data_map->find(*it);
		if (data_it != data_map->end()) {
			poiSim(&data_it->second, pre_proc_data, resource, poi_sim_cfg,
					rule_node_vec, sim_temp_cache, is_base);

			if (poi_sim_cfg->m_debug) {
				pthread_mutex_lock(&prepare_output_mutex);
				print(sim_temp_cache);
				pthread_mutex_unlock(&prepare_output_mutex);
			}

			if (sim_temp_cache->m_score > poi_sim_cfg->m_poisim_all_output_th) {
				outputResult(sim_temp_cache, is_base, prepare_output_mutex);
			}
		}
	}
}

void poiSimMax(const PreProcData *pre_proc_data, const PreProcMap *data_map,
		const set<unsigned long long> *id_set, const Resource *resource,
		const PoiSimCfg *poi_sim_cfg, const vector<RuleNode> *rule_node_vec,
		bool is_base, SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex) {

	SimTempCache *temp_cache = new SimTempCache();
	set<unsigned long long>::const_iterator it = id_set->begin();
	PreProcMap::const_iterator data_it;
	for (; it != id_set->end(); ++it) {
		data_it = data_map->find(*it);
		if (data_it != data_map->end()) {
			poiSim(&(data_it->second), pre_proc_data, resource, poi_sim_cfg,
					rule_node_vec, temp_cache, is_base);
			compareSimScore(sim_temp_cache, temp_cache);
		}
	}

	if (poi_sim_cfg->m_debug) {
		pthread_mutex_lock(&prepare_output_mutex);
		print(sim_temp_cache);
		pthread_mutex_unlock(&prepare_output_mutex);
	}

	if (sim_temp_cache->m_score > poi_sim_cfg->m_poisim_all_output_th) {
		outputResult(sim_temp_cache, is_base, prepare_output_mutex);
	}

	delete temp_cache;
}

void poiSim(const PreProcData *pre_proc_data, const PreProcMap *data_map,
		const GridMap *grid_map, const Resource *resource,
		const PoiSimCfg *poi_sim_cfg, const vector<RuleNode> *rule_node_vec,
		PoiSimFunc poi_sim_func, bool is_base, SimTempCache *sim_temp_cache,
		map<unsigned long long, bool> &base_sgl_map,
		map<unsigned long long, bool> &inc_sgl_map,
		pthread_mutex_t &prepare_output_mutex) {

	int level = 0;
	set<unsigned long long> temp_id_set;
	if (pre_proc_data->m_type == "271020" || pre_proc_data->m_type == "271016"
			|| pre_proc_data->m_type == "261000"
			|| pre_proc_data->m_type == "261100") {
		level = 2;
	} else {
		level = 1;
	}

	sim_temp_cache->clear();
	getAroundGridMap(*pre_proc_data, *grid_map, temp_id_set, level);
	if (!is_base) {
		temp_id_set.erase(pre_proc_data->m_id);
	}

	poi_sim_func(pre_proc_data, data_map, &temp_id_set, resource, poi_sim_cfg,
			rule_node_vec, is_base, sim_temp_cache, base_sgl_map, inc_sgl_map,
			prepare_output_mutex);
}

// ��Զ�
void poiSim(PreProcMap::const_iterator &it, PreProcMap::const_iterator &end_it,
		const PreProcMap *base_map, const PreProcMap *inc_map,
		const GridMap *base_grid_map, const GridMap *inc_grid_map,
		const Resource *resource, const PoiSimCfg *poi_sim_cfg,
		map<string, PoiSimFunc> &poi_sim_func_map,
		SimResultCache *sim_result_cache, pthread_mutex_t &prepare_read_mutex,
		pthread_mutex_t &prepare_output_mutex) {

	SimTempCache *sim_temp_cache = new SimTempCache();

	// ���ƶȼ���
	PreProcMap::const_iterator temp_it;
	while (it != end_it) {
		pthread_mutex_lock(&prepare_read_mutex);
		temp_it = it;
		++it;
		pthread_mutex_unlock(&prepare_read_mutex);

		poiSim(&(temp_it->second), base_map, base_grid_map, resource,
				poi_sim_cfg, &(poi_sim_cfg->m_base_inc_rule_node_vec),
				poi_sim_cfg->m_base_inc_sim_func, true, sim_temp_cache,
				sim_result_cache->m_base_sgl_map,
				sim_result_cache->m_inc_sgl_map, prepare_output_mutex);

		if (poi_sim_cfg->m_find_in_self) {
			poiSim(&(temp_it->second), inc_map, inc_grid_map, resource,
					poi_sim_cfg, &(poi_sim_cfg->m_inc_inc_rule_node_vec),
					poi_sim_cfg->m_inc_inc_sim_func, false, sim_temp_cache,
					sim_result_cache->m_inc_sgl_map,
					sim_result_cache->m_inc_sgl_map, prepare_output_mutex);
		}
	}

	delete sim_temp_cache;
}

void *poiSimWraper(void *param) {
	PoiSimParam *poi_sim_param = (PoiSimParam *) param;
	poiSim(poi_sim_param->it, poi_sim_param->end_it, poi_sim_param->base_map,
			poi_sim_param->inc_map, poi_sim_param->base_grid_map,
			poi_sim_param->inc_grid_map, poi_sim_param->resource,
			poi_sim_param->poi_sim_cfg, *(poi_sim_param->poi_sim_func_map),
			poi_sim_param->sim_result_cache,
			*(poi_sim_param->prepare_read_mutex),
			*(poi_sim_param->prepare_output_mutex));
}

void poiSim(PreProcMap *base_map, PreProcMap *inc_map, Resource *resource,
		PoiSimCfg *poi_sim_cfg) {
	// ������Ч�Լ��
	if (!isValidCfg(poi_sim_cfg)) {
		cout << "[Err]: poi sim cfg has some err" << endl;
		return;
	}

	// ɾ��inc�е�baseid
	if (poi_sim_cfg->m_del_same_id) {
		delSameId(base_map, inc_map);
	}

	// �������ݸ���idf����Դ����
	PreProcMap::iterator it = base_map->begin();
	if (!poi_sim_cfg->m_use_local_idf) {
		for (; it != base_map->end(); ++it) {
			statIdf(it->second, resource->idf);
			statTel(it->second, resource->uselessTelMap);
		}
	} else {
		for (; it != base_map->end(); ++it) {
			statTel(it->second, resource->uselessTelMap);
		}
	}

	it = inc_map->begin();
	if (!poi_sim_cfg->m_use_local_idf) {
		for (; it != inc_map->end(); ++it) {
			statIdf(it->second, resource->idf);
			statTel(it->second, resource->uselessTelMap);
		}
	} else {
		for (; it != inc_map->end(); ++it) {
			statTel(it->second, resource->uselessTelMap);
		}
	}

	if (!poi_sim_cfg->m_use_local_idf) {
		makeIdf(base_map->size() + inc_map->size(), resource->idf,
				resource->max_idf, resource->min_idf);
	}

	makeUselessTel(resource->uselessTelMap,
			poi_sim_cfg->m_useless_tel_threshold);

	// ��ʼ���Ź���
	GridMap *base_grid_map = new GridMap();
	GridMap *inc_grid_map = new GridMap();

	initGridMap(base_map, resource, base_grid_map);
	initGridMap(inc_map, resource, inc_grid_map);

	// ��ʼ����¼�Ƿ�û�оۺ��ϵ����ݽṹ
	SimResultCache *sim_result_cache = new SimResultCache();
	initResultCache(base_map, sim_result_cache->m_base_sgl_map);
	initResultCache(inc_map, sim_result_cache->m_inc_sgl_map);

	map<string, PoiSimFunc> poi_sim_func_map;
	poi_sim_func_map.insert(make_pair("all", poiSimAll));
	poi_sim_func_map.insert(make_pair("max", poiSimMax));
	poi_sim_func_map.insert(make_pair("first", poiSimFirst));

	pthread_mutex_t prepare_read_mutex;
	pthread_mutex_t prepare_output_mutex;
	pthread_mutex_init(&prepare_read_mutex, NULL);
	pthread_mutex_init(&prepare_output_mutex, NULL);

	// ������ʼ��
	PoiSimParam *poi_sim_param = new PoiSimParam();
	poi_sim_param->it = inc_map->begin();
	poi_sim_param->end_it = inc_map->end();
	poi_sim_param->base_map = base_map;
	poi_sim_param->inc_map = inc_map;
	poi_sim_param->base_grid_map = base_grid_map;
	poi_sim_param->inc_grid_map = inc_grid_map;
	poi_sim_param->resource = resource;
	poi_sim_param->poi_sim_cfg = poi_sim_cfg;
	poi_sim_param->poi_sim_func_map = &poi_sim_func_map;
	poi_sim_param->sim_result_cache = sim_result_cache;
	poi_sim_param->prepare_read_mutex = &prepare_read_mutex;
	poi_sim_param->prepare_output_mutex = &prepare_output_mutex;

	// creat thread
	vector<pthread_t> tid_vec;
	for (int i = 0; i < poi_sim_cfg->m_thread_num; ++i) {
		pthread_t tid;
		int error;
		error = pthread_create(&tid, NULL, poiSimWraper,
				(void *) poi_sim_param);
		if (error) {
			cout << "[Err]: phread is not created..." << endl;
			return;
		}
		tid_vec.push_back(tid);
	}

	for (int i = 0; i < poi_sim_cfg->m_thread_num; ++i) {
		pthread_join(tid_vec[i], NULL);
	}

// ���û�оۺ��ϵ�����id
//	if (poi_sim_cfg->m_out_base_sgl) {
//		outputSgl(sim_result_cache->m_base_sgl_map, true);
//	}
//
//	if (poi_sim_cfg->m_out_inc_sgl) {
//		outputSgl(sim_result_cache->m_inc_sgl_map, false);
//	}

	delete base_grid_map;
	delete inc_grid_map;
	delete poi_sim_param;
	delete sim_result_cache;
}
