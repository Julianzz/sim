#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include"string_process.h"
#include"classify_data_defs.h"
#include"classify_hospital_defs.h"
using namespace std;

vector<vector<string> > hospital_keys;
vector<map<string,string> > hospital_category;

bool ProcessLastWord(const string &record);

void GetHospitalKeyCategory(const string &filepath)
{
	string files[HOSPITALCATENUM] = {"key_hospital"};
	vector<string> keys;
	map<string,string> key_category;
	
	for(int i = 0; i < HOSPITALCATENUM; ++i)
	{
		ReadMapFile((filepath+files[i]).c_str(),keys,key_category);
		hospital_keys.push_back(keys);
		hospital_category.push_back(key_category);
		keys.clear();
		key_category.clear();
	}
}

bool EndWithDoor(const string &record)
{
	string::size_type rlen = record.size();
	
	if(record.substr(rlen-2,2).compare("门") == 0)
		return true;

	return false;
}

bool IsHospitalBranch(const string &record,string &category,string &last_key,const string hospital[],int num,                                     vector<string> &v_keys)
{
	string::size_type pos1 = 0;
	string::size_type rlen = record.size();

	for(int i = 0; i < num; ++i)
	{
		if(last_key.compare(hospital[i]) == 0)
		{
			//**医院**分院
			for(int j = 0; j < BRANCH; ++j)
			{
				if((pos1 = record.rfind(branch[j])) != string::npos)
				{
					if(EndWithDoor(record) == true)
						return true;
					if(pos1 == record.size() - (branch[j]).size())
						return true;
					if(ProcessLastWord(record) == true)
						return true;
					//category = "其它";
					return true;
				}
			}
			return false;  // 不能返回true,返回后还需要判断是否是附属医院
		}
	}

	return false;
}

bool IsAffilyHospital(const string &record,const string &last_key,string &category)
{
	if(last_key.compare("医院") == 0)
	{
		string::size_type pos1 = record.rfind(last_key);
		string::size_type pos2 = record.find(last_key);
			
		if(pos2 != string::npos && pos2 < pos1)
		{
			if(pos1 != record.size() - 4)
				if(ProcessLastWord(record) == false)
					category = "其它";
			
			return true;
		}		
	}

	return false;
}

bool IsDepartment(const string &record,string &last_key,vector<string> &v_keys)
{
	string::size_type pos1 = 0,pos2 = 0;
	const int OTHERSHINUM = 2;
	string other_shi[2] = {"院长室","办公室"};

	for(int i = 0; i < GENERALNUM; ++i)
	{
		if(last_key.compare(general_hospital[i]) == 0 || last_key.substr(last_key.size()-2,2).compare("院") == 0 
				|| last_key.substr(last_key.size()-2,2).compare("医") == 0)
		{
			pos1 = record.rfind(last_key);
			if(record.substr(record.size()-2,2).compare("科") == 0)
			{
				last_key = record.substr(pos1+last_key.size(),record.size() - pos1 - last_key.size());
				v_keys.push_back(last_key);
				return true;
			}

			if(last_key.substr(last_key.size()-2,2).compare("室") != 0)
			{
				if(record.substr(record.size()-2,2).compare("室") == 0)
				{
					for(int j = 0; j < OTHERSHINUM; ++j)
						if(record.size() >= 6 && record.substr(record.size()-6,6).compare(other_shi[j]) == 0)
							return false;
					last_key = record.substr(pos1+last_key.size(),record.size() - pos1 - last_key.size());
					v_keys.push_back(last_key);
					return true;
				}
			}

			if(record.substr(record.size()-4,4).compare("中心") == 0)
			{
				if(last_key.compare("医疗中心") == 0 || last_key.compare("老年健康服务中心") == 0                                                      || last_key.compare("保健中心") == 0 )
					return false;
				last_key = record.substr(pos1+last_key.size(),record.size() - pos1 - last_key.size());
				v_keys.push_back(last_key);
				return true;
			}
			
			if(record.size() >= 4)
			{
				string str = record.substr(record.size()-4,4);
				if(str.compare("急诊") == 0)
					return true;
			}

			return false;
		}
	}

	return false;
}

bool IsCommunityHealthServices(const string &record,string &last_key,vector<string> &v_keys)
{
	for(int i = 0; i < COMMUNITYHEALTHNUM; ++i)
	{
		if(last_key.find(community_health[i]) != string::npos)
			return true;
	}

	if(record.find("社区医疗中心") != string::npos || record.find("医院社区") != string::npos                                      || record.find("社区健康中心") != string::npos || record.find("社区康复保健中心") != string::npos)
		return true;
	
	return false;
}

bool IsSpecialistHospital(const string &record)
{	
	for(int i = 0; i < SPECNUM; ++i)
	{
		if(record.find(spec_hospital[i]) != string::npos)
			return true;
	}

	return false;
}

bool ProcessLastWord(const string &record)
{
	string::size_type rlen = record.size();
	string last_word = record.substr(rlen-2,2);
	
	if(last_word.compare("站") == 0 || last_word.compare("部") == 0 || last_word.compare("室") == 0 
	   || last_word.compare("区") == 0)
	{
		string forword_word = record.substr(rlen-4,2);

		for(int i = 0; i < CSCHOOLNO; ++i)
		{
			if(forword_word.compare(school_number_c[i]) == 0)
				return true;
		}

		for(int j = 0; j < ASCHOOLNO; ++j)
		{
			if(forword_word.compare(school_number_a[j]) == 0)
				return true;
		}
	}

	last_word = record.substr(rlen-4,4);
	for(int i = 0; i < AREANUM; ++i)
	{
		if(last_word.compare(area[i]) == 0)
			return true;
	}
	
	
	return false;
}

void ProcessSpecial(const string record,string &key)
{
	if(key.compare("卫生服务") == 0)
	{
		string::size_type pos = record.rfind(key);
		string::size_type len;
		for(int j = 0; j < HEALTHSERVICE; ++j)
		{
			len = (health[j]).size();
			if(record.substr(pos+8,len).compare(health[j]) == 0/* && (pos+8+len == record.size())*/)
				key = record.substr(pos,8+len);
		}
	}
}

bool IsAbbreDepartment(const string &record,string &category)
{
	string::size_type pos = 0;
	for(int i = 0; i < DEPARTMENTNUM; ++i)
	{
		if((pos = record.find(department[i])) != string::npos)
		{
			if(pos == 0)
				category = "医疗保健:综合医院:科室";
			else
				category = "医疗保健:专科医院:其它专科医院";

			return true;
		}
	}

	//住院几部
	string::size_type pos1 = 0,pos2 = 0;
	while((pos2 = record.find("住院",pos1)) != string::npos)
	{
		if(pos2 + 8 > record.size())
			break;
		if(record.substr(pos1+6,2).compare("部") == 0)
		{
			for(int i = 0; i < CSCHOOLNO; ++i)
			{
				if(record.substr(pos2+4,2).compare(school_number_c[i]) == 0)
				{
					category = "医疗保健:综合医院:科室";
					return true;
				}
			}
		}
		pos1 = pos2 + 4;
	}

	return false;
}

bool IsAbbreviation(string &record,string &akey)
{
	string::size_type pos1 = 0,pos2 = 0;
	string keys1[2] = {"院","医"};
	string keys2[6] = {"中","附","东","西","南","北"};
	
	for(int i  = 0; i < 2; ++i)
	{
		while((pos2 = record.find(keys1[i],pos1)) != string::npos)
		{
			if(pos2 <= 1)
			{
				pos1 = pos2 + 2;
				continue;
			}
			//几院
			for(int j = 0; j < CSCHOOLNO; ++j)
			{
				if(record.substr(pos2-2,2).compare(school_number_c[j]) == 0)
				{
					akey = record.substr(pos2-2,4);
					return true;
				}
			}
			//*中院,*附院,*东院
			if(i == 0)
			{
				for(int j = 0; j < 6; ++j)
				{
					if(record.substr(pos2-2,2).compare(keys2[j]) == 0)
					{
						akey = record.substr(pos2-2,4);
						return true;
					}
				}
			}
			pos1 = pos2+2;
		}
	}
	
	pos1 = 0;
	pos2 = 0;
	//卫生*室(所),卫生分室
	while((pos2 = record.find("卫生",pos1)) != string::npos)
	{
		if(pos2 + 8 > record.size())
			break;
		if(record.substr(pos2+6,2).compare("室") == 0 || record.substr(pos2+6,2).compare("所") == 0)
		{
			for(int i = 0; i < CSCHOOLNO; ++i)
			{
				if(record.substr(pos2+4,2).compare(school_number_c[i]) == 0)
				{
					akey = record.substr(pos2,8);
					return true;
				}
			}
			
			if(record.substr(pos2+4,2).compare("分") == 0)
			{
				akey = record.substr(pos2,8);
				return true;
			}
		}
		pos1 = pos2 + 4;
		if(pos1 > record.size())
			break;
	}
	
	//医院部门
	for(int i = 0; i < HOSKEY; ++i)
	{
		if(record.find(hos_key[i]) != string::npos)
			return true;
	}

	for(int i = 0; i < ABBREAREANUM; ++i)
	{
		//**楼,**病房,**病区
		string::size_type len1 = record.size();
		string::size_type len2 = (abbre_area[i]).size();
		if(len1 >= len2)
			if(record.substr(len1-len2,len2).compare(abbre_area[i]) == 0)
				return true;
	}

	//括号里面出现医院
	if(record.find("医院") != string::npos)
		return true;

	return false;
}

bool IsAbbreSpecialist(const string &record)
{
	//专科医院
	for(int i = 0; i < ABBRESPECNUM; ++i)
	{
		if(record.find(abbre_spec_hospital[i]) != string::npos)
			return true;
	}

	return false;
}

bool IsAbbreHospital(const string &record,string &last_key)
{
	//可能是医院
	for(int i = 0; i < ABBREHOSNUM; ++i)
	{
		if(record.find(abbre_hospital[i]) != string::npos)
		{
			last_key = abbre_hospital[i];
			return true;
		}
	}

	if(record.size() >= 4)
	{
		if(record.substr(record.size()-4,4).compare("卫生") == 0)
		{
			last_key = "卫生";
			return true;
		}
	}

	return false;
}

bool IsDissPreve(const string &record)
{
	for(int i = 0; i < DISSPREVENUM; ++i)
	{
		if(record.find(diss_preve[i]) != string::npos)
			return true;
	}

	return false;
}

bool IsPharmacy(const string &record,const string &category)
{
	int k = 0;
	if(category.compare("医疗保健:药房药店") != 0)
		k = 3;
	
	for(int i = k; i < PHARMACYNUM; ++i)
	{
		if(record.size() >= (pharmacy[i]).size() && record.substr(record.size() - (pharmacy[i]).size(),(pharmacy[i]).size()).compare(pharmacy[i]) == 0)
		{
			if((pharmacy[i]).compare("医药") == 0 && record.find("医药大学") == string::npos)
				return true;
			if((pharmacy[i]).compare("医药") != 0)
				return true;
		}
	}

	string key1[4] = {"药","药业","药品","保健品"};
	for(int i = 0; i < 4; ++i)
	{
		if(record.size() >= (key1[i]).size())
			if(record.substr(record.size()-(key1[i]).size(),(key1[i]).size()).compare(key1[i]) == 0)
				return true;
	}

	string key[5] = {"店","路","道","街","超市"};
	if(record.find("药") != string::npos)
	{
		for(int i = 0; i < 4; ++i)
		{
			if(record.size() >= (key[i]).size())
				if(record.substr(record.size()-(key[i]).size(),(key[i]).size()).compare(key[i]) == 0)
					return true;
		}
	}
	
	return false;
}

bool IsEmergencyCenter(const string &record)
{
	for(int i = 0; i <  EMERGENCYNUM; ++i)
	{
		if(record.find(emergency[i]) != string::npos)
			return true;
	}

	return false;
}

bool IsReproduct(const string &record,const string &category)
{
	int k = 0;
	if(category.compare("医疗保健:药房药店") == 0)
		k = 1;
	
	for(int i = k; i < REPRODUCTNUM; ++i)
	{
		if(record.find(reproduct[i]) != string::npos)
			return true;
	}

	return false;
}

bool IsGeneralHospital(const string &trecord,string &category,multimap<string::size_type,string> &m_keys,string &last_key,                                                                                                   const string &raw_record)
{
	string record = trecord;
	int i = 0;
	string akey = "";
	bool flag = false;

/*
	for(int i = 0; i < CLINICNUM; ++i)
		if(record.size() >= (clinic[i]).size() && record.substr(record.size()-(clinic[i]).size(),(clinic[i]).size()).compare(clinic[i]) == 0)
			return false;
*/	
	//药房药店
	if(IsPharmacy(record,category) == true)
	{
		category = "医疗保健:药房药店";
		return true;
	}
	
	//急救
	if(IsEmergencyCenter(record))
	{
		category = "医疗保健:急救中心";
		return true;
	}
	
	//生殖,计划生育,计生
	if(IsReproduct(record,category) == true)
	{
		category = "医疗保健:专科医院:其它专科医院";
		return true;
	}
	
	//疾病预防
	if(IsDissPreve(record) == true)
	{
		category = "医疗保健:疾病预防";
		return true;
	}

	//专科医院
	if(category.find("医疗保健:专科医院") == string::npos && (IsAbbreSpecialist(record) == true || IsSpecialistHospital(record) == true))
	{
		category = "医疗保健:专科医院:其它专科医院";
		return true;
	}

	if(category.find("医疗保健:专科医院") != string::npos)
	{
		if(record.find("儿科医院") != string::npos || record.find("儿科院") != string::npos)
		{
			category = "医疗保健:专科医院:儿科";
			return true;
		}
		if(record.find("产科医院") != string::npos || record.find("产院") != string::npos)
		{
			category = "医疗保健:专科医院:妇产科";
			return true;
		}
		if(record.find("脑科医院") != string::npos || record.find("脑科院") != string::npos)
		{
			category = "医疗保健:专科医院:脑科";
			return true;
		}
		if(record.find("肿瘤医院") != string::npos || record.find("肿瘤院") != string::npos)
		{
			category = "医疗保健:专科医院:肿瘤";
			return true;
		}
		if(record.find("骨科医院") != string::npos || record.find("骨科院") != string::npos)
		{
			category = "医疗保健:专科医院:骨科";
			return true;
		}
		if(record.find("胸科医院") != string::npos || record.find("胸科院") != string::npos)
		{
			category = "医疗保健:专科医院:胸科";
			return true;
		}
		if(record.find("耳鼻喉医院") != string::npos || record.find("耳鼻喉院") != string::npos)
		{
			category = "医疗保健:专科医院:耳鼻喉";
			return true;
		}
		if(record.find("眼科医院") != string::npos || record.find("眼科院") != string::npos)
		{
			category = "医疗保健:专科医院:眼科";
			return true;
		}
		if(record.find("整形医院") != string::npos || record.find("整形院") != string::npos)
		{
			category = "医疗保健:专科医院:整形";
			return true;
		}
		if(record.find("传染病医院") != string::npos || record.find("传染病院") != string::npos)
		{
			category = "医疗保健:专科医院:传染病医院";
			return true;
		}
		if(record.find("精神病医院") != string::npos || record.find("精神病院") != string::npos)
		{
			category = "医疗保健:专科医院:精神病医院";
			return true;
		}
		if(category.compare("医疗保健:专科医院:其它专科医院") == 0 && IsSpecialistHospital(record) == true)
		{
			category = "医疗保健:专科医院:其它专科医院";
			return true;
		}

		return false;
	}
	
	for(i = 0; i < GENERALNUM; ++i)
	{
		if(record.find(general_hospital[i]) != string::npos)
			break;
	}
/*
	if(i >= GENERALNUM)
	{
		//括号里面出现医院
		string::size_type pos1 = raw_record.rfind("(");
		if(pos1 != string::npos)
		{
			if(raw_record.find("医院",pos1) != string::npos)
			{
				record = "";
				for(int i = 0; i < raw_record.size(); ++i)
				{
					if(raw_record[i] != '(' && raw_record[i] != ')')
						record += raw_record[i];
				}
			}
		}
	}
*/	
	//一院(几院),住院部之类的
	if(IsAbbreviation(record,akey) == true)
		flag = true;
	
	//不包含医院类的关键词
	if((flag == false) && i >= GENERALNUM)
	{
		//药房药店
		//if(IsPharmacy(record,category) == true)
		//{
		//	category = "医疗保健:药房药店";
		//	return true;
		//}
		
		//**科(要区分专科和医院科室)
		//if(IsAbbreDepartment(record,category) == true)
		//	return true;
		
		if(category.compare("医疗保健:诊所") == 0)
			return false;
		
		if(category.find("医疗保健:综合医院") == string::npos)
			return false;
		
		//**中心
		if((record.size() >= 4) && (record.substr(record.size()-4,4).compare("中心") == 0))
		{
			int j = 0;
			for(j = 0; j < CENTERNUM; ++j)
			{
				if(record.find(center[j]) != string::npos)
					break;
			}
			
			if( j == CENTERNUM)
			{
				category = "医疗保健:综合医院:科室";
				return true;
			}
		}

		//**室,**部
		string str = "";
		if(record.size() > 2)
			str = record.substr(record.size()-2,2);
		if(str.compare("室") == 0 || str.compare("部") == 0)
		{
			if(record.find("办公室") == string::npos && record.find("院长室") == string::npos)
			{
				category = "医疗保健:综合医院:科室";
				return true;
			}
		}	
		
		//可能是医院
		if(IsAbbreHospital(record,last_key) == true && (last_key.find("门诊") != string::npos || last_key.find("急诊") != string::npos))
		{
			category = "医疗保健:综合医院:科室";	
			return true;
		}

		return false;
	}

	string::size_type pos1 = 0,pos2 = 0;
	vector<string>::iterator viter = (hospital_keys[GENERAL]).begin();
	while(viter != (hospital_keys[GENERAL]).end())
	{
		pos1 = 0,pos2 = 0;
		while((pos2 = record.find(*viter,pos1)) != string::npos)
		{	
			if((*viter).compare("楼") == 0 && pos2 != record.size()-2)
			{
				pos1 = pos2 + 2;
				continue;
			}
			m_keys.insert(make_pair(pos2,*viter));
			pos1 = pos2 + (*viter).size();
		}
		++viter;
	}
	
	string key;
	for(i = 0; i < GENERALNUM; ++i)
	{
		pos1 = 0,pos2 = 0;
		key = general_hospital[i];
		while((pos2 = record.find(key,pos1)) != string::npos)
		{
			ProcessSpecial(record,key);
			m_keys.insert(make_pair(pos2,key));
			pos1 = pos2 + (key.size());
		}
	}

	//一院..简写关键词
	if(akey.compare("") != 0)
	{
		pos2 = record.find(akey);
		m_keys.insert(make_pair(pos2,akey));
	}

	vector<string> v_keys;
	multimap<string::size_type,string>::iterator miter = m_keys.begin();
	while(miter != m_keys.end())
	{
		v_keys.push_back(miter->second);
		++miter;
	}
	
	if(v_keys.size() == 0)
		cout << record<<"\t" << v_keys.size() << "\n";
	
	last_key = v_keys[v_keys.size() - 1];
	if(last_key.compare(akey) == 0)
		category = "综合医院";

	for(int i = 0; i < CLINICNUM; ++i)
		if(record.find(clinic[i]) != string::npos)
			 return false;
	
	//社区卫生服务机构
	if(IsCommunityHealthServices(record,last_key,v_keys) == true)
	{
		category = "医疗保健:综合医院:社区卫生服务机构";
		return true;
	}
	
	//**医院**科(中心)
	if(IsDepartment(record,last_key,v_keys) == true)
	{
		category = "医疗保健:综合医院:科室";
		return true;
	}
	
	//*医院*分院
	if(IsHospitalBranch(record,category,last_key,general_hospital,GENERALNUM,v_keys) == true)
	{
		if(category.compare("其它") == 0)
		{
			category = "医疗保健:综合医院:其它";
			return true;
		}
	/*	
		//医院*医院
		if(IsAffilyHospital(record,last_key,category) == true)
		{
			category = "医疗保健:综合医院:关联医院";
			return true;
		}
	*/
		category = "医疗保健:综合医院";
		return true;
	}
/*
	if(IsAffilyHospital(record,last_key,category) == true)
	{
		if(category.compare("其它") == 0)
			category = "医疗保健:综合医院:其它";
		else
			category = "医疗保健:综合医院:关皆;

		return true;
	}
*/
	//cout << record << "\t" << last_key;
	//getchar();
	//*医院
	for(int i = 0; i < GENERALNUM; ++i)
	{
		string::size_type pos = record.rfind(last_key);
		if(last_key.compare(general_hospital[i]) == 0 || last_key.find("卫生服务") != string::npos)
		{
			if(pos == record.size() - last_key.size() || ProcessLastWord(record) == true || EndWithDoor(record) == true)
			{	
				category = "医疗保健:综合医院";
				return true;
			}
			else
			{
				int j  = 0;
				for(j = 0; j < HOSCOMNUM; ++j)
				{
					string::size_type pos2 = record.rfind(hos_community[j]);
					if(pos2 != string::npos && pos2 > pos)
					{
						category = "医疗保健:综合医院:社区卫生服务机构";
						return true;
					}
				}
				category = "医疗保健:综合医院:其它";
				return true;
			}
		}
		if(category.compare("综合医院") == 0)
		{
			 category = "医疗保健:综合医院";
			 return true;
		}
	}
	
	category = hospital_category[GENERAL][last_key];
	
	return true;
}

void GetHospitalKeyWord(const string &record,const string & raw_record,const string &category, string &key_word,                                                                                    multimap<string::size_type,string> &m_keys)
{
	multimap<string::size_type,string>::iterator miter = m_keys.end();
	multimap<string::size_type,string>::size_type msize = m_keys.size();
	
	string last_key,forward_key;
	string::size_type pos1,pos2,len;
	
	if(category.compare("医疗保健:综合医院:其它") == 0 && msize >= 1)
	{
		last_key = (m_keys.begin())->second;
		pos1 = record.rfind(last_key);
		key_word = record.substr(pos1+last_key.size(),record.size()-pos1-last_key.size());
		return;
	}
		
	if(msize == 1)
	{
		last_key = (m_keys.begin())->second;
		
		for(int i = 0; i < BRANCH; ++i)
		{
			len = (branch[i]).size();
			if(record.size() >= len && record.substr(record.size()-len,len).compare(branch[i]) == 0)
			{
				pos1 = record.find(last_key);
				key_word = record.substr(pos1+last_key.size(),record.size()-pos1-last_key.size());
				return;
			}
		}
		for(int i = 0; i < AREANUM; ++i)
		{
			len = (area[i]).size();
			if(record.size() >= len && record.substr(record.size()-len,len).compare(area[i]) == 0)
			{   
				pos1 = record.find(last_key);
				key_word = record.substr(pos1+last_key.size(),record.size()-pos1-last_key.size());
				return;
			}
		}
		pos1 = record.find(last_key);
		if(pos1+last_key.size() < record.size() && category.compare("医疗保健:综合医院:社区卫生服务机构") != 0)
		{
			key_word = record.substr(pos1+last_key.size(),record.size()-pos1-last_key.size());
			return;
		}
	}
	
	if(msize >= 2)
	{
		last_key = (--miter)->second;
		pos1 = miter->first;
		forward_key = (--miter)->second;
		pos2 = miter->first;
		
		for(int i = 0; i < AREANUM; ++i)
		{
			if(last_key.compare(area[i]) == 0)
			{
				key_word = last_key;
				return;
			}
		}
		if(record.find(last_key) != string::npos)
		{
			if(pos2 + forward_key.size() < pos1)
				key_word = record.substr(pos2+forward_key.size(),record.size() - pos2 - forward_key.size());
		}
		else
		{
			pos1 = raw_record.rfind(last_key);
			pos2 = raw_record.rfind(forward_key);
			if(pos2 + forward_key.size() < pos1)
				key_word = raw_record.substr(pos2+forward_key.size(),raw_record.size() - pos2 - forward_key.size() - 1);
		}
	}		
}


bool IsHospital(const string &record)
{
	if(record.size() >= 4 && record.substr(record.size()-4,4).compare("公司") == 0)
		return false;

	return true;
}

bool GetHospitalCategory(const string &record,string &category,string &key_word,const string &raw_record)
{
	string category_temp = category;
	string last_key;
	multimap<string::size_type,string> m_keys;

	if(IsHospital(raw_record) == false)
	{
		category = "非医院";
		return true;
	}
	
	if(IsGeneralHospital(record,category,m_keys,last_key,raw_record) == true)
	{
		GetHospitalKeyWord(record,raw_record,category,key_word,m_keys);
		return true;
	}
	
	return false;
}
















