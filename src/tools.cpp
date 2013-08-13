/*
 * tools.cpp
 *
 *  Created on: 2012-10-23
 *      Author: stefanyang
 */

#include <dirent.h>
#include <fstream>
#include <algorithm>
#include "tools.h"
#include "rules.h"
#include "common.h"

//========================================
// 函数形态的tools
//========================================
// 打印预处理结果
void print(PreProcData *pre_proc_data) {
	cout << "=======" << "\n";
	cout << ">id: " << pre_proc_data->m_id << "\n";
	cout << ">format name: " << pre_proc_data->m_format_name << "\n";
	cout << ">delpunc name: " << pre_proc_data->m_del_punc_name << "\n";
	cout << ">bigram name: " << pre_proc_data->m_bigram_name << "\n";

	cout << ">soso type: " << pre_proc_data->m_soso_type << "\n";
	cout << ">gov tail: " << pre_proc_data->m_gov_tail << "\n";
	cout << ">gov level: " << pre_proc_data->m_gov_level << "\n";
	cout << ">gov level keyword: " << pre_proc_data->m_gov_lvl_kw << "\n";

	cout << ">accurate keyword: " << pre_proc_data->m_accurate_keyword << "\n";
	cout << ">accurate type: " << pre_proc_data->m_accurate_type << "\n";

	cout << ">di zhi: " << pre_proc_data->m_dizhi << "\n";
	cout << ">fen dian: " << pre_proc_data->m_fendian << "\n";

	cout << ">obj name:";
	out_set(pre_proc_data->m_obj_name_set);

	cout << ">name vec==>";
	out_vector(pre_proc_data->m_name_vec);

	cout << ">bracket vec==>";
	out_vector(pre_proc_data->m_bracket_vec);

	cout << ">num name set==>";
	out_set(pre_proc_data->m_num_name_set);

	cout << ">door set==>";
	out_set(pre_proc_data->m_door_set);

	cout << ">dpmark set==>";
	out_set(pre_proc_data->m_dpmark_set);

	cout << ">smname group set==>";
	out_set(pre_proc_data->m_smname_group_set);

	cout << ">unlack word set==>";
	out_set(pre_proc_data->m_unlack_word_set);

	cout << ">grid name set==>";
	out_set(pre_proc_data->m_grid_name_set);

	cout << ">poi set==>";
	out_set(pre_proc_data->m_addrsim_poi_set);

	cout << ">xzqh map==>" << "\n";
	out_map(pre_proc_data->m_addrsim_xzqh_map);

	cout << ">road map==>" << "\n";
	out_map(pre_proc_data->m_addrsim_road_map);
}

void print(const SimTempCache *sim_temp_cache) {
	if (sim_temp_cache->m_base_id == 0 || sim_temp_cache->m_inc_id == 0) {
		return;
	}

	cout << "<== sim temp cache ==>" << "\n";
	cout << sim_temp_cache->m_base_id << "\t" << sim_temp_cache->m_inc_id
			<< "\n";
	cout << sim_temp_cache->m_base_name << "\t" << sim_temp_cache->m_inc_name
			<< "\n";

	cout << ">distance: " << sim_temp_cache->m_distance << "\n";
	cout << ">len diff: " << sim_temp_cache->m_len_diff << "\n";
	cout << ">addr sim: " << sim_temp_cache->m_addr_sim << "\n";
	cout << ">same type str: " << sim_temp_cache->m_same_type_str << "\n";

	cout << ">score: " << sim_temp_cache->m_score << "\n";
	cout << ">total same score: " << sim_temp_cache->m_total_same_score << "\n";
	cout << ">total diff score: " << sim_temp_cache->m_total_diff_score << "\n";

	cout << "> same name score: " << sim_temp_cache->m_name_same_score << "\n";
	cout << "> sub same name score: " << sim_temp_cache->m_name_sub_same_score
			<< "\n";
	cout << "> diff name score: " << sim_temp_cache->m_name_diff_score << "\n";
	cout << "> sub diff name score: " << sim_temp_cache->m_name_sub_diff_score
			<< "\n";
	cout << ">name score: " << sim_temp_cache->m_name_score << "\n";
	cout << ">addr score: " << sim_temp_cache->m_addr_score << "\n";
	cout << ">type score: " << sim_temp_cache->m_type_score << "\n";
	cout << ">tel score: " << sim_temp_cache->m_tel_score << "\n";
	cout << ">tail score: " << sim_temp_cache->m_tail_score << "\n";
	cout << ">accurate type score: " << sim_temp_cache->m_accurate_type_score
			<< "\n";
	cout << ">spec score: " << sim_temp_cache->m_spec_score << "\n";
	cout << ">distance score: " << sim_temp_cache->m_distance_score << "\n";
	cout << ">same num score: " << sim_temp_cache->m_same_num_score << "\n";
	cout << ">area sub poi score: " << sim_temp_cache->m_area_sub_poi_score
			<< "\n";
	cout << ">name include score: " << sim_temp_cache->m_include_addr_tel_score
			<< "\n";

	cout << ">obj name==>";
	out_set(sim_temp_cache->m_obj_name_set);

	cout << ">same types==>";
	out_set(sim_temp_cache->m_same_type_set);

	cout << ">same words==>";
	out_vector(sim_temp_cache->m_same_words_vec);

	cout << ">base sub same words==>";
	out_vector(sim_temp_cache->m_sub_same_words_base_vec);
	cout << ">inc sub same words==>";
	out_vector(sim_temp_cache->m_sub_same_words_inc_vec);

	cout << ">base diff words==>";
	out_vector(sim_temp_cache->m_diff_words_base_vec);
	cout << ">inc diff words==>";
	out_vector(sim_temp_cache->m_diff_words_inc_vec);

	cout << ">base sub diff words==>";
	out_vector(sim_temp_cache->m_sub_diff_words_base_vec);
	cout << ">inc sub diff words==>";
	out_vector(sim_temp_cache->m_sub_diff_words_inc_vec);

	cout << ">rule map==>" << "\n";
	out_map(sim_temp_cache->m_rule_result_map);
}

string sbc2dbc(const string &str) {
	string temp_str = str;

	string result = "";
	unsigned char tmp;
	unsigned char tmp1;
	unsigned int len = temp_str.length();
	for (unsigned int i = 0; i < len; ++i) {
		tmp = temp_str[i];
		tmp1 = temp_str[i + 1];
		if (tmp == 163) { ///第一个字节是163，标志着是全角字符
			result += (unsigned char) tmp1 - 128;
			i++;
		} else if (tmp == 161 && tmp1 == 161) {
			///处理全角空格
			result += " ";
			i++;
		} else if (tmp == 161 && (tmp1 == 163 || tmp1 == 164)) {
			result += ".";
			i++;
		} else if (tmp == 161 && (tmp1 == 174 || tmp1 == 175)) {
			result += "'";
			i++;
		} else if (tmp == 161 && (tmp1 == 176 || tmp1 == 177)) {
			result += "\"";
			i++;
		} else if (tmp == 161 && tmp1 == 162) {
			result += ",";
			i++;
		} else if (tmp == 161 && tmp1 == 170) {
			result += "-";
			i++;
		} else if (tmp == 161 && tmp1 == 171) {
			result += "~";
			i++;
		} else if (tmp == 161 && tmp1 == 231) {
			result += "$";
			i++;
		} else if (tmp > 128) { //汉字
			result.append(temp_str, i, 2);
			i++;
		} else {
			result += tmp;
		}
	}

	return result;
}

void lower(string &str) {
	for (size_t i = 0; i < str.size(); ++i) {
		str[i] = tolower(str[i]);
	}
}

void removePunc(string &str, bool needBra) {
	string::iterator it = str.begin();
	string::iterator curit = it;
	char *temp;
	while (it != str.end()) {
		temp = NULL;
		if (needBra)
			temp = (char *) memchr(" \t\r\n\\-_[]{}:.,=*&^%$#@!~?<>/|'\"`", *it,
					32);
		else
			temp = (char *) memchr(" \t\r\n\\-_[](){}:.,=*&^%$#@!~?<>/|'\"`",
					*it, 34);

		if (temp == NULL)
			*curit++ = *it++;
		else {
			++it;
		}
	}

	str.erase(curit, str.end());
}

void transZhNum(string &str, const _hashset &number, const _hashset &number2,
		int length) {
	vector<string> temp_vec;
	split2word2(str, temp_vec);
	string temp(""), num("");
	string result("");
	bool flag = false;
	vector<string>::iterator it = temp_vec.begin();
	for (; it != temp_vec.end(); ++it) {
		if (!flag) {
			if (number2.find(*it) != number2.end()) {
				temp += *it;
				flag = true;
			} else {
				result += *it;
			}
		} else {
			if (number.find(*it) != number.end()) {
				temp += *it;
			} else {
				if (temp != "" && len(temp) > length) {
					num = Decapitalized(temp);
					flag = false;
					if (num != "")
						result += num;
					else
						result += temp;
					temp = "";
				} else {
					result += temp;
					temp = "";
				}
				result += *it;
			}
		}
	}

	if (temp != "" && len(temp) > length) {
		num = Decapitalized(temp);
		flag = false;
		if (num != "")
			result += num;
		else
			result += temp;
	} else {
		result += temp;
	}

	if (result != "") {
		str = result;
	}
}

void transZhNum(_set &src, const _hashset &number, const _hashset &number2,
		int length) {
	_setit src_it = src.begin();
	_set result;
	std::string srcStr("");
	for (; src_it != src.end(); ++src_it) {
		srcStr = *src_it;
		transZhNum(srcStr, number, number2, length);
		if (srcStr != "") {
			result.insert(srcStr);
		}
	}
	src = result;
}

// 删除括号及其内部内容
void removeBracket(string &str, vector<string> &result) {
	result.clear();
	char prefix = '(';
	char suffix = ')';

	size_t count = 0;
	string temp_str("");
	unsigned int pos = 0;
	unsigned int i = 0;

	for (; i < str.length(); ++i) {
		if (str[i] == prefix) {
			++count;
			if (i - pos > 1) {
				temp_str += str.substr(pos, i - pos);
			}
			pos = i + 1;
		} else if (str[i] == suffix) {
			if (count > 0) {
				--count;
				// 找到一对括号
				if (count >= 0) {
					if (i - pos > 1) {
						result.push_back(str.substr(pos, i - pos));
					}
					pos = i + 1;
				}
			} else {
				pos = i + 1;
			}
		}
	}

	if (i > pos) {
		if (count > 0) {
			result.push_back(str.substr(pos, i - pos));
		} else {
			temp_str += str.substr(pos, i - pos);
		}
	}

	str = temp_str;
}

// 长词后缀
//int findAreaTail(string &formatName, vector<string> &bracket_vec,
//		const _hashset &areaTailSet) {
//	string temp("");
//	size_t strLen = formatName.length();
//	_hashset::const_iterator it;
//
//	for (int i = 0; i < strLen; i++) {
//		temp = formatName.substr(i, strLen - i);
//		it = areaTailSet.find(temp);
//		if (it != areaTailSet.end()) {
//			// 找到后缀
//			if (i > 2) {
//				bracket_vec.push_back(temp);
//				formatName = formatName.substr(0, i);
//				break;
//			}
//		}
//		if (formatName.at(i) < 0) {
//			i++;
//		}
//	}
//
//	return 0;
//}

int findAreaTail(PreProcData *pre_proc_data, const Resource *resource) {
	string temp("");
	size_t strLen = pre_proc_data->m_format_name.length();
	_hashset::const_iterator it;

	for (int i = 0; i < strLen; i++) {
		temp = pre_proc_data->m_format_name.substr(i, strLen - i);
		it = resource->areaTailSet.find(temp);

		if (it != resource->areaTailSet.end()) {
			// 找到后缀
			if (i > 2) {
				if (endswith(temp, "分店")) {
					temp = temp.substr(0, temp.size() - 4);
				} else if (endswith(temp, "店")) {
					temp = temp.substr(0, temp.size() - 2);
				}
				if (temp != "") {
					pre_proc_data->m_bracket_vec.push_back(temp);
					pre_proc_data->m_fendian = temp;
				}

				pre_proc_data->m_format_name =
						pre_proc_data->m_format_name.substr(0, i);

				break;
			}
		}
		if (pre_proc_data->m_format_name.at(i) < 0) {
			i++;
		}
	}

	return 0;
}

// 获取地名中的行政后缀
void getGovTail(std::string &src, string &gov_tail, const _mapSet &typeword) {
	gov_tail = "";
	std::string str_gov_tail = "县乡镇村";
	std::string str_end("");
	if (src.length() > 4) {
		str_end = src.substr(src.length() - 2, 2);
		_mapSet::const_iterator it = typeword.find("01");
		if (it != typeword.end()) {
			if (it->second.find(src) == it->second.end()
					&& str_gov_tail.find(str_end) != std::string::npos) {
				src = src.substr(0, src.length() - 2);
				gov_tail = str_end;
			}
		}
	}
}

// 获取连锁店名
void findObjName(PreProcData *pre_proc_data, const Resource *resource) {
	std::string tempObjName("");
	_mapStrDbl::const_iterator distance_it;
	_map::const_iterator type_it;
	bool null_flag = true;

	_vectorit it = pre_proc_data->m_name_vec.begin();
	for (; it != pre_proc_data->m_name_vec.end(); ++it) {
		distance_it = resource->objNameMap.find(*it);
		if (distance_it != resource->objNameMap.end()) {
			// 名称在连锁店列表中
			type_it = resource->objTypeMap.find(distance_it->first);
			if (type_it == resource->objTypeMap.end()
					|| (type_it != resource->objTypeMap.end()
							&& (startswith(pre_proc_data->m_type,
									type_it->second)
									|| startswith(pre_proc_data->m_soso_type,
											type_it->second)))) {
				// 类型匹配
				pre_proc_data->m_obj_name_set.insert(distance_it->first);
				if (null_flag
						|| pre_proc_data->m_obj_max_distance
								> distance_it->second) {
					pre_proc_data->m_obj_max_distance = distance_it->second;
					null_flag = false;
				}
			}
		}
	}
}

// 对名称只是类型词的括号内内容处理
void addContentForTWName(PreProcData *pre_proc_data, const _mapSet &typeword) {
	_mapSet::const_iterator it = typeword.find("01");
	if (it != typeword.end()) {
		if (it->second.find(pre_proc_data->m_format_name) != it->second.end()
				|| (endswith(pre_proc_data->m_format_name, "店")
						&& it->second.find(
								pre_proc_data->m_format_name.substr(0,
										pre_proc_data->m_format_name.length()
												- 2)) != it->second.end())) {
			pre_proc_data->m_name_vec.insert(pre_proc_data->m_name_vec.end(),
					pre_proc_data->m_bracket_vec.begin(),
					pre_proc_data->m_bracket_vec.end());
			pre_proc_data->m_bracket_vec.clear();
		}
	}
}

// 合并数字,字母内容
void mergeNumAlp(PreProcData *pre_proc_data, const Resource *resource) {
	vector<string> tempResultVec;
	vector<string> tempVec;

	if (pre_proc_data->m_main_type == "28") {
		mergeNumAlp("", pre_proc_data->m_bracket_vec, tempVec, tempResultVec,
				pre_proc_data->m_num_name_set, resource->measure, true);
		pre_proc_data->m_bracket_vec = tempVec;

		if (pre_proc_data->m_bracket_vec.size() > 0
				&& isDigit(
						pre_proc_data->m_bracket_vec[pre_proc_data->m_bracket_vec.size()
								- 1])) {
			pre_proc_data->m_num_name_set.insert(
					pre_proc_data->m_bracket_vec[pre_proc_data->m_bracket_vec.size()
							- 1]);
			pre_proc_data->m_bracket_vec.erase(
					pre_proc_data->m_bracket_vec.end());
		}

		if (pre_proc_data->m_name_vec.size() > 0
				&& isDigit(
						pre_proc_data->m_name_vec[pre_proc_data->m_name_vec.size()
								- 1])) {
			pre_proc_data->m_num_name_set.insert(
					pre_proc_data->m_name_vec[pre_proc_data->m_name_vec.size()
							- 1]);
			pre_proc_data->m_name_vec.erase(pre_proc_data->m_name_vec.end());
		}
	}

	mergeNumAlp(pre_proc_data->m_format_name, pre_proc_data->m_name_vec,
			tempVec, tempResultVec, pre_proc_data->m_num_name_set,
			resource->measure, false);

	int notTypeWordCount = 0;
	countNotType(tempVec, notTypeWordCount, resource->typeword);
	if (notTypeWordCount != 0) {
		pre_proc_data->m_name_vec = tempVec;
	} else {
		pre_proc_data->m_name_vec = tempResultVec;
	}

	if (startswith(pre_proc_data->m_type, "24")
			|| startswith(pre_proc_data->m_soso_type, "24")) {
		pre_proc_data->m_name_vec = tempResultVec;
	}
}

void mergeTailSglWord(PreProcData *pre_proc_data) {
	int nameWordCount = pre_proc_data->m_name_vec.size();
	if (nameWordCount > 1
			&& (startswith(pre_proc_data->m_type, "24")
					|| startswith(pre_proc_data->m_type, "12"))) {
		std::string str_tail = "委办";
		if (str_tail.find(pre_proc_data->m_name_vec[nameWordCount - 1])
				!= std::string::npos) {
			pre_proc_data->m_name_vec[nameWordCount - 2] +=
					pre_proc_data->m_name_vec[nameWordCount - 1];
			pre_proc_data->m_name_vec.erase(pre_proc_data->m_name_vec.end());
		}
	}
}

void filteDistrict(PreProcData *pre_proc_data, const Resource *resource,
		int &count) {
	if (!startswith(pre_proc_data->m_type, "26")
			&& !startswith(pre_proc_data->m_type, "27")) {
		if (pre_proc_data->m_name_vec.size() > 1
				&& pre_proc_data->m_province != "") {
			filte(pre_proc_data->m_name_vec, pre_proc_data->m_province,
					resource->typeword, count);
		}
		if (pre_proc_data->m_name_vec.size() > 1
				&& pre_proc_data->m_city != "") {
			filte(pre_proc_data->m_name_vec, pre_proc_data->m_city,
					resource->typeword, count);
		}

		if (pre_proc_data->m_name_vec.size() > 1
				&& pre_proc_data->m_district != "") {
			filte(pre_proc_data->m_name_vec, pre_proc_data->m_district,
					resource->typeword, count);
		}

		if (pre_proc_data->m_name_vec.size() > 1) {
			if (!startswith(pre_proc_data->m_type, "28")) {
				filteDistrict(pre_proc_data->m_name_vec, pre_proc_data->m_addr,
						resource->xzqh, resource->typeword, count);
			}
		}
	}

	filte(pre_proc_data->m_bracket_vec, pre_proc_data->m_province);
	filte(pre_proc_data->m_bracket_vec, pre_proc_data->m_city);
	filte(pre_proc_data->m_bracket_vec, pre_proc_data->m_district);
	filteDistrict(pre_proc_data->m_bracket_vec, pre_proc_data->m_addr,
			resource->xzqh);
}

void formatGov(string &name, string &gov_tail, const _mapSet &typeword) {
	_mapSet::const_iterator it = typeword.find("01");
	if (it != typeword.end()) {
		std::string str_gov_tail = "县乡镇村";
		std::string str_end("");
		if (name.length() > 4) {
			str_end = name.substr(name.length() - 2, 2);
			if (it->second.find(name) == it->second.end()
					&& str_gov_tail.find(str_end) != std::string::npos) {
				name = name.substr(0, name.length() - 2);
				gov_tail = str_end;
			}
		}
	}
}

void formatGov(_vector &src, string &gov_tail, const _mapSet &typeword) {
	_vectorit it = src.begin();
	for (; it != src.end(); ++it) {
		formatGov(*it, gov_tail, typeword);
	}
}

bool findType(const string &base_main_type, const string &base_sub_type,
		const string &base_type, const string &inc_main_type,
		const string &inc_sub_type, const string &inc_type,
		const _mapSet &type_map, SimTempCache *sim_temp_cache) {

	if (base_type == "" || inc_type == "") {
		return false;
	}

	//=================================
	//	不同类型的映射
	//=================================
	_mapSet::const_iterator it = type_map.find(base_type);
	if (it != type_map.end()) {
		_hashset::const_iterator it_set = it->second.find(inc_type);
		if (it_set != it->second.end()) {
			sim_temp_cache->m_same_type_set.insert(base_type);
			sim_temp_cache->m_same_type_set.insert(inc_type);
			return true;
		}
	}

	//=================================
	//	大中小类的检测
	//=================================
	//商场类的特别处理
	if ((base_type == "131000") && (inc_type == "131000")) {
		sim_temp_cache->m_same_type_set.insert(base_type);
		return true;
	} else if ((base_type == "131000") ^ (inc_type == "131000")) {
		return false;
	}

	if (base_type == inc_type) {
		sim_temp_cache->m_same_type_set.insert(base_type);
	} else if (base_sub_type == inc_sub_type) {
		sim_temp_cache->m_same_type_set.insert(base_sub_type);
	} else if (base_main_type == inc_main_type) {
		sim_temp_cache->m_same_type_set.insert(base_main_type);
	}

	if (sim_temp_cache->m_same_type_set.size() == 1) {
		string temp_type = *(sim_temp_cache->m_same_type_set.begin());
		if (temp_type.size() == 2 && temp_type == "24") {
			// 检查学校中类
			if (!endswith(base_sub_type, "99")
					&& !endswith(inc_sub_type, "99")) {
				sim_temp_cache->m_same_type_set.clear();
				return false;
			}
		}
	}

	if (sim_temp_cache->m_same_type_set.size() > 0) {
		return true;
	}

	return false;
}

bool findType(const PreProcData *base_data, const PreProcData *inc_data,
		const _mapSet &type_map, SimTempCache *sim_temp_cache) {

	if (findType(base_data->m_main_type, base_data->m_sub_type,
			base_data->m_type, inc_data->m_main_type, inc_data->m_sub_type,
			inc_data->m_type, type_map, sim_temp_cache)) {
		return true;
	} else if (findType(base_data->m_main_type, base_data->m_sub_type,
			base_data->m_type, inc_data->m_soso_main_type,
			inc_data->m_soso_sub_type, inc_data->m_soso_type, type_map,
			sim_temp_cache)) {
		return true;
	} else if (findType(base_data->m_soso_main_type, base_data->m_soso_sub_type,
			base_data->m_soso_type, inc_data->m_main_type, inc_data->m_sub_type,
			inc_data->m_type, type_map, sim_temp_cache)) {
		return true;
	}

	return false;
}

bool isSameType(const string &base_type, const string &inc_type,
		const _mapSet &type_map) {
	if (base_type == "" || inc_type == "") {
		return false;
	} else if (base_type.size() != 6 || inc_type.size() != 6) {
		return false;
	}

	if (base_type == inc_type) {
		return true;
	}

	//=================================
	//	不同类型的映射
	//=================================
	_mapSet::const_iterator it = type_map.find(base_type);
	if (it != type_map.end()) {
		_hashset::const_iterator it_set = it->second.find(inc_type);
		if (it_set != it->second.end()) {
			return true;
		}
	}

	//=================================
	//	大中小类的检测
	//=================================
	if (base_type == inc_type) {
		return true;
	} else if (base_type.substr(0, 4) == inc_type.substr(0, 4)) {
		return true;
	} else if (base_type.substr(0, 2) == inc_type.substr(0, 2)) {
		return true;
	}

	return false;
}

// 一个词是否包含于一个vector中
bool isInclude(const string &str, const vector<string> &str_vec) {
	vector<string>::const_iterator it = str_vec.begin();
	for (; it != str_vec.end(); ++it) {
		if ((*it).find(str) != std::string::npos) {
			return true;
		}
	}

	return false;
}

// 不可缺失词的识别
void findUnlackWord(PreProcData *pre_proc_data, const _mapSet &unlack_map) {
	_mapSet::const_iterator map_it = unlack_map.find(pre_proc_data->m_type);
	if (map_it != unlack_map.end()) {
		vector<string>::const_iterator it = pre_proc_data->m_name_vec.begin();
		for (; it != pre_proc_data->m_name_vec.end(); ++it) {
			if (map_it->second.find(*it) != map_it->second.end()) {
				pre_proc_data->m_unlack_word_set.insert(*it);
			}
		}
	}
}

// 精准子类关键词的识别
//void findDiffAccurateKeyword(const vector<string> &data_vec,
//		const string &base_str, const string &inc_str, const Resource *resource,
//		vector<string> &result_vec) {
//	vector<string>::const_iterator data_it = data_vec.begin();
//	_mapStrDbl::const_iterator idf_it;
//	for (; data_it != data_vec.end(); ++data_it) {
//		if (base_str.find(*data_it) != std::string::npos
//				&& inc_str.find(*data_it) == std::string::npos) {
//			idf_it = resource->idf.find(*data_it);
//			if (idf_it != resource->idf.end() && idf_it->second > 3.0
//					&& !endswith(*data_it, "分校") && !endswith(*data_it, "分部")
//					&& !endswith(*data_it, "校区")) {
//				result_vec.push_back(*data_it);
//			}
//		}
//	}
//}

void findDiffAccurateKeyword(const vector<string> &data_vec,
		const set<string> &word_set, const Resource *resource,
		vector<string> &result_vec) {
	vector<string>::const_iterator data_it = data_vec.begin();
	_mapStrDbl::const_iterator idf_it;
	for (; data_it != data_vec.end(); ++data_it) {
		if (word_set.find(*data_it) != word_set.end()) {
			idf_it = resource->idf.find(*data_it);
			if (idf_it != resource->idf.end() && idf_it->second > 3.0) {
				result_vec.push_back(*data_it);
			}
		}
	}
}

// 过滤00词表中出现的词
void filte(vector<string> &data_vec, const Resource *resource) {
	vector<string> temp_vec = data_vec;
	_mapSet::const_iterator it = resource->typeword.find("00");
	if (it != resource->typeword.end()) {
		filte(data_vec, it->second);
		if (data_vec.size() == 0) {
			data_vec = temp_vec;
		}
	}
}

// 将名称分词后，带店结尾的词添加到grid表中
void setGridKeyWord(const vector<string> &name_vec, set<string> &grid_set) {
	vector<string>::const_iterator it = name_vec.begin();
	for (; it != name_vec.end(); ++it) {
		if (endswith(*it, "店") && (*it).size() > 2) {
			grid_set.insert((*it).substr(0, (*it).size() - 2));
		}
	}
}

// 规范门类信息
void formatDoorSet(set<string> &door_set) {
	set<string>::iterator it = door_set.begin();
	set<string> new_set;
	string temp = "";
	for (; it != door_set.end(); ++it) {
		temp = *it;
		new_set.insert(replaceStr(temp, "号门", "门"));
	}

	door_set = new_set;
}

// 检查不同部分是否出现有非频繁非01词表词
bool hasNotFreqTypeWord(vector<string> &data_vec, const Resource *resource,
		double theshold, int &count, bool checkGovTail) {
	_mapStrDbl::const_iterator idf_it;
	vector<string>::iterator it;
	set<string>::iterator find_it;
	_mapSet::const_iterator map_it = resource->typeword.find("01");
	string temp_tail = "";
	count = 0;

	if (map_it != resource->typeword.end() && data_vec.size() != 0) {
		for (it = data_vec.begin(); it != data_vec.end(); ++it) {
			if (resource->provinceSet.find(*it)
					!= resource->provinceSet.end()) {
				continue;
			}

			idf_it = resource->idf.find(*it);
			if (idf_it != resource->idf.end()) {
				// idf表中存在该词
				if ((*it).length() < 2) {
					continue;
				}

				// 当词语是个类型词时，是否需要考虑这个策略
//				if(map_it->second.find(*it) != map_it->second.end()){
//					if (idf_it->second >= 3.0){
//						count++;
//					}else{
//						continue;
//					}
//				}

				//cout << "---" << *it << "\t" << idf_it->second << "\t" << (idf_it->second >= theshold) << endl;

				temp_tail = (*it).substr((*it).length() - 2, 2);
				if (idf_it->second >= theshold
						&& (!checkGovTail
								|| (checkGovTail
										&& resource->govTailSet.find(temp_tail)
												== resource->govTailSet.end()))) {
					// 词的idf值较大
					count++;
					//return true;
				}
			} else {
				// idf表中不存在该词
				count++;
				//return true;
			}
		}
	}

	if (count > 0) {
		return true;
	}

	return false;
}

// 获取某路径下的所有文件名，并返回增加目录的路径表
int getPaths(const std::string &path, vector<string> &path_vec) {
	DIR * dir;
	struct dirent * ent;
	string dirpath = path;
	if (dirpath[dirpath.length() - 1] != '/') {
		dirpath += "/";
	}
	cout << "the directory path is :" << dirpath << endl;
	dir = opendir(dirpath.c_str());
	if (!dir) {
		return -1;
	}

	while (ent = readdir(dir)) {
		if ((strcmp(ent->d_name, ".") == 0)
				|| (strcmp(ent->d_name, "..") == 0)) {
			continue;
		}
		string filename = dirpath + ent->d_name;
		path_vec.push_back(filename);
	}
	closedir(dir);

	return 0;
}

// 以块方式读取文件
bool getFileLine(ifstream &fin, string& strLine) {
	const unsigned int BUFLEN = 200 * 1000 * 1000;

	static char * pcBigBuf = new char[BUFLEN];
	static int dwTotalLen = 0;
	static int dwNowPos = 0;
	if (pcBigBuf == NULL) {
		return false;
	}

	string strTemp;
	if (dwNowPos >= dwTotalLen) {
		memset(pcBigBuf, 0, BUFLEN);
		fin.read(pcBigBuf, BUFLEN - 20 * 1024);
		getline(fin, strTemp);
		strcat(pcBigBuf, strTemp.c_str());
		dwTotalLen = strlen(pcBigBuf);
		if (dwTotalLen == 0) {
			delete[] pcBigBuf;
			return false;
		}
		char * pcEnd = pcBigBuf + dwTotalLen;

		char * p = pcBigBuf;
		while (p < pcEnd) {
			p = strchr(p, '\n');
			if (p != NULL) {
				*p = 0;
			} else {
				break;
			}
			p++;
		}
		dwNowPos = 0;
	}

	//char * szLine[10240];
	strLine = pcBigBuf + dwNowPos;
	dwNowPos += strLine.size() + 1;
	return true;
}

// 获取名称中不可缺失词
void findUnlackWord(PreProcData *pre_proc_data, const Resource *resource) {
	set<string> temp_key_set;
	temp_key_set.insert("税务所");
	temp_key_set.insert("派出所");
	temp_key_set.insert("车管站");
	temp_key_set.insert("交警队");

	string temp_key_word = "";

	_mapSet::const_iterator map_it = resource->unlackword.find(
			pre_proc_data->m_main_type);

	if (map_it != resource->unlackword.end()) {
		vector<string>::reverse_iterator rit;
		rit = pre_proc_data->m_name_vec.rbegin();
		for (; rit != pre_proc_data->m_name_vec.rend(); rit++) {
			if (pre_proc_data->m_main_type == "12" && temp_key_word == ""
					&& temp_key_set.find(*rit) != temp_key_set.end()) {
				temp_key_word = *rit;
			}

			if (map_it->second.find(*rit) != map_it->second.end()) {
				pre_proc_data->m_unlack_word_set.insert(*rit);
				//break;
			}
		}

		if (pre_proc_data->m_main_type == "22"
				|| pre_proc_data->m_sub_type == "1924") {
			vector<string>::reverse_iterator rit =
					pre_proc_data->m_bracket_vec.rbegin();
			for (; rit != pre_proc_data->m_bracket_vec.rend(); rit++) {
				if (map_it->second.find(*rit) != map_it->second.end()) {
					pre_proc_data->m_unlack_word_set.insert(*rit);
					//break;
				}
			}
		}
	}

	if (temp_key_word == "税务所") {
		pre_proc_data->m_unlack_word_set.erase("国税局");
		pre_proc_data->m_unlack_word_set.erase("地税局");
	} else if (temp_key_word == "派出所") {
		pre_proc_data->m_unlack_word_set.erase("公安局");
	} else if (temp_key_word == "车管站" || temp_key_word == "交警队") {
		pre_proc_data->m_unlack_word_set.erase("公安");
		pre_proc_data->m_unlack_word_set.erase("公安局");
		pre_proc_data->m_unlack_word_set.erase("交管局");
		pre_proc_data->m_unlack_word_set.erase("交管");
	}
}

// 市旅游局 扩展为 北京市旅游局
void extendGov(PreProcData *pre_proc_data) {
	if (startswith(pre_proc_data->m_format_name, "省")
			&& pre_proc_data->m_province != "") {
		pre_proc_data->m_format_name = pre_proc_data->m_province
				+ pre_proc_data->m_format_name.substr(2,
						pre_proc_data->m_format_name.size() - 2);
	} else if (startswith(pre_proc_data->m_format_name, "市")
			&& pre_proc_data->m_city != "") {
		pre_proc_data->m_format_name = pre_proc_data->m_city
				+ pre_proc_data->m_format_name.substr(2,
						pre_proc_data->m_format_name.size() - 2);
	} else if (startswith(pre_proc_data->m_format_name, "区")
			&& pre_proc_data->m_district != ""
			&& endswith(pre_proc_data->m_district, "区")) {
		pre_proc_data->m_format_name = pre_proc_data->m_district
				+ pre_proc_data->m_format_name.substr(2,
						pre_proc_data->m_format_name.size() - 2);
	}
}

// 上海火车站 扩展为 上海站
void extendTrain(PreProcData *pre_proc_data) {
	if (pre_proc_data->m_format_name.size() > 6) {
//		if (endswith(pre_proc_data->m_format_name, "火车站")
//				&& pre_proc_data->m_format_name != "火车站") {
//			pre_proc_data->m_format_name = pre_proc_data->m_format_name.substr(
//					0, pre_proc_data->m_format_name.size() - 6) + "站";
//		}

		if (!startswith(pre_proc_data->m_format_name, "火车站")) {
			pre_proc_data->m_format_name = replaceStr(
					pre_proc_data->m_format_name, "火车站", "站");
		}
	}
}

// 获取数据的行政级别
void findGovLevel(PreProcData *pre_proc_data, const Resource *resource) {
	if (pre_proc_data->m_name_vec.size() == 1) {
		if (startswith(pre_proc_data->m_format_name, "国家")
				|| startswith(pre_proc_data->m_format_name, "中国")) {
			pre_proc_data->m_gov_level = 1;
			pre_proc_data->m_gov_lvl_kw = "中国";
		}
	} else {
		vector<string>::reverse_iterator rit =
				pre_proc_data->m_name_vec.rbegin();
		for (; rit != pre_proc_data->m_name_vec.rend(); ++rit) {
			if (endswith(*rit, "村")) {
				// 村级
				pre_proc_data->m_gov_level = 8;
				pre_proc_data->m_gov_lvl_kw = *rit;
				break;
			} else if (endswith(*rit, "乡")) {
				// 村级
				pre_proc_data->m_gov_level = 7;
				pre_proc_data->m_gov_lvl_kw = *rit;
				break;
			} else if (endswith(*rit, "镇")) {
				// 村级
				pre_proc_data->m_gov_level = 6;
				pre_proc_data->m_gov_lvl_kw = *rit;
				break;
			} else if (endswith(*rit, "县")) {
				// 村级
				pre_proc_data->m_gov_level = 5;
				pre_proc_data->m_gov_lvl_kw = *rit;
				break;
			} else if (startswith(pre_proc_data->m_district, *rit)) {
				// 区级
				pre_proc_data->m_gov_level = 4;
				pre_proc_data->m_gov_lvl_kw = *rit;
				break;
			} else if (startswith(pre_proc_data->m_city, *rit)) {
				// 市级
				pre_proc_data->m_gov_level = 3;
				pre_proc_data->m_gov_lvl_kw = *rit;
				break;
			} else if (startswith(pre_proc_data->m_province, *rit)) {
				// 省级
				pre_proc_data->m_gov_level = 2;
				pre_proc_data->m_gov_lvl_kw = *rit;
				break;
			} else if (resource->stateSet.find(*rit)
					!= resource->stateSet.end()) {
				pre_proc_data->m_gov_level = 1;
				pre_proc_data->m_gov_lvl_kw = *rit;
				break;
			}
		}
	}
}

void findDizhi(PreProcData *pre_proc_data, const Resource *resource) {
	vector<string>::reverse_iterator rit = pre_proc_data->m_name_vec.rbegin();
	for (; rit != pre_proc_data->m_name_vec.rend(); ++rit) {
		if (resource->dizhiSet.find(*rit) != resource->dizhiSet.end()) {
			pre_proc_data->m_dizhi = *rit;
			break;
		}
	}
}

void findDoor(PreProcData *pre_proc_data, const Resource *resource) {
//	if(endswith(pre_proc_data->m_format_name, "门")){
//
//	}
}

// 获取括号中的以店结尾的可能的分店信息
void findFendian(PreProcData *pre_proc_data, const Resource *resource) {
	if (pre_proc_data->m_raw_bracket_vec.size() > 0) {
		string fendian =
				pre_proc_data->m_raw_bracket_vec[pre_proc_data->m_raw_bracket_vec.size()
						- 1];

		if (resource->areaTailSet.find(fendian)
				!= resource->areaTailSet.end()) {
//			pre_proc_data->m_format_name = pre_proc_data->m_format_name.substr(
//					0, pre_proc_data->m_format_name.size() - fendian.size());

			if (endswith(fendian, "分店")) {
				fendian = fendian.substr(0, fendian.size() - 4);
			} else if (endswith(fendian, "店")) {
				fendian = fendian.substr(0, fendian.size() - 2);
			}
			pre_proc_data->m_fendian = fendian;
		}
	}
}

// 长词后缀
void findTail(string &str, string &result, const set<string> &tailSet) {
	string temp("");
	size_t strLen = str.length();
	set<string>::const_iterator it;

	for (int i = 0; i < strLen; i++) {
		temp = str.substr(i, strLen - i);
		it = tailSet.find(temp);
		if (it != tailSet.end()) {
			// 找到后缀
			if (i > 2) {
				result = temp;
			}
		}
	}
}

// 识别 2院 这种分词已经在一起的数量词
bool isNumWord(string &str, const set<string> &measure) {
	vector<string> temp_vec;
	split2word(str, temp_vec);
	string temp("");

	vector<string>::iterator srcIt = temp_vec.begin();

	for (; srcIt < temp_vec.end(); ++srcIt) {
		if ((*srcIt) == "第") {
			if (srcIt + 1 < temp_vec.end() && isAlpDig(*(srcIt + 1))) {
				// num
				return true;
			}
		} else if (isAlpDig(*srcIt)) {
			if (srcIt + 1 < temp_vec.end()) {
				if (measure.find(*(srcIt + 1)) != measure.end()) {
					return true;
				} else {
					return false;
				}
			}
		}
	}

	return false;
}

void findNumWord(PreProcData *pre_proc_data, const Resource *resource) {
	vector<string>::iterator it;

	it = pre_proc_data->m_name_vec.begin();
	while (it != pre_proc_data->m_name_vec.end()) {
		if (isNumWord(*it, resource->measure)) {
			pre_proc_data->m_num_name_set.insert(*it);
			it = pre_proc_data->m_name_vec.erase(it);
		} else {
			it++;
		}
	}
}

//void findParkPrefix(PreProcData *pre_proc_data, const Resource *resource) {
//	if (pre_proc_data->m_type == "191100"
//			|| pre_proc_data->m_soso_type == "191100") {
//		vector<string>::reverse_iterator it =
//				pre_proc_data->m_name_vec.rbegin();
//		for (; it != pre_proc_data->m_name_vec.rend(); ++it) {
//			if (*it == "停车库" && (it + 1) != pre_proc_data->m_name_vec.rend()) {
//				if (*(it + 1) != "大厦") {
//					pre_proc_data->m_park_prefix = replaceStr(*(it + 1), "大厦",
//							"");
//				} else if ((it + 2) != pre_proc_data->m_name_vec.rend()) {
//					pre_proc_data->m_park_prefix = *(it + 2);
//				}
//				break;
//			}
//		}
//	}
//}

double sumIDF(const vector<string> &data_vec, const Resource *resource) {
	double sum = 0.0;
	_mapStrDbl::const_iterator idf_it;
	vector<string>::const_iterator it = data_vec.begin();
	for (; it != data_vec.end(); ++it) {
		idf_it = resource->idf.find(*it);
		if (idf_it != resource->idf.end()) {
			sum += idf_it->second;
		} else {
			sum += resource->max_idf;
		}
	}

	return sum;
}

void getSetSim(const set<string> &base_set, const set<string> &inc_set,
		double &score, vector<string> &same_vec, vector<string> &diff_base_vec,
		vector<string> &diff_inc_vec, const Resource *resource) {
	//both in base and inc
	set_intersection(base_set.begin(), base_set.end(), inc_set.begin(),
			inc_set.end(),
			insert_iterator<_vector>(same_vec, same_vec.begin()));

	//only in base
	set_difference(base_set.begin(), base_set.end(), inc_set.begin(),
			inc_set.end(),
			insert_iterator<_vector>(diff_base_vec, diff_base_vec.begin()));

	//only in inc
	set_difference(inc_set.begin(), inc_set.end(), base_set.begin(),
			base_set.end(),
			insert_iterator<_vector>(diff_inc_vec, diff_inc_vec.begin()));

	double same_score = sumIDF(same_vec, resource) * 2;
	double diff_score = sumIDF(diff_base_vec, resource)
			+ sumIDF(diff_inc_vec, resource);

	score = same_score - diff_score;
}

void getSetSim(set<string> &base_set, set<string> &inc_set, double &score,
		const Resource *resource) {
	vector<string> same_vec, diff_base_vec, diff_inc_vec;
	getSetSim(base_set, inc_set, score, same_vec, diff_base_vec, diff_inc_vec,
			resource);
}

bool isSpecDoor(const set<string> &door_set, const Resource *resource) {
	set<string>::const_iterator it = door_set.begin();
	set<string>::const_iterator direct_it;
	for (; it != door_set.end(); ++it) {
		direct_it = resource->directionSet.begin();
		for (; direct_it != resource->directionSet.end(); ++direct_it) {
			if ((*it).find(*direct_it) != string::npos) {
				return true;
			}
		}
	}

	return false;
}

void filterSet(vector<string> &data_vec, set<string> &filter_set) {
	vector<string>::iterator it = data_vec.begin();
	while (it != data_vec.end()) {
		if (filter_set.find(*it) != filter_set.end()) {
			it = data_vec.erase(it);
		} else {
			++it;
		}
	}
}

void filter(const string &str, vector<string> &data_vec,
		vector<string> &result_vec) {
	vector<string>::iterator it = data_vec.begin();
	while (it != data_vec.end()) {
		if (str.find(*it) != string::npos) {
			result_vec.push_back(*it);
			it = data_vec.erase(it);
		} else {
			++it;
		}
	}
}

void getNumWord(set<string> &num_set, vector<string> &name_vec) {
	set<string>::iterator it = num_set.begin();
	for (; it != num_set.end(); ++it) {
		if (endswith(*it, "中") || endswith(*it, "小")) {
			name_vec.push_back(*it);
		}
	}
}

void formatName(string &name) {
	unsigned pos = name.rfind(' ');
	string temp_name = "";
	if (pos != string::npos) {
		temp_name = name.substr(0, pos) + "("
				+ name.substr(pos, name.size() - pos) + ")";
		name = temp_name;
	}
}

// 增加量词
// 处理空港科技大厦a 中的a，扩展为a座
void addSuffix(PreProcData *pre_proc_data) {
	if (pre_proc_data->m_sub_type == "2812") {
		if (pre_proc_data->m_name_vec.size() > 0
				&& isAlpha(
						pre_proc_data->m_name_vec[pre_proc_data->m_name_vec.size()
								- 1])
				&& pre_proc_data->m_name_vec[pre_proc_data->m_name_vec.size()
						- 1].size() == 1) {
			pre_proc_data->m_name_vec[pre_proc_data->m_name_vec.size() - 1] =
					pre_proc_data->m_name_vec[pre_proc_data->m_name_vec.size()
							- 1] + "座";
		}
	}
}

bool checkInclude(const set<string> &base_unlack_set,
		const set<string> &inc_unlack_set, const set<string> &inc_name_set) {
	set<string> temp_set;

	temp_set.insert(inc_unlack_set.begin(), inc_unlack_set.end());
	temp_set.insert(inc_name_set.begin(), inc_name_set.end());

	return checkSetInclude(base_unlack_set, temp_set);
}

bool checkInclude(const set<string> &base_door_set,
		const set<string> &inc_door_set, const string &inc_name) {
	_set::const_iterator it = base_door_set.begin();
	for (; it != base_door_set.end(); ++it) {
		if (inc_door_set.find(*it) == inc_door_set.end()
				&& inc_name.find(*it) == string::npos)
			return false;
	}
	return true;
}

void transSpecWord(PreProcData *pre_proc_data) {
	vector<string>::iterator it = pre_proc_data->m_name_vec.begin();
	for (; it != pre_proc_data->m_name_vec.end(); ++it) {
		if (*it == "大酒店") {
			*it = "酒店";
		} else if ((*it == "附院" || *it == "分院")
				&& pre_proc_data->m_main_type == "20") {
			*it = "院";
		}
	}
}

void formatTypeName(PreProcData *pre_proc_data, const Resource *resource) {
	if (pre_proc_data->m_format_name.size() > 2) {
		string tail = pre_proc_data->m_format_name.substr(
				pre_proc_data->m_format_name.size() - 2, 2);
		string temp_name = pre_proc_data->m_format_name.substr(0,
				pre_proc_data->m_format_name.size() - 2);
		if (tail == "店"
				&& resource->typeNameMap.find(temp_name)
						!= resource->typeNameMap.end()) {
			pre_proc_data->m_format_name = temp_name;
		}
	}
}

bool isTypeConflict(const string &base_type, const string &inc_type,
		const Resource *resource) {
	string base_main_type = "", inc_main_type = "";
	if (base_type.size() > 2) {
		base_main_type = base_type.substr(0, 2);
	}

	if (inc_type.size() > 2) {
		inc_main_type = inc_type.substr(0, 2);
	}

	if (base_main_type != "" && inc_main_type != ""
			&& base_main_type != inc_main_type) {
		_mapSet::const_iterator it = resource->typeMap.find(base_type);
		if (it != resource->typeMap.end()) {
			_hashset::const_iterator it_set = it->second.find(inc_type);
			if (it_set == it->second.end()) {
				//类型有冲突
				return true;
			}
		}
	}

	return false;
}

bool isConflict(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource) {
	if (isTypeConflict(base_data->m_soso_type, inc_data->m_type, resource)
			|| isTypeConflict(inc_data->m_soso_type, base_data->m_type,
					resource)
			|| isTypeConflict(base_data->m_soso_type, inc_data->m_soso_type,
					resource)) {
		return true;
	}

	return false;
}

void formatNumName(string &name) {
	int pos = name.find("第");
	string temp = "";

	if (pos >= 0 && (pos + 3) <= name.size()) {
		temp = name.substr(pos + 2, 1);
		if (isDigit(temp)) {
			replaceStr(name, "第", "");
		}
	}
}
