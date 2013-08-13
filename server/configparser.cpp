/*
 * IndexDataPross.cpp
 *
 *  Created on: 2011-11-10
 *      Author: frankmeng
 */
#include "configparser.h"

bool parse_config(const string& file_name, CConfigParser&config){
    
    if (!config.ParseConfigFile(file_name.c_str())){
        std::cout<<"configure file parse error"<<std::endl;
        return false;
    }
    return true;
}

bool parse_server_config(CConfigParser&config,
        string&server_host,
        double& timeout,
        int& workers,
        string& storehost,
        string& slavehost,
        string& logfile,int& loglevel){
            
    int ret = 0;
    ret = config.GetItem("server_host",server_host);
    if(!ret){
        std::cout<<"server_host,config error"<<std::endl;
        return false;
    }

    ret = config.GetItem("timeout",timeout,1.0);
    if(!ret){
        return false;
    }
    ret = config.GetItem("workers",workers,10);
    if(!ret){
        return false;
    }
    ret = config.GetItem("logfile",logfile,"log.txt");
    if(!ret){
        return false;
    }
    ret = config.GetItem("loglevel",loglevel,1);
    if(!ret){
        return false;
    }
    return true;

}
/////////////////////////////////////////////////
///CConfigParser
/////////////////////////////////////////////////
bool CConfigParser::ParseConfigFile(const char* pszFile)
{
	ifstream fin;
	//locale::global(locale(""));
	fin.open(pszFile);
	//locale::global(locale("C"));
	if(fin==NULL)
	{
		printf("Can't Open Config File: %s\n", pszFile);
		return false;
	}
	string strLine;
	while(getline(fin, strLine, '\n'))
	{
		StringUtility::TrimBothSides(strLine);
		string s_key,s_value;
		if(IsItem(strLine.c_str(),s_key,s_value))
		{
			m_configSection[s_key] = s_value;
		}
	}
	fin.close();
	return true;
}

bool CConfigParser::GetSeriousItems(const char* pszItemKey, vector<string>& vItemVals)
{
	string pszItemVal;
	if( !GetItem(pszItemKey, pszItemVal) )
	{
		return false;
	}
	StringUtility::Split(pszItemVal, "\t", vItemVals, true);
	//vItemVals = StringUtility::Split(pszItemVal, "|");
	return true;
}

bool CConfigParser::GetItem(const char* pszItemKey, string& pszItemVal)
{
	if(m_configSection.find(string(pszItemKey)) == m_configSection.end())
	{
		printf("Not Find Key: %s\n", pszItemKey);
		return false;
	}
	pszItemVal = m_configSection[pszItemKey];
	return true;
}

bool CConfigParser::GetItem(const char* pszItemKey, char* pszItemVal)
{
	if(m_configSection.find(string(pszItemKey)) == m_configSection.end())
	{
		printf("Not Find Key: %s\n", pszItemKey);
		return false;
	}
	strcpy(pszItemVal,m_configSection[pszItemKey].c_str());
	return true;
}

bool CConfigParser::GetItem(const char* pszItemKey, unsigned int& u32ItemVal)
{
	if(m_configSection.find(string(pszItemKey)) == m_configSection.end())
	{
		u32ItemVal = 0;
		printf("Not Find Key: %s\n", pszItemKey);
		return false;
	}
	sscanf(m_configSection[pszItemKey].c_str(),"%u",&u32ItemVal);
	return true;
}

bool CConfigParser::GetItem(const char* pszItemKey, char* pszItemVal,const char*defaultValue){
    if (!this->GetItem(pszItemKey,pszItemVal)){
        strcpy(pszItemVal,defaultValue);
    }
    return true;
}
bool CConfigParser::GetItem(const char* pszItemKey, string& pszItemVal,const string&defaultValue ){
    if (!this->GetItem(pszItemKey,pszItemVal)){
        pszItemVal.assign(defaultValue);
    }
    return true;
}
bool CConfigParser::GetItem(const char* pszItemKey, unsigned int& u32ItemVal,const unsigned int& defaultValue){
    if (!this->GetItem(pszItemKey,u32ItemVal)){
        u32ItemVal = defaultValue;
    }
    return true;
}
bool CConfigParser::GetItem(const char* pszItemKey, int& i32ItemVal,const int defaultValue){
    if (!this->GetItem(pszItemKey,i32ItemVal)){
        i32ItemVal = defaultValue;
    }
    return true;
}
bool CConfigParser::GetItem(const char* pszItemKey, float& fItemVal,const float defaultValue){
    if (!this->GetItem(pszItemKey,fItemVal)){
        fItemVal = defaultValue;
    }
    return true;
}
bool CConfigParser::GetItem(const char* pszItemKey, double& dItemVal,const float defaultValue){
    if (!this->GetItem(pszItemKey,dItemVal)){
        dItemVal = defaultValue;
    }
    return true;
}

bool CConfigParser::GetItem(const char* pszItemKey, int& i32ItemVal)
{
	if(m_configSection.find(string(pszItemKey)) == m_configSection.end())
	{
		i32ItemVal = -1;
		printf("Not Find Key: %s\n", pszItemKey);
		return false;
	}
	sscanf(m_configSection[pszItemKey].c_str(),"%d",&i32ItemVal);
	return true;
	return true;
}

bool CConfigParser::GetItem(const char* pszItemKey, float& fItemVal)
{
	if(m_configSection.find(string(pszItemKey)) == m_configSection.end())
	{
		fItemVal = -1;
		printf("Not Find Key: %s\n", pszItemKey);
		return false;
	}
	sscanf(m_configSection[pszItemKey].c_str(),"%f",&fItemVal);
	return true;
}

bool CConfigParser::GetItem(const char* pszItemKey, double& dItemVal)
{
	if(m_configSection.find(string(pszItemKey)) == m_configSection.end())
	{
		dItemVal = -1;
		printf("Not Find Key: %s\n", pszItemKey);
		return false;
	}
	sscanf(m_configSection[pszItemKey].c_str(),"%lf",&dItemVal);
	return true;
}

bool CConfigParser::GetItems(vector<string>& vItems) //const char* pszItemKey,
{
	StringMap::iterator ite;
	if(m_configSection.size()==0)
	{
		vItems.resize(0);
		return false;
	}
	for (ite=m_configSection.begin();ite!=m_configSection.end();++ite)
	{
		vItems.push_back(ite->first);
	}
	return true;
}

bool CConfigParser::IsItem(const char* pszLine, string& s_key, string& s_value)
{
	if(pszLine[0] == ';'||(pszLine[0] == '[' && pszLine[strlen(pszLine)-1] == ']')){
		return false;
	}
	s_key = "";
	s_value = "";
	int keyEnd = 0;
	for(unsigned int i=0; i<strlen(pszLine); ++i)
	{
		if(pszLine[i] == ' ' || pszLine[i] == '\t' ||pszLine[i] == '\n')continue;
		if(pszLine[i] == '=')
		{
			if( strlen(pszLine)>=c_MaxLenItem || strlen(pszLine)<=c_MinLenItem || pszLine[strlen(pszLine)-1]=='=' ) //no value
			{
				printf("Item Len Invalid: %s\n", pszLine);
				return false;
			}
			keyEnd = 1;
		}
		else if (keyEnd)
		{
			s_value += pszLine[i];
		}
		else
		{
			s_key += pszLine[i];
		}
	}
	if (s_key!=""&&s_value!="")
	{
		return true;
	}
	return false;
}

