/*
 * init.h
 *
 *  Created on: 2011-10-10
 *      Author: stefanyang
 *
 *  ����ۺ���Ҫ�Ĵʱ�ȼ��صĻ�������
 */

#ifndef INIT_H_
#define INIT_H_

#include <string>
#include <set>
#include <vector>
#include <map>
#include <sys/types.h>
#include <dirent.h>

#include "typeDefine.h"

// �жϸ�����·���Ƿ���һ���ļ���
bool judge_dir(const char *path);

// ��һ���ļ����µ��ļ����ݶ�load���ڴ���
// �ļ�������Ϊmap��key
// �ļ�����Ϊÿ��һ��valueֵ�����valueֵ���ŵ�һ��set��
int load(const std::string &dirpath, _mapSet &result);

//==============================================================================
// ���ı��е�����load��map�ṹ��
// ����˵����
// path: �ļ�·��
// result: ���map
// delim: �ָ��ַ���
// rever: Ĭ�Ϸָ�������ݵ�һ����key��������value��reverΪtrue���һ��Ϊalue������Ϊkey
// clear: �Ƿ����result��Ϊtrue�������
// �ļ���ʽ˵����
// һ��Ϊһ����¼��ÿ�а���һ��key��һ��alue�������м���delim�ָ�磺"key\tvalue\n"
int load(const std::string &path, _map &result, const std::string &delim = " ",
		bool rever = false, bool clear = true);

//==============================================================================
// ���ı��е�����load��ָ���ṹ��
// ����˵����
// path���ļ�·��
// clear: �Ƿ����result��Ϊtrue�������
int load(const std::string &path, _hashset &result, bool clear = true);
int load(const std::string &path, _vector &result, bool clear = true);

//==============================================================================
// ���ı��е�����load��ָ���ṹ��
// ����˵����
// path: �ļ�·��
// result: ���map, һ��key��Ӧһ��vector����set����һ��double���͵�����
// delim: �ָ��ַ���
// rever: Ĭ�Ϸָ�������ݵ�һ����key��������value��reverΪtrue���һ��Ϊalue������Ϊkey
// clear: �Ƿ����result��Ϊtrue�������
// �ļ���ʽ˵����
// һ��Ϊһ����¼��ÿ�а���һ��key��һ��alue�������м���delim�ָ�磺"key\tvalue\n"
int load(const std::string &path, _mapVec& result, const std::string &delim =
		" ", bool rever = false, bool clear = true);
int loadTypeMap(const std::string &path, _mapSet& result,
		const std::string &delim = " ", bool rever = false, bool clear = true);
int load(const std::string &path, _mapStrDbl& result, const std::string &delim,
		bool rever = false, bool clear = true);

//==============================================================================
// һЩ�Ƚ��ر�Ĵʱ�ļ��غ���
///////////////////////////////////////////////////////////////
// ��Ҫ����ͬ��ʱ�ļ���
// ��������֮ǰ�ĺ���
// ͬ��ʱ��������ʱ�ͬ�����ڣ�ÿ����һ��key�������ж��value
int loadTrans(const std::string &path, _map &result, const std::string &delim =
		" ", bool rever = false, bool clear = true);

///////////////////////////////////////////////////////////////
// ��Ҫ���ڴ�������ʱ��м���ÿ���������Ӧ�����ͱ���
int loadObjType(const std::string &path, _map &result, const std::string &delim,
		bool clear = true);

///////////////////////////////////////////////////////////////
// ��Ҫ���ڼ�ơ��ϲ����ȵ�ʶ����滻�Ĵʱ����
int load(const std::string &path, _mapStrIntSet &result, const std::string &delim,
		bool clear = true);

//==============================================================================
// ����Ӧ��ֻ����һ�������ļ��ĺ�����װΪ���Դ����������ļ��ĺ���
int load(_vector &path, _map &result, const std::string &delim = " ",
		bool rever = false);
int load(_vector &path, _hashset &result);
int load(_vector &path, _vector &result);
int load(_vector &path, _mapVec& result, const std::string &delim = " ",
		bool rever = false);
int load(_vector &path, _mapStrDbl& result, const std::string &delim = " ",
		bool rever = false);
int loadTypeMap(_vector &path, _mapSet& result, const std::string &delim = " ");

int loadTrans(_vector &path, _map &result, const std::string &delim = " ",
		bool rever = false);
int loadObjType(_vector &path, _map &result, const std::string &delim);
int load(_vector &path, _mapStrIntSet &result, const std::string &delim);

#endif
