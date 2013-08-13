/*
 * IndexDataPross.cpp
 *
 *  Created on: 2011-11-10
 *      Author: frankmeng
 */

#ifndef __CONFIGPARSER_H__
#define __CONFIGPARSER_H__
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdio.h>
     
#include "StringUtility.h"
using namespace std;

typedef std::map<string,string> StringMap;

const unsigned int c_MaxLenSection = 256;
const unsigned int c_MinLenSection = 3;
const unsigned int c_MaxLenItem = 512;
const unsigned int c_MinLenItem = 2; //3

class CConfigParser
{
public:
	CConfigParser() {};
	virtual ~CConfigParser(){
		clear();
	};
	bool ParseConfigFile(const char* pszFile);  //获取配置文件中的信息,形式如 s_Key=s_Value
	bool GetSeriousItems(const char* pszItemKey, vector<string>& vItemVals);
	bool GetItem(const char* pszItemKey, char* pszItemVal);
	bool GetItem(const char* pszItemKey, string& pszItemVal);
	bool GetItem(const char* pszItemKey, unsigned int& u32ItemVal);
	bool GetItem(const char* pszItemKey, int& i32ItemVal);
	bool GetItem(const char* pszItemKey, float& fItemVal);
	bool GetItem(const char* pszItemKey, double& dItemVal);

    bool GetItem(const char* pszItemKey, char* pszItemVal,const char*defaultValue);
    bool GetItem(const char* pszItemKey, string& pszItemVal,const string&defaultValue );
    bool GetItem(const char* pszItemKey, unsigned int& u32ItemVal,const unsigned int& defaultValue);
    bool GetItem(const char* pszItemKey, int& i32ItemVal,const int defaultValue);
    bool GetItem(const char* pszItemKey, float& fItemVal,const float defaultValue);
    bool GetItem(const char* pszItemKey, double& dItemVal,const float defaultValue);

	bool GetItems(vector<string>& vItems); //const char* pszItemKey, 

	void DumpInfo(){
	    std::cout<<"++++++++configure informations++++++"<<std::endl;
	    StringMap::const_iterator iter = m_configSection.begin();
	    for(;iter!=m_configSection.end();iter++){
	        std::cout<<iter->first<<" : " <<iter->second<<std::endl;
	    }
	    std::cout<<"++++++++end configure file++++++"<<std::endl;
	}
	void clear()
	{
		m_configSection.clear();
	}

private:
	bool IsItem(const char* pszLine, string& s_key, string& s_value); //判断是否为配置信息

private:
	StringMap m_configSection; //配置文件信息对应关系
};

bool parse_config(const string& file_name,CConfigParser&config);
bool parse_server_config(CConfigParser&config,
        string&server_host,double& timeout,
        int& workers,
        string& storehost,string& slavehost,
        string&logfile,int& loglevel);

#endif
