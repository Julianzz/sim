/*
 * data.h
 *
 *  Created on: 2012-9-24
 *      Author: stefanyang
 *
 *  ����poi�ۺ�����Ҫ�õĻ����ṹ���ࣩ
 */

#ifndef DATA_H_
#define DATA_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdio.h>
#include <string.h>
#include "utils/computeAddressSimilarity.h"
using namespace std;

//===============================================
// �ۺ���Ҫ��Ϊ����Ļ����ֶζ���
//===============================================
class Record {
public:
	bool m_is_null;				// ��Ǹü�¼�Ƿ�Ϊ��, true��ʾΪ�գ�false��ʾ����
	int m_status;				// �ü�¼�����ݿ��е�״̬��0: ���ã�1��������

	int m_addr_level;			// ��ַ����
	unsigned long long m_id;	// ��¼id
	double m_lat;				// γ��
	double m_lng;				// ����
	string m_name;				// ����
	string m_addr;				// ��ַ
	string m_split_addr;		// �ָ��ĵ�ַ
	string m_type;				// ���ʹ��룬�磺241000
	string m_type_name;			// ��������
	string m_source;			// ��Դ
	string m_province;			// ʡ
	string m_city;				// ��
	string m_district;			// ��
	string m_tel;				// �绰
	string m_accurate_type;		// �ӵ�ؼ���ʶ��ģ�鷵�ص�ֵ

public:
	void clear() {
		m_is_null = true;
		m_status = 0;

		m_id = 0;
		m_lat = 0.0;
		m_lng = 0.0;
		m_name = "";
		m_addr = "";
		m_split_addr = "";
		m_addr_level = 100;
		m_type = "";
		m_type_name = "";
		m_source = "";
		m_province = "";
		m_city = "";
		m_district = "";
		m_tel = "";
		m_accurate_type = "";
	}

	Record() {
		clear();
	}
};

//===============================================
// ÿ��poi����Ԥ���������Ľ����
//===============================================
class PreProcData {
public:
	bool m_is_null;					// ��Ǹü�¼�Ƿ�Ϊ��, true��ʾΪ�գ�false��ʾ����
	int m_status; 					// �ü�¼�����ݿ��е�״̬��0: ���ã�1��������

	// raw
	unsigned long long m_id;		// ��¼id

	double m_lat;					// γ��
	double m_lng;					// ����

	string m_name;					// ����
	string m_addr;					// ��ַ
	string m_split_addr;			// �ָ��ĵ�ַ
	int m_addr_level;				// ��ַ����
	string m_type;					// ���ͱ���
	string m_source;				// ��������
	string m_province;				// ʡ
	string m_city;					// ��
	string m_district;				// ��
	set<string> m_tel_set;			// �绰

	// preproc
	string m_accurate_type;			// �ӵ�ʶ�������
	string m_accurate_keyword;		// �ӵ�ʶ��Ĺؼ���
	vector<string> m_accurate_kw_vec;
	set<string> m_accurate_kw_set;
	string m_format_name; 			// �淶���������
	string m_del_punc_name; 		// �淶����������ŵ�����
	unsigned int m_del_punc_name_len;	// m_del_punc_name��������������һ����
	string m_bigram_name;			// ����bigram�зֵ�����
	vector<string> m_raw_bracket_vec;	// �򵥹淶��������е�����

	vector<string> m_name_vec;		// ���Ʒִʺ�Ľ��
	vector<string> m_bracket_vec;	// ���������ݷִʵĽ��
	set<string> m_name_set;
	set<string> m_bracket_set;

	string m_main_type;				// ԭʼ����
	string m_sub_type;				// ԭʼ����
	string m_soso_type;				// ���ݺ�׺ʶ������ͱ���
	string m_soso_main_type;		// ��׺ʶ�����͵Ĵ������
	string m_soso_sub_type;			// ��׺ʶ�����͵��������
	string m_accurate_main_type;	// �ӵ�ʶ������ʹ���
	string m_accurate_sub_type;		// �ӵ�ʶ�����������

	// ��ַ������Ҫ��Ԥ��������
	map<int, string> m_addrsim_xzqh_map;
	map<string, string> m_addrsim_road_map;
	set<string> m_addrsim_community_set;
	set<string> m_addrsim_poi_set;
	set<string> m_addrsim_hn_set;
	set<string> m_addrsim_home_set;
	set<string> m_addrsim_floor_set;

	set<string> m_num_name_set;		// ����������ʶ����
	set<string> m_door_set;			// ����������Ϣʶ�������磺���ú�԰���š��еġ����š�
	set<string> m_dpmark_set;		// �����е������Ǵʣ��磺��������԰��

	double m_obj_max_distance;		// ����������꣬����ֱ�Ӿۺϵ�������
	set<string> m_obj_name_set;		// ���ư�����������������
	string m_gov_tail;				// �����ȵ������������׺ʶ����

	// ����ֱ���滻��ͬ�������ϣ��磺���Ұ���ƥ���������� ���� ��������
	// ��Ҫ����һЩ��˾�ļ�ƺ�ѧУ�ĺϲ����ƣ��磺����ҽ�ƴ�ѧ ���� ������ѧҽѧ��
	set<int> m_smname_group_set;

	set<string> m_unlack_word_set;	// �����а����Ĳ���ȱʧ�ʣ��磺��ҽ��Ժ���� �е� �����ơ�

	//======================================
	// �����ܱ߼���ʱ������Ƽ���
	// ԭ�棺ʹ��bigram�����磺�������� ������ ["����", "����", "����"]
	// ԭ�����⣺�ٻ��ʸߣ����������ٻؽ϶࣬����Ӱ���ٶȣ�
	// 			�Ҷ�����Щ��ƣ���д��ͬ���ֵ��ٻ�Ч������
	// �°棺���ڶ�����ʹ��bigram�����ڳ�����ʹ�÷ִʣ���Ϲ淶���ȫ�ơ�ƴ���ȣ�
	//		�ٻ��ʲ����͵�ǰ���£������ٻصľ�׼��
	set<string> m_grid_name_set;

	// ���������ļ���
	// 0: ��֪��
	// 1: ���Ҽ�
	// 2: ʡ��
	// 3: �м�
	// 4: ����
	int m_gov_level;

	// ���������ļ���ؼ��ʣ��磺�й�����������������
	string m_gov_lvl_kw;

	string m_dizhi; 				// С���ļ��ұ�����
	string m_fendian;				// ������� �ϵ»��йش���С��йش塱��ʶ����

public:
	void clear() {
		m_is_null = true;
		m_status = 0;

		m_id = 0;

		m_lat = 0.0;
		m_lng = 0.0;

		m_name = "";
		m_addr = "";
		m_split_addr = "";
		m_addr_level = 100;
		m_type = "";
		m_source = "";
		m_province = "";
		m_city = "";
		m_district = "";
		m_tel_set.clear();

		// preproc
		m_accurate_type = "";
		m_accurate_keyword = "";
		m_accurate_kw_vec.clear();
		m_accurate_kw_set.clear();

		m_format_name = ""; // format name string
		m_del_punc_name = ""; // del all punc from name
		m_del_punc_name_len = 0;
		m_bigram_name = "";
		m_raw_bracket_vec.clear(); // after format, the deleted string that in brackets

		m_name_vec.clear(); // wordsplit result
		m_bracket_vec.clear();
		m_name_set.clear(); // wordsplit result in set
		m_bracket_set.clear();

		m_main_type = "";
		m_sub_type = "";
		m_soso_type = "";
		m_soso_main_type = "";
		m_soso_sub_type = "";
		m_accurate_main_type = "";
		m_accurate_sub_type = "";

		m_addrsim_xzqh_map.clear();
		m_addrsim_road_map.clear();
		m_addrsim_community_set.clear();
		m_addrsim_poi_set.clear();
		m_addrsim_hn_set.clear();
		m_addrsim_home_set.clear();
		m_addrsim_floor_set.clear();

		m_num_name_set.clear();
		m_door_set.clear();
		m_dpmark_set.clear();

		m_obj_max_distance = 0.0;
		m_obj_name_set.clear();
		m_gov_tail = "";
		m_smname_group_set.clear();
		m_unlack_word_set.clear();
		m_grid_name_set.clear();

		m_gov_level = 0;
		m_gov_lvl_kw = "";

		m_dizhi = "";
		m_fendian = "";
	}

	PreProcData() {
		clear();
	}
};

//===============================================
// �������Ԥ�������Ľṹ
//===============================================
typedef map<unsigned long long, PreProcData> PreProcMap;


//===============================================
// �����ܱ��ٻص�key�ṹ��
//===============================================
struct GridKey {
	int lat;
	int lng;
	string keyword;

	GridKey() {
		lat = 0;
		lng = 0;
		keyword = "";
	}
};

//===============================================
// ��Ϊʹ�ýṹ����Ϊmap��key
// ��������ܱ��ٻ�ʹ�õĽṹ�����˶��ƵıȽϺ���
//===============================================
struct GridKeyCmp {
	bool operator ()(const GridKey& key1, const GridKey& key2) const {
		if (key1.lat < key2.lat) {
			return true;
		} else if (key1.lat == key2.lat) {
			if (key1.lng < key2.lng)
				return true;
			else if (key1.lng == key2.lng) {
				if (key1.keyword.compare(key2.keyword) < 0)
					return true;
				else
					return false;
			} else
				return false;
		}

		return false;
	}
};

//===============================================
// ����ܱ߼�����������keyΪ�з�����+����Ľṹ�壬valueΪpoiid
//===============================================
typedef map<GridKey, set<unsigned long long>, GridKeyCmp> GridMap;

//===============================================
// �����ļ�תΪ���ýṹ��Ҫ���������ݶ���
// ˵����
//		1. �����ļ�֧��ÿ��poiռһ�У����ض��ַ�����Ϊ�ָ���Ľṹ
//		2. �ṹ����Ҫ����record��Ӧ�ֶ����ĵ��е�λ����������0��ʼ����
//===============================================
class CsvIndexCfg {
public:
	bool m_debug;			// �Ƿ���debug��ʽ��true��

	int m_id;				// �ĵ���id��index
	int m_lat;				// �ĵ���γ�ȵ�index
	int m_lng;				// �ĵ��о��ȵ�index
	int m_name;				// �ĵ������Ƶ�index
	int m_addr;				// �ĵ��е�ַ��index
	int m_split_addr;		// �ĵ��зָ��ַ��index
	int m_addr_level;		// �ĵ��е�ַ�����index
	int m_type;				// �ĵ����������index
	int m_type_name;		// �ĵ����������Ƶ�index
	int m_source;			// �ĵ�����Դ��index
	int m_province;			// �ĵ���ʡ��index
	int m_city;				// �ĵ����е�index
	int m_district;			// �ĵ�������index
	int m_accurate_type;	// �ĵ����ӵ�ʶ������index
	int m_tel;				// �ĵ��е绰��index
	int m_status;			// �ĵ�����Ч״̬��index

	int m_max_index;		// �ĵ�����Ҫʹ���ֶε�����index����Ҫ�����ڼ��ǰ��������Ƿ�������
	int m_thread_num;		// ʹ���߳�����
	string m_delim;			// �ָ���
public:
	void clear() {
		m_debug = false;

		m_id = -1;
		m_lat = -1;
		m_lng = -1;
		m_name = -1;
		m_addr = -1;
		m_split_addr = -1;
		m_addr_level = -1;
		m_type = -1;
		m_type_name = -1;
		m_source = -1;
		m_province = -1;
		m_city = -1;
		m_district = -1;
		m_accurate_type = -1;
		m_tel = -1;
		m_status = -1;

		m_max_index = 52;
		m_thread_num = 4;

		m_delim = "";
	}

	CsvIndexCfg() {
		clear();
	}
};

//=====================================================================
// poiһ��һ���ƶȼ�������������м�������ṹ��
//=====================================================================
class SimTempCache {
public:
	bool m_is_null;						// ��ǽṹ�Ƿ�Ϊ��
	int m_status; 						// 0: ���ã�1��������
	bool m_is_base; 					// ��base�����ҵ���������

	// chn���͵����ݾۺϱ��
	// ��ʹ�øñ��ʱ����ʹ��һЩ�ر�Ĳ���
	bool m_is_mode_chn;

	// index���Լ��ۺϱ��
	bool m_is_mode_self;


	unsigned long long m_base_id;		// �ۺ�ʱ��Ϊ��baseid
	unsigned long long m_inc_id;		// �ۺ�ʱ��Ϊ��incid

	string m_base_name;					// �ۺ�ʱ��Ϊ��base_name
	string m_inc_name;					// �ۺ�ʱ��Ϊ��inc_name

	// step
	// ֵΪtrue���ýڵ���ȷ���Ľ���������ڵ㲻��Ҫ��������
	// ֵΪfalse���ýڵ�û��ȷ���Ľ���������ڵ���Ҫ��������
	map<string, bool> m_rule_result_map;

	double m_distance;					// poi������
	double m_addr_sim;					// ��ַ���ƶ�
	set<string> m_obj_name_set;			// ����poi��ͬ��������������
	double m_max_obj_distance;			// ���������ֱ�Ӿۺϵ�������
	set<string> m_spec_obj_name_set;	// �ر����͵��������������Ƽ��ϣ������վ��ͣ������
	double m_max_spec_obj_distance;		// �������Ϳ���ֱ�Ӿۺϵ�������
	int m_len_diff;						// �༭����
	bool m_name_is_include;				// �����Ƿ��а�����ϵ
	AddressSimilarityValue m_addr_sim_value; 	//��ŵ�ַ���ƶȼ����ϸ��

	set<string> m_same_tel_set;			// ��ͬ�绰����
	set<string> m_same_type_set;		// ��ͬ����ͨ��ӳ��ۺ�ʱ��������ͱ�
	string m_same_type_str; 			// ����poiΪ��ͬ���ͣ�������ӳ�䣩����������ֵ����Ϊ��

	vector<string> m_same_words_vec;	// ��������ͬ�Ĵ�
	vector<string> m_sub_same_words_base_vec;	// base��������ͬ����Ϊ����Ҫ�Ĵ�
	vector<string> m_sub_same_words_inc_vec;	// inc��������ͬ����Ϊ����Ҫ�Ĵ�
	vector<string> m_diff_words_base_vec;		// base�����в���ͬ�Ĵ�
	vector<string> m_diff_words_inc_vec;		// inc�����в���ͬ�Ĵ�
	vector<string> m_sub_diff_words_base_vec;	// base�����в���ͬ����Ϊ����Ҫ�Ĵ�
	vector<string> m_sub_diff_words_inc_vec;	// inc�����в���ͬ����Ϊ����Ҫ�Ĵ�

	vector<string> m_bracket_same_words_vec;	// ���Ƶ���������Ϊ��ͬ�Ĵ�
	vector<string> m_bracket_diff_words_base_vec;	// base������������Ϊ��ͬ�Ĵ�
	vector<string> m_bracket_diff_words_inc_vec;	// inc������������Ϊ��ͬ�Ĵ�

	// accurate keyword
	vector<string> m_accurate_kw_base_vec;		// base�ӵ�ʶ������Ĺؼ���
	vector<string> m_accurate_kw_inc_vec;		// inc�ӵ�ʶ������Ĺؼ���

	// name similarity
	double m_name_same_score;			// ������ͬ���ֵĵ÷�
	double m_name_sub_same_score;		// ���Ƹ���ͬ���ֵĵ÷�
	double m_name_diff_score;			// ���Ʋ���ͬ���ֵĵ÷�
	double m_name_sub_diff_score;		// ���Ƹ���ͬ���ֵĵ÷�
	double m_bracket_score;				// �������������ƶȵ÷�

	// base score
	double m_name_score;				// �������ƶ��ܷ�
	double m_tel_score;					// �绰���ƶ��ܷ�
	double m_addr_score;				// ��ַ���ƶ��ܷ�
	double m_type_score;				// �������ƶ��ܷ�
	double m_distance_score;			// �������ƶ��ܷ�

	//=====================================================================
	// special score
	// ���µĵ÷���Ҫ���ڶ�ǰ��÷ֵ�����
	//=====================================================================
	// ��ͬ�����͡���ͬ����Դ�����ݣ���Ҫ�ڻ����÷ֻ�������һЩ����ԵĴ���
	double m_accurate_type_score;		// �ӵ�ʶ��÷�

	// �����а�������һ��������һ�ߵĽ�β��̬��Ҫ��һЩ�ӷִ���
	// �ʹ�����Ժ����ի   ����ի
	double m_tail_score;

	// һЩ����������Ҫ���������һЩ����
	// ��:
	//    1. ��ͬ����ͨ��ӳ��ۺϵ���Ҫ��һ������
	//    2. ������𡢹�˾���ȸ����������ݽ��оۺϵļ���
	double m_spec_score;

	// �������ư�������һ�����ݵ�������ʱ����Ҫ���ر����͵������ִ���
	// �ϵ»�    **С�ԣ��ϵ»��ԣ�
	double m_in_bracket_score;

	// ����������������ͬ������
	double m_same_num_score;

	// ��Ƶ���ʿ۷�
	// ��������а���һЩ��Ƶ���ʳ����ڲ�ͬ������Ҫ��������������ͬ�Ŀ۷ִ���
	double m_area_sub_poi_score;

	// �����а�����ϵ
	// ��ַ��绰��ͬ�����
	double m_include_addr_tel_score;

	// �����Ƶ÷�
	double m_total_same_score;
	// �ܲ�ͬ�÷�
	double m_total_diff_score;
	// �����ƶȵ÷�
	double m_score;

public:
	void clear() {
		m_is_null = true;
		m_status = 0;
		m_is_base = false;

		m_is_mode_chn = false;
		m_is_mode_self = false;

		m_base_id = 0;
		m_inc_id = 0;

		m_base_name = "";
		m_inc_name = "";

		m_rule_result_map.clear();

		m_distance = 9999.0;
		m_addr_sim = 0.0;
		m_obj_name_set.clear();
		m_max_obj_distance = 9999.0;
		m_spec_obj_name_set.clear();
		m_max_spec_obj_distance = 9999.0;
		m_len_diff = 999;
		m_name_is_include = false;

		m_addr_sim_value.clear();

		m_same_tel_set.clear();
		//m_same_type_vec.clear();
		m_same_type_set.clear();
		m_same_type_str = "";

		m_same_words_vec.clear();
		m_sub_same_words_base_vec.clear();
		m_sub_same_words_inc_vec.clear();
		m_diff_words_base_vec.clear();
		m_diff_words_inc_vec.clear();
		m_sub_diff_words_base_vec.clear();
		m_sub_diff_words_inc_vec.clear();

		m_bracket_same_words_vec.clear();
		m_bracket_diff_words_base_vec.clear();
		m_bracket_diff_words_inc_vec.clear();

		// accurate keyword
		m_accurate_kw_base_vec.clear();
		m_accurate_kw_inc_vec.clear();

		// name similarity
		m_name_same_score = 0.0;
		m_name_sub_same_score = 0.0;
		m_name_diff_score = 0.0;
		m_name_sub_diff_score = 0.0;
		m_bracket_score = 0.0;

		// base score
		m_name_score = 0.0;
		m_tel_score = 0.0;
		m_addr_score = 0.0;
		m_type_score = 0.0;
		m_distance_score = 0.0;

		// special score
		m_accurate_type_score = 0.0;
		m_tail_score = 0.0; // if one in endswith other
		m_spec_score = 0.0;
		m_in_bracket_score = 0.0;
		m_same_num_score = 0.0;
		m_area_sub_poi_score = 0.0;
		m_include_addr_tel_score = 0.0;

		m_total_same_score = 0.0;
		m_total_diff_score = 0.0;
		m_score = -1.0;
	}

	SimTempCache() {
		clear();
	}
};

//=====================================================================
// poi�ۺ����ڱ���û�оۺ��ϵ�id����
// ˵����
// 		���ں��������˳�ͻ��⣬���ƶȼ����м�����Ϊ��ͬ��id��ϵҲ�п����ں���
// ��Ϊ��ͻ��ϵ���������Ϊһ�������㣬���Ըýṹ���ڻ����Ѿ�����
//=====================================================================
class SimResultCache {
public:
	map<unsigned long long, bool> m_base_sgl_map;
	map<unsigned long long, bool> m_inc_sgl_map;
};

#endif /* DATA_H_ */
