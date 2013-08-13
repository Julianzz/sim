
#include "string_process.h"
#include<iostream>
#include<fstream>
#include<map>
#include<vector>
using namespace std;
// ȫ��ת���
void FullWidthToHalfWidth(string &str)
{
		int flag = 1;
		string::size_type len = str.size();
		string tstr;
		unsigned char chigh,clow;

		for(unsigned int j = 0; j < len; ++j)
		{
			chigh = str[j];
															
			if(chigh < 128)						// ����ַ�
			{
				tstr += str[j];
				continue;
			}
			clow  = str[j+1];
			if(chigh == 161 && clow == 161)		// ȫ�ǿո�
			{
				tstr += " ";
				++j;
				flag = 0;
				continue;
			}
			if(chigh == 163)					// ȫ���ַ�������ȫ�ǣ���Ӧ����Ӣ��$��
			{
				tstr += str[j+1] - 128;
				++j;
				flag = 0;
				continue;
			}
			if(chigh > 128)						// ����
			{
				tstr += str[j];	
				tstr += str[j+1];
				++j;
			}
		}
		if(flag == 0)
			str = tstr;
}

void LowerWord(string &str)
{
	for(string::size_type i = 0; i < str.size(); ++i)
	{	
		if(str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
	}
}

void GetCateCode1(const char *filename,map<string,string> &cate_code)
{
		ifstream ifile(filename);
		
		string record,code,cate;
		string::size_type pos;
		
		while(getline(ifile,record))
		{
				pos = record.find("\t");
				code = record.substr(0,pos);
				cate = record.substr(pos+1,record.size()-pos-1);
				cate_code.insert(make_pair(cate,code));
		}

		ifile.close();
}
		
void GetCateCode2(const char *filename,map<string,string> &code_cate,map<string,string> &cate_code)
{
	ifstream ifile(filename);

	string record,code,cate;
	string::size_type pos;
	while(getline(ifile,record))
	{
		pos = record.find("\t");
		code = record.substr(0,pos);
		cate = record.substr(pos+1,record.size()-pos-1);
		code_cate.insert(make_pair(code,cate));
		cate_code.insert(make_pair(cate,code));
	}

	ifile.close();
}

void ReadMapFile(const char *filename,vector<string> &keys,map<string,string> &key_category)
{
	ifstream keyfile(filename);
	string str,key,category;
	string::size_type pos;
	
	while(getline(keyfile,str))
	{
		pos = str.find("\t");
		key = str.substr(0,pos);
		category = str.substr(pos+1,str.size()-pos-1);
		keys.push_back(key);
		key_category.insert(make_pair(key,category));
	}
	keyfile.close();
}

