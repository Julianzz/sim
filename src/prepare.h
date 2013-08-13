/*
 * prepare.h
 *
 *  Created on: 2012-10-10
 *      Author: stefanyang
 *
 *  poi���ƶȼ����漰������Ԥ������
 *
 *  Ԥ������Ҫ������
 *  1. poi��Ҫ�ֶεĹ淶��
 *		�磺ȫ��ת��ǣ������ŵ�ɾ��������ת����
 *  2. poi��������ʶ�𣬰����������������������
 *  3. �ܱ߼����ṹ��ĳ�ʼ��
 *  4. idf�ʱ��׼��
 *  5. ��Ч�绰����Ƶ�绰��Ϊ�����õ绰����ʶ��
 */

#ifndef PREPARE_H_
#define PREPARE_H_

#include <fstream>
#include "typeDefine.h"
#include "data.h"
#include "tools.h"
#include "typeDefine.h"

//=================================================
// ���̲߳����ṹ��
//=================================================
struct PrepareFromCsvParam {
	ifstream *fin; 							// �����ļ�������
	PreProcMap *pre_proc_map; 				// Ԥ�����������map
	const Resource *resource; 				// Ԥ�����л��õ����дʱ���Դ
	const CsvIndexCfg *csv_index_cfg; 		// Ԥ�������漰����������
	pthread_mutex_t *prepare_read_mutex; 	// ���ļ���
	pthread_mutex_t *prepare_setmap_mutex;	// �޸Ľ��map��
	const Tools *tools;						// �ִʡ���ת���ȹ��ߺ���
};

//struct PrepareCsvDirParam {
//	ifstream *fin;						// �����ļ�������
//	PreProcMap *pre_proc_map;			// Ԥ����������map
//	const Resource *resource;			// Ԥ�����л��õ����дʱ���Դ
//	const CsvIndexCfg *csv_index_cfg;	// Ԥ�������漰����������
//	const Tools *tools;					// �ִʡ���ת���ȹ��ߺ���
//};

//=================================================
// �ж������Ƿ���Ч
//=================================================
// �ж�Ԥ�������Ƿ��ǿ�������
bool isBadPreProcData(PreProcData *pre_proc_data);
// �ж����ò����ṹ���Ƿ�����Ч����
bool isBadCsvIndexCfg(const CsvIndexCfg *csv_index_cfg);

//=================================================
// Ԥ����poi����
//
// ===== poi���ݵ�Ԥ������� =====
// ���򣨶����������ȣ�ʶ��
// ��ʶ�𣨶��ţ����ŵȣ�
// ������ʶ��
// ����ʶ�𲢹淶
// ���������ʶ��
// ��ơ��ϲ���ʶ��
// ͬ����滻
// �ӵ�����ʶ��
// ��׺����ʶ��
// �ֵ�ʶ��
// �ȵ�
//
// ===== Ԥ����������Ҫ�м�����Ŀ��� =====
// 1. ������������Ԥ����Ȼ��������ƶȼ��㣬��Ч�������ظ��������ٶ��д������
// 2. ��̬���ɴʱ��idf���������Ը��ݴ���������ݵõ�����Ե������ʼ�������߾ۺϾ���
// 3. ��ȫ�����ݵĵ绰Ԥ�ȴ�����Ч���ٲ�ͬpoi�㹫��һ���绰�����ľۺϴ���
//=================================================
// ����record�ṹ��
void getRecFromCsv(const vector<string> &vec_data, Record &record,
		const CsvIndexCfg *csv_index_cfg);
void getRecFromCsv(const string &str_data, Record &record,
		const CsvIndexCfg *csv_index_cfg);

// ����Ԥ������
void makePreProcFromRec(const Record *record, PreProcData *pre_proc_data,
		const Resource *resource, const Tools *tools);
void makePreProcFromCsv(const string &str_data, PreProcData *pre_proc_data,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg);
void makePreProcFromCsv(const string &str_data, PreProcData *pre_proc_data,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg, Record *record);

// ���ļ�����Ԥ������
void _initMapFromCsvFileSgl(ifstream &fin, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg);
void _initMapFromCsvFileMulti(ifstream &fin, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg, pthread_mutex_t &prepare_read_mutex,
		pthread_mutex_t &prepare_setmap_mutex);
void initMapFromCsvFile(const char *path, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg);
//void initMapFromCsvFileDir(const char *data_dir, PreProcMap *pre_proc_map,
//		const Resource *resource, const Tools *tools,
//		const CsvIndexCfg *csv_index_cfg);

// ���ļ�����Ԥ���������������ݿ�
void _updateDBFromCsvFileSgl(ifstream &fin, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg);
void _updateDBFromCsvFileMulti(ifstream &fin, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg, pthread_mutex_t &prepare_read_mutex,
		pthread_mutex_t &prepare_setmap_mutex);
void updateDBFromCsvFile(const string &path, PreProcMap *pre_proc_map,
		const Resource *resource, const Tools *tools,
		const CsvIndexCfg *csv_index_cfg);

// �����ݿ�����Ԥ������
//void initMapFromDB();

//=================================================
// ͳ�ƺ���
//=================================================
// ͳ�������дʵ�idfֵ
void statIdf(const PreProcData &pre_proc_data,
		map<string, double> &stat_idf_map);
// ͳ�Ƶ绰�ĳ��ִ���
void statTel(const PreProcData &pre_proc_data, map<string, int> &stat_tel_map);

// ����idf
void makeIdf(unsigned long number, map<string, double> &stat_idf_map
		, double &max_idf, double &min_idf);
// ��ȡ��Ч�绰�б�
void makeUselessTel(map<string, int> &stat_tel_map, int threshold);

//=================================================
// �ܱ�������غ���
//
// �ܱ���������Ҫ����˵����
// 		���й��ĵ�ͼ����Ϊ��С��ȵĺܶ�����ӣ���������poi�㰴�����Ʒִʺ����꣬
// ӳ�䵽��ͬ�ĸ����С�����Ҫ���ܱ�����ʱ���Ȱ��չؼ��ʻ�ȡ�õ����ڵĸ����Լ��ܱ�
// ��8�����ӵ��������ݣ�Ȼ�����������룬�����ˣ����õ���Ҫ�����ݼ���
//
// �����зַ���˵����
// 		���������з�ʹ��bigram�ķ�ʽ�����ַ�ʽ�ٻ����б�֤�������ٻص��������ݺܶ�
// �Ҷ��ںܶ�ͬ�����εı��̴ֶʺ����ٻء��Ż�������ʹ�õķִʷ�ʽΪ���̴�����ʹ��bigram�з�
// ��ƴ��ͬʱ��������������������ķִ�+ƴ���ķ�ʽ
//=================================================
int getLatKey(double lat);
int getLngKey(double lng);

void setGridMap(const GridKey &key, const unsigned long long value,
		GridMap &grid_map);
void setGridMap(const PreProcData *pre_proc_data, const Resource *resource,
		GridMap *grid_map);

void initGridMap(const PreProcMap *pre_proc_map, const Resource *resource,
		GridMap *grid_map);

void getAroundGridMap(const PreProcData &pre_proc_data, const GridKey &key, const GridMap &grid_map,
		set<unsigned long long> &result_set, int level);
void getAroundGridMap(const PreProcData &pre_proc_data, const GridMap &grid_map,
		set<unsigned long long> &result_set, int level);

#endif /* PREPARE_H_ */
