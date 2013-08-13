/*
 * init.h
 *
 *  Created on: 2011-10-10
 *      Author: stefanyang
 *
 *  定义聚合需要的词表等加载的基础函数
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

// 判断给定的路径是否是一个文件夹
bool judge_dir(const char *path);

// 将一个文件夹下的文件内容都load到内存中
// 文件名将作为map的key
// 文件内容为每行一个value值，多个value值会存放到一个set中
int load(const std::string &dirpath, _mapSet &result);

//==============================================================================
// 将文本中的内容load到map结构中
// 参数说明：
// path: 文件路径
// result: 结果map
// delim: 分割字符串
// rever: 默认分割完的数据第一个是key，接着是value，rever为true则第一个为alue，接着为key
// clear: 是否清空result，为true，则清空
// 文件格式说明：
// 一行为一条记录，每行包含一个key和一个alue，并且中间以delim分割，如："key\tvalue\n"
int load(const std::string &path, _map &result, const std::string &delim = " ",
		bool rever = false, bool clear = true);

//==============================================================================
// 将文本中的内容load到指定结构中
// 参数说明：
// path：文件路径
// clear: 是否清空result，为true，则清空
int load(const std::string &path, _hashset &result, bool clear = true);
int load(const std::string &path, _vector &result, bool clear = true);

//==============================================================================
// 将文本中的内容load到指定结构中
// 参数说明：
// path: 文件路径
// result: 结果map, 一个key对应一个vector，或set，或一个double类型的数据
// delim: 分割字符串
// rever: 默认分割完的数据第一个是key，接着是value，rever为true则第一个为alue，接着为key
// clear: 是否清空result，为true，则清空
// 文件格式说明：
// 一行为一条记录，每行包含一个key和一个alue，并且中间以delim分割，如："key\tvalue\n"
int load(const std::string &path, _mapVec& result, const std::string &delim =
		" ", bool rever = false, bool clear = true);
int loadTypeMap(const std::string &path, _mapSet& result,
		const std::string &delim = " ", bool rever = false, bool clear = true);
int load(const std::string &path, _mapStrDbl& result, const std::string &delim,
		bool rever = false, bool clear = true);

//==============================================================================
// 一些比较特别的词表的加载函数
///////////////////////////////////////////////////////////////
// 主要用于同义词表的加载
// 参数类似之前的函数
// 同义词表与其他词表不同的在于，每行有一个key，但是有多个value
int loadTrans(const std::string &path, _map &result, const std::string &delim =
		" ", bool rever = false, bool clear = true);

///////////////////////////////////////////////////////////////
// 主要用于从连锁店词表中加载每个连锁店对应的类型编码
int loadObjType(const std::string &path, _map &result, const std::string &delim,
		bool clear = true);

///////////////////////////////////////////////////////////////
// 主要用于简称、合并名等的识别和替换的词表加载
int load(const std::string &path, _mapStrIntSet &result, const std::string &delim,
		bool clear = true);

//==============================================================================
// 将对应的只处理一个输入文件的函数封装为可以处理多个输入文件的函数
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
