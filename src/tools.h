/*
 * tools.h
 *
 *  Created on: 2012-10-23
 *      Author: stefanyang
 *
 *  һЩ���õĻ������ߺ���
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <cstdio>
#include <cstring>
#include <errno.h>
#include <string>
#include <iostream>
#include "utils/contrib.h"
#include "data.h"
#include "common.h"
#include "typeDefine.h"
#include "utils/wordsplit/wordsplit.h"
using namespace std;

//========================================
// ����ת����
//========================================
class Trad2Simp {
private:
	unsigned short** m_trad2simp_tbl;
	size_t m_map_arr_size;

public:
	bool init();
	bool destroy();
	bool func(char* pcDest, const char* pcSrc, const unsigned int dwSrcLen);
	string func(const string &str_src);

	Trad2Simp() {
		if (!init()) {
			cout << "trad2simp init err" << endl;
		}
	}

	~Trad2Simp() {
		destroy();
	}
};

//========================================
// tools �ṹ��
//========================================
class Tools {
public:
	Trad2Simp *m_trad2simp;
	WordSplit *m_wordsplit;

public:
	Tools() {
		m_trad2simp = new Trad2Simp();
		m_wordsplit = new WordSplit();
	}

	~Tools() {
		delete m_trad2simp;
		delete m_wordsplit;
	}
};

//========================================
// ������̬��tools
//========================================
// ��ӡԤ������
void print(PreProcData *pre_proc_data);
void print(const SimTempCache *sim_temp_cache);

// ȫ��ת���
string sbc2dbc(const string &str);

// ��дתСд
void lower(string &str);

// ȥ��������
void removePunc(string &values, bool needBra);

// ��������ת����������
void transZhNum(string &str, const _hashset &number, const _hashset &number2,
		int length);
void transZhNum(_set &src, const _hashset &number, const _hashset &number2,
		int length);

// ȥ�������ڵ�����
void removeBracket(string &str, vector<string> &result);

// ȥ�����ƺ����**���׺
//int findAreaTail(string &formatName, vector<string> &bracket_vec,
//		const _hashset &areaTailSet);
int findAreaTail(PreProcData *pre_proc_data, const Resource *resource);

// ��ȡ�����е�������׺
void getGovTail(std::string &src, string &gov_tail, const _mapSet &typeword);

// ��ȡ��������
void findObjName(PreProcData *pre_proc_data, const Resource *resource);

// ����ֻΪ���ʹ�ʱ���������ڵ�������ӵ��淶������
void addContentForTWName(PreProcData *pre_proc_data, const _mapSet &typeword);

// �ϲ�����,��ĸ����
void mergeNumAlp(PreProcData *pre_proc_data, const Resource *resource);

// �ϲ���β�ر�ĵ���
void mergeTailSglWord(PreProcData *pre_proc_data);

// ȥ������������
void filteDistrict(PreProcData *pre_proc_data, const Resource *resource,
		int &count);

// �淶����������׺
void formatGov(string &name, string &gov_tail, const _mapSet &typeword);
void formatGov(_vector &src, string &gov_tail, const _mapSet &typeword);

// ������ͬ���������ӳ������
bool findType(const string &base_main_type, const string &base_sub_type,
		const string &base_type, const string &inc_main_type,
		const string &inc_sub_type, const string &inc_type,
		const _mapSet &type_map, SimTempCache *sim_temp_cache);
bool findType(const PreProcData *base_data, const PreProcData *inc_data,
		const _mapSet &type_map, SimTempCache *sim_temp_cache);
bool isSameType(const string &base_type, const string &inc_type,
		const _mapSet &type_map);

// һ�����Ƿ������һ��vector��
bool isInclude(const string &str, const vector<string> &str_vec);

// ����ȱʧ�ʵ�ʶ��
void findUnlackWord(PreProcData *pre_proc_data, const _mapSet &unlack_map);

// �Ӳ�ͬ�ʲ��ֲ����Ƿ��а�����ȷ��ؼ��ʵĴ�
//void findDiffAccurateKeyword(const vector<string> &data_vec,
//		const string &base_str, const string &inc_str, const Resource *resource,
//		vector<string> &result_vec);
void findDiffAccurateKeyword(const vector<string> &data_vec,
		const set<string> &word_set, const Resource *resource,
		vector<string> &result_vec);

// ����00�ʱ��г��ֵĴ�
void filte(vector<string> &data_vec, const Resource *resource);

// �����Ʒִʺ󣬴����β�Ĵ���ӵ�grid����
void setGridKeyWord(const vector<string> &name_vec, set<string> &grid_set);

// �淶������Ϣ
void formatDoorSet(set<string> &door_set);

// ��鲻ͬ�����Ƿ�����з�Ƶ����01�ʱ��
bool hasNotFreqTypeWord(vector<string> &data_vec, const Resource *resource,
		double theshold, int &count, bool checkGovTail);

// ��ȡĳ·���µ������ļ���������������Ŀ¼��·����
int getPaths(const std::string &path, vector<string> &path_vec);

// �Կ鷽ʽ��ȡ�ļ�
bool getFileLine(ifstream &fin, string& strLine);

// ��ȡ�����в���ȱʧ��
void findUnlackWord(PreProcData *pre_proc_data, const Resource *resource);

// �����ξ� ��չΪ ���������ξ�
void extendGov(PreProcData *pre_proc_data);

// �Ϻ�վ ��չΪ �Ϻ���վ
void extendTrain(PreProcData *pre_proc_data);

// ��ȡ���ݵ���������
void findGovLevel(PreProcData *pre_proc_data, const Resource *resource);

// ��ȡ��֧������
void findDizhi(PreProcData *pre_proc_data, const Resource *resource);

// ��ȡ������Ϣ
void findDoor(PreProcData *pre_proc_data, const Resource *resource);

// ��ȡ�����е��Ե��β�Ŀ��ܵķֵ���Ϣ
void findFendian(PreProcData *pre_proc_data, const Resource *resource);

// ��ȡ��׺�Ƿ��ڴʱ��г���
void findTail(string &str, string &result, const set<string> &tailSet);

// ʶ�� 2Ժ ���ִַ��Ѿ���һ���������
bool isNumWord(string &str, const set<string> &measure);
void findNumWord(PreProcData *pre_proc_data, const Resource *resource);

// ʶ�� �ű�����ͣ���� �е��ű�
void findParkPrefix(PreProcData *pre_proc_data, const Resource *resource);

// ����idf��ֵ֮��
double sumIDF(const vector<string> &data_vec, const Resource *resource);

// ��������set�Ƿ�����
void getSetSim(const set<string> &base_set, const set<string> &inc_set,
		double &score, vector<string> &same_vec, vector<string> &diff_base_vec,
		vector<string> &diff_inc_vec, const Resource *resource);
void getSetSim(set<string> &base_set, set<string> &inc_set, double &score,
		const Resource *resource);

// �ж�������Ϣ���Ƿ����������Ϣ
bool isSpecDoor(const set<string> &door_set, const Resource *resource);

// ��������filter_set�еĴʴ�data_vec�й��˵�
void filterSet(vector<string> &data_vec, set<string> &filter_set);

// ��������str�еĴʴ�data_vec�й��˵�
void filter(const string &str, vector<string> &data_vec,
		vector<string> &result_vec);

// �� **�еȷŵ����������еĴʸ��Ƶ�name��
void getNumWord(set<string> &num_set, vector<string> &name_vec);

// ��Զ���С��������������������ر���
// �����а����ո񣬿ո��������ݵ��������ڵ�����
void formatName(string &name);

// ��������
// ����ոۿƼ�����a �е�a����չΪa��
void addSuffix(PreProcData *pre_proc_data);

// ������Ƿ��а���
bool checkInclude(const set<string> &base_unlack_set,
		const set<string> &inc_unlack_set, const set<string> &inc_name_set);
bool checkInclude(const set<string> &base_door_set,
		const set<string> &inc_door_set, const string &base_name);

// ���������������ͬ�������Ĵ�������滻
void transSpecWord(PreProcData *pre_proc_data);

// ���������ƹ淶��
void formatTypeName(PreProcData *pre_proc_data, const Resource *resource);

bool isTypeConflict(const string &base_type, const string &inc_type,
		const Resource *resource);
bool isConflict(const PreProcData *base_data, const PreProcData *inc_data,
		const Resource *resource);

void formatNumName(string &name);
#endif /* TOOLS_H_ */
