#include<fstream>
#include<map>
#include<vector>
#include"classify_school.h"
#include"string_process.h"
#include"classify_data_defs.h"
#include"classify_school_defs.h"
using namespace std;

vector<vector<string> > school_keys;
vector<map<string,string> > school_category;


void GetSchoolKeyCategory(const string &filepath)
{
	string files[4] = {"key_nursery","key_primary","key_middle","key_high"};
	vector<string> keys;
	map<string,string> key_category;
	
	for(int i = 0; i < 4; ++i)
	{
		ReadMapFile((filepath+files[i]).c_str(),keys,key_category);
		school_keys.push_back(keys);
		school_category.push_back(key_category);
		keys.clear();
		key_category.clear();
	}
}

bool IsSpecialUniversity(const string &record,string &last_key)
{
	for(int i = 0; i < SUNUM; ++i)
	{
		if(record.find(suniver[i]) != string::npos)
		{
			last_key = suniver[i];
			return true;
		}
	}

	return false;
}

bool SchoolStyle(const string &record,const string &last_key,string &category,const string school[],int num)
{
	for(int i = 0; i < num; ++i)
	{
		if(category.compare(school[i]) == 0)	//���Ĺؼ�����ѧУ
		{
			string::size_type pos1 = record.rfind(last_key),pos2 = 0;
				
			for(int j = 0; j < DOORNUM; ++j)	// **ѧУ**��
			{
				if((pos2 = record.find(door[j],pos1+last_key.size())) != string::npos)
					if(pos2 == record.size() - (door[j]).size())
						return true;
			}
			
			for(int i = 0; i < CAMPUSNUM; ++i)	// **ѧУ**��У**��
			{
				if((pos1 = record.find(campus[i])) != string::npos)
				{
					if(pos1 == record.size() - (campus[i]).size())
						return true;
					for(int j = 0; j < DOORNUM; ++j)
					{
						if((pos2 = record.rfind(door[j])) != string::npos)
						{
							if(pos2 == record.size()- (door[j]).size())
								return true;
						}
					}
				}
			}

			// **��
			if(record.substr(record.size()-2,2).compare("��") == 0)
			{
				string sno = record.substr(record.size()-4,2);
				for(int i = 0; i < CSCHOOLNO; ++i)
				{
					if(sno.compare(school_number_c[i]) == 0)
						return true;
				}
			}
		
			string::size_type pos = record.rfind(last_key);
			if(pos != record.size() - last_key.size())
			{
				category = "����";
				return true;
			}

			return false;
		}
	}

	return false;
}

bool IsSecondaryCollege(const string &record,string &category,const vector<string> &v_keys)
{
	string last_key = v_keys[v_keys.size() - 1];

	for(int k = 0; k < SECONDNUM; ++k)
	{
		if(last_key.compare(second_school[k]) == 0)
		{
			if(v_keys.size() > 1)
			{
				for(int i = 0; i < v_keys.size() - 1; ++i)
				{
					for(int j = 0; j <  HIGHNUM; ++j)
					{
						if((v_keys[i]).compare(high_school[j]) == 0)
						{
							string::size_type pos1 = record.find(v_keys[i]);
							string::size_type pos2 = record.find(last_key);
							if(pos1 + (v_keys[i]).size() != pos2)
							{
								category = school_category[HIGH]["����ѧԺ"];
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool CenterType(const string &record,string &category,const vector<string> &v_keys)
{
	string last_key = v_keys[v_keys.size()-1];

	if(last_key.compare("����") == 0)
	{
		string::size_type pos = record.find("����");
		string key;
		if(pos >= 4)
			key = record.substr(pos-4,8);
		else
			key = record.substr(0,pos+4);

		for(int i = 0; i < ADMINCENTERNUM; ++i)
		{
			if(key.compare(admincenter[i]) == 0)
			{
				category = school_category[HIGH]["��������"];
				return true;
			}
		}
		for(int i = 0; i < PLACECENTERNUM; ++i)
		{
			if(key.compare(placecenter[i]) == 0)
			{
				category = school_category[HIGH]["��������"];
				return true;
			}
		}
		
		if(key.compare("��������") == 0 || key.compare("�ڿ�����") == 0)
		{
			category = school_category[HIGH]["��ѧ"];
			return true;
		}

		 if(key.compare("ѧϰ����") == 0)
		{   
			last_key = "ѧϰ����";  
			if(SetKeyType(record,last_key,"ѧϰ����","�о�ѧϰ����","Զ��ѧϰ����",studycenter,STUDYCENTERNUM) == true)
			{
				category = school_category[HIGH][last_key];
				return true;
			}
			return false;
		}
		
		if(key.compare("��ѧ����") == 0)
		{
			last_key = "��ѧ����";	
			if(SetKeyType(record,last_key,"��ѧ����","�о���ѧ����","Զ�̽�ѧ����",techcenter,TECHCENTERNUM) == true)
			{
				category = school_category[HIGH][last_key];
				return true;
			}
			return false;
		}

		if(key.compare("��������") == 0)
		{
			last_key = "��������";
			if(SetKeyType(record,last_key,"��������","�о���������","Զ�̽�������",educatecenter,EDUCATECENTERNUM) == true)
			{
				category = school_category[HIGH][last_key];
				return true;
			}
			return false;
		}
				

		category = school_category[HIGH]["��������"];
		
		return true;
	}

	return false;
}

bool SetKeyType(const string &record,string &last_key,string keykind,string keymapkind1,string keymapkind2,string part[]                    ,int num)
{
	if(last_key.compare(keykind) == 0)
	{
		for(int i = 0; i < num; ++i)
		{
			string::size_type len = (part[i]).size() - keykind.size();
			string::size_type pos = record.find(keykind);
			string key;
			if(pos >= len)
				key = record.substr(pos-len,(part[i]).size());
			else
				key = record.substr(0,pos+keykind.size());

			if(key.compare(part[i]) == 0)
			{
				last_key = keymapkind1;
				return true;
			}
		}

		last_key = keymapkind2;
		return true;
	}

	return false;
}

bool TechDepartType(const string &record,string &category,const vector<string> &v_keys)
{
	string last_key = v_keys[v_keys.size()-1];

	if(SetKeyType(record,last_key,"��ѧ��","�о���ѧ��","Զ�̽�ѧ��",techdepart,TECHDEPARTNUM) == true)
	{
		category = school_category[HIGH][last_key];
		return true;
	}
	else
		return false;
}

bool IsHighSchool(const string &record,string &category,multimap<string::size_type,string> &m_keys,string &last_key)
{
	int i;
	string::size_type pos3 = 0;
	for(i = 0; i < HIGHNUM; ++i)
	{
		if((pos3 = record.find(high_school[i])) != string::npos)
			break;
	}
	if(i == HIGHNUM)
		return false;

	// �������а�������"*��"
	if(i >= UABBNUM)
	{
		int j;
		for(j = 0; j < UABBNUM; ++j)
		{
			if(record.find(high_school[j],pos3+2) != string::npos) //"*��"�����Ƿ���ִ�ѧ��Ĵ� 
				break;
		}
		if(j >= UABBNUM)
			if(category.compare("����ѧУ:��ѧ") != 0)			  // ��ǰ���಻����"��ѧ"����
				return false;
	}

	if((high_school[i]).compare("��ѧ") == 0)
	{
		pos3 = record.find(high_school[i]);
		if(record.substr(pos3+2,4).compare("ѧ��") == 0)
			return false;
	}
	
/*	
	if(IsSpecialUniversity(record,last_key) == true)
	{
		category =	school_category[HIGH]["����ѧУ"];
		return true;
	}
*/
	string::size_type pos4 = record.find("����");
	if(pos4 != string::npos)
	{
		if(record.find("У",pos4) != string::npos)
		{
			last_key = "����";
			category =  school_category[HIGH]["����ѧУ"];
			return true;
		}
	}
/*
	pos4 = record.find("ѧУ");
	if(pos4 != string::npos && pos4 != pos3+4)
	{
		category =  school_category[HIGH]["����ѧУ"];
		ofile11 << record << "\t" << category << "\n";
		return true;
	}
*/	
	string::size_type pos1 = 0,pos2 = 0;
	//multimap<string::size_type,string> m_keys;
	vector<string>::iterator viter = (school_keys[HIGH]).begin();
	
	while(viter != (school_keys[HIGH]).end())
	{
		pos1 = 0,pos2 = 0;
		while((pos2 = record.find(*viter,pos1)) != string::npos)
		{
			m_keys.insert(make_pair(pos2,*viter));
			pos1 = pos2 + (*viter).size();
		}
		++viter;
	}

	string ukey;
	for(int i = UNUM; i < HIGHNUM; ++i)
	{
		pos1 = 0,pos2 = 0;
		while((pos2 = record.find(high_school[i],pos1)) != string::npos)
		{
			ukey = high_school[i];
			if(ukey.compare("��ѧ") == 0)
			{
				if(record.substr(pos2+2,4).compare("ѧ��") == 0)
					ukey = "";
			}
			if(ukey.compare("��ѧ") != 0 && ukey.find("��") != string::npos)
			{
				if(record.substr(pos2+2,4).compare("��ѧ") == 0)
					ukey = "��ѧ";
			}
			if(ukey.compare("ʦ��") == 0)
			{
				string tempstr = record.substr(pos2+4,4);
				if(tempstr.compare("��ѧ") == 0 || tempstr.compare("ѧԺ") == 0 || tempstr.compare("ѧУ") == 0)
					ukey = record.substr(pos2,8);
			}
			if(ukey.compare("") != 0 && ukey.compare("��ѧ") != 0)
				m_keys.insert(make_pair(pos2,ukey));
			pos1 = pos2 + (ukey.size());
		}
	}
	
	vector<string> v_keys;
	multimap<string::size_type,string>::iterator miter = m_keys.begin();
	
	while(miter != m_keys.end())
	{
		v_keys.push_back(miter->second);
		++miter;
	}

	last_key = v_keys[v_keys.size() - 1];
	category = last_key;

	if(SchoolStyle(record,last_key,category,high_school,HIGHNUM) == true)
	{
		last_key = category;
		for(int i = 0; i <HIGHNUM; ++i)
		{
			if(last_key.compare(high_school[i]) == 0)
			{
				if(v_keys.size() > 1)
					category = school_category[HIGH]["����ѧԺ"];
				else
					category = school_category[HIGH]["��ѧ"];
				return true;
			}
		}
	}

	for(int i = 0; i < HIGHNUM; ++i)
	{
		if(last_key.find("ʦ��") != string::npos)
			last_key = "ʦ��";
		if(last_key.compare(high_school[i]) == 0)
		{
			if(IsSecondaryCollege(record,category,v_keys) == true)
				return true;

			category = school_category[HIGH]["��ѧ"];
			return true;
		}   
	}   
	

	if(TechDepartType(record,category,v_keys) == true)
		return true;
	
	if(CenterType(record,category,v_keys) == true)
		return true;


	for(int i = 0; i < HIGHNUM; ++i)
	{
		if(last_key.compare(high_school[i]) == 0)
		{
			category = school_category[HIGH]["��ѧ"];
			return true;
		}
	}
	
	category = school_category[HIGH][last_key];
		
	return true;
}

bool IsNursery(const string &record,string &category,string &last_key,string &key_word)
{
	string::size_type pos = 0;
	for(int i = 0; i < NURSERYNUM; ++i)
	{
		pos = record.find(nursery_school[i]);
		if(pos != string::npos)
		{
			category = nursery_school[i];
			last_key = nursery_school[i];
			if(pos + last_key.size() < record.size())
				key_word = record.substr(pos+last_key.size(),record.size()-pos-last_key.size());
			return true;
		}
	}

	return false;
}

bool GetSchoolNo(const string &record,string::size_type pos,int cnum,string school_number[],int school_num,string str2,						string &category,string &last_key)
{
	string strno = record.substr(pos-cnum,cnum);
	for(int i = 0; i < school_num; ++i)
	{
		if(strno.compare(school_number[i]) == 0)
		{
			category = str2;
			last_key = record.substr(pos-cnum,cnum+2);
			return true;
		}
	}

	return false;
}

bool PrimaryAndMiddle(const string &record,string &category,int num,string school[],string str,string str2,                                      string &last_key)
{
	for(int i = 0; i < num; ++i)
	{
		if(record.find(school[i]) != string::npos)
		{
			category = school[i];
			last_key = school[i];
			return true;
		}
	}

	string strno,str1;
	string::size_type pos1 = 0,pos2 = 0;
		for(int j = 0; j < str.size(); j += 2)
		{
			pos1 = 0;
			pos2 = 0;
			str1 = str.substr(j,2);
			while((pos2 = record.find(str1,pos1)) != string::npos)
			{
				if(pos2 >= 1)
					if(GetSchoolNo(record,pos2,1,school_number_a,10,str2,category,last_key) == true)
						return true;

				if(pos2 >= 2)
					if(GetSchoolNo(record,pos2,2,school_number_c,10,str2,category,last_key) == true)
						return true;

				pos1 = pos2 + 2;
			}
		}
	
	return false;
}

bool IsPrimarySchool(const string &record,string &category,string &last_key)
{
	string temp_cate = category;
	
	if(PrimaryAndMiddle(record,category,PRIMARYNUM,primary_school,"С","Сѧ",last_key) == true)
	{
		for(int i = 0; i < PRIMARYSPECIALNUM; ++i)
		{
			if(record.find(primary_special_case[i]) != string::npos)
				return false;
		}
		
		if(category.compare("Сѧ") == 0)
		{
			string::size_type pos1 = record.find("Сѧ");
			string::size_type pos2 = record.find("ѧԺ");
			if(pos2 != string::npos && pos1 < pos2)
				return false;
		}
		
		return true;
	}

	if(temp_cate.compare("����ѧУ:Сѧ") == 0)
	{
		if(record.find("ѧУ") != string::npos)
		{
			category = "Сѧ";
			last_key = "ѧУ";
			return true;
		}
	}

	return false;
}

bool IsMiddleSchool(const string &record,string &category,string &last_key)
{
	if(PrimaryAndMiddle(record,category,MIDDLENUM,middle_school,"�и�","��ѧ",last_key) == true)
	{
		for(int i = 0; i < MIDDLESPECIALNUM; ++i)
		{
			if(record.find(middle_special_case[i]) != string::npos)
				return false;
		}

		return true;
	}

	return false;
}

bool IsOccupSchool(const string &record,string &category,string &key_word)
{
	string::size_type pos = 0;
	for(int i = 0; i < OCCUPNUM; ++i)
	{
		pos = record.find(occup_school[i]);
		if(pos != string::npos)
		{
			if(pos + (occup_school[i]).size() < record.size())
				key_word = record.substr(pos + (occup_school[i]).size(),record.size() - pos - (occup_school[i]).size());
			category = "����ѧУ:ְҵ����ѧУ";
			return true;
		}
	}

	return false;
}

void StripBraket(string &record)
{
	string::size_type pos1 = 0,pos2 = 0;
	string trecord = record;
	while((pos1 = record.find_first_of("()")) != string::npos)
	{
		trecord = record.substr(0,pos1);
		trecord += record.substr(pos1+1,record.size() - pos1 -1);
		record = trecord;
		trecord = "";
	}
	
}

void GetSchoolKeyWord(const string &record,const string &raw_record,multimap<string::size_type,string> &m_keys,                                                                                         string &key_word,const string &last_key)
{
	multimap<string::size_type,string>::size_type len = m_keys.size();
	
	if(len == 0)
		return;
	
	string kword1,kword2;
	string::size_type pos1 = 0,pos2 = 0;

	if(last_key.compare("����") == 0)
	{
		pos1 = record.find(last_key);
		key_word = record.substr(pos1,record.size()-pos1);
		return;
	}

	multimap<string::size_type,string>::iterator miter = m_keys.end();
	
	if(last_key.compare("����") == 0)
	{
		kword1 = (--miter)->second;
		pos1 = record.rfind(kword1);
		key_word = record.substr(pos1+kword1.size(),record.size()-pos1);
		
		return;
	}
	
	multimap<string::size_type,string>::iterator miter1,miter2;

	if(len >= 2)
	{
		miter2 = --miter;
		miter1 = --miter2;
		pos1 = record.find(miter1->second);
		pos2 = record.find(miter2->second,pos1+(miter1->second).size());
		key_word = record.substr(pos1+(miter1->second).size(),pos2-pos1);
		return;
	}
	
	for(int i = 0; i < CAMPUSNUM; ++i)
	{
		if((pos1 = record.rfind(campus[i])) != string::npos)
		{
			kword1 = (--miter)->second;
			if(len >= 2)
				pos2 = record.rfind(kword1);
			else
				pos2 = record.find(kword1);
			if(pos2 + kword1.size() != pos1)
				key_word = record.substr(pos2+kword1.size(),record.size() - pos2 - kword1.size());
			return;
		}
	}

	string str;
	if((pos1 = raw_record.find("(")) != string::npos)
	{
		if(pos2 = raw_record.find(")",pos1) != string::npos)
			str = raw_record.substr(pos1+1,raw_record.size()-pos1-2);
		else
			str = raw_record.substr(pos1+1,raw_record.size()-pos1-1);
		for(int i = 0; i < CAMPUSNUM; ++i)
		{
			string::size_type pos3 = str.find(campus[i]);
			if(pos3 != string::npos && (pos3 + (campus[i]).size() == str.size()))
			{
				key_word = str;
				return;
			}
		}
	}
}

bool ProcessSpecial(const string &record,string school[],int num,string &key_word)
{
	string::size_type pos = 0;
	for(int i = 0; i < num; ++i)
	{
		pos = record.find(school[i]);
		if(pos != string::npos)
		{
			if(pos + (school[i]).size() < record.size())
				key_word = record.substr(pos+(school[i]).size(),record.size() - pos - (school[i]).size());
			return true;
		}
	}

	return false;
}

bool SchoolWithBuAndXiao(const string &record,const string &str)
{
	string::size_type pos = record.find(str);
	string strtemp;
	if(pos != string::npos && pos == record.size()-6)
	{
		strtemp = record.substr(pos+4,2);
		if(strtemp.compare("��") == 0 || strtemp.compare("У") == 0)
			return true;
	}

	return false;
}

bool GetSchoolCategory(const string &record,string &category,string &key_word,const string &raw_record)
{
	string temp_cate = category;
	string last_key;
	multimap<string::size_type,string> m_keys;

	if(ProcessSpecial(record,train_school,TRAINNUM,key_word) == true)
	{
		category = "����ѧУ:��ѵ";
		return true;
	}

	if(ProcessSpecial(record,special_school,SPECIALSCHOOLNUM,key_word) == true)
	{
		category = "����ѧУ:����ѧУ";
		return true;
	}

	if(ProcessSpecial(record,suniver,SUNUM,key_word) == true)
	{
		category = "����ѧУ:���˽���";
		return true;
	}
	
	if(IsNursery(record,category,last_key,key_word) == true)
	{	
		//SchoolStyle(record,last_key,category,nursery_school,NURSERYNUM);
		//category = school_category[NURSERY][category];
		category = "����ѧУ:�׶�԰";
		return true;
	}

	if(temp_cate.compare("����ѧУ:Сѧ") == 0)
	{
		if((record.find("У") == record.size()-2) || (record.find("С") == record.size()-2)                                            || record.find("����У") != string::npos || record.find("����ѧУ") != string::npos)
		{
			category = school_category[PRIMARY]["Сѧ"];
			return true;
		}
	}
	
	if(IsPrimarySchool(record,category,last_key) == true)
	{
		string::size_type pos = 0;
		SchoolStyle(record,last_key,category,primary_school,PRIMARYNUM);
		if(category.compare("����") == 0)
		{
			if(SchoolWithBuAndXiao(record,"Сѧ") == true)	
				category = "Сѧ";
		}
		if(category.compare("����") == 0)
		{
			pos = record.rfind(last_key);
			if(pos != string::npos && pos + last_key.size() < record.size())
				key_word = record.substr(pos+last_key.size(),record.size() - pos - last_key.size());
		}
		category = school_category[PRIMARY][category];
		return true;
	}


	if(IsOccupSchool(record,category,key_word) == true)
		return true;
	
	if(IsMiddleSchool(record,category,last_key) == true)
	{
		string::size_type pos = 0;
		SchoolStyle(record,last_key,category,middle_school,MIDDLENUM);
		if(category.compare("����") == 0)
		{
			if(SchoolWithBuAndXiao(record,"��ѧ") == true || SchoolWithBuAndXiao(record,"����") == true 
			   || SchoolWithBuAndXiao(record,"����") == true)
				category = "��ѧ";
		}
		if(category.compare("����") == 0)
		{
			pos = record.rfind(last_key);
			if(pos != string::npos && pos + last_key.size() < record.size())
				key_word = record.substr(pos+last_key.size(),record.size() - pos - last_key.size());
		}
		category = school_category[MIDDLE][category];
		return true;
	}

	//if(IsOccupSchool(record,category) == true)
	//	return true;
	if(IsHighSchool(record,category,m_keys,last_key) == true)
	{
		GetSchoolKeyWord(record,raw_record,m_keys,key_word,last_key);
		return true;
	}

	if(ProcessSpecial(record,train_school2,TRAINNUM2,key_word) == true)
	{
		category = "����ѧУ:��ѵ";
		return true;
	}
	
	if(record.find("��ѵ") != string::npos)
	{
		category = "����ѧУ:��ѵ";
		return true;
	}
	
	if(ProcessSpecial(record,adult_school,ADULTNUM,key_word) == true)
	{
		category = "����ѧУ:���˽���";
		return true;
	}

	if(temp_cate.compare("����ѧУ:��ѧ") == 0)
	{
		if(record.find("У") != string::npos || record.find("��") != string::npos)
		{
			category = "����ѧУ:�еȽ���ѧУ";
			return true;
		}
	}

	if(temp_cate.compare("����ѧУ:�׶�԰") != 0 && temp_cate.compare("����ѧУ:��ѵ") != 0              
		&& temp_cate.compare("����ѧУ:ְҵ����ѧУ") != 0 && temp_cate.compare("����ѧУ:���˽���") != 0)
	{
		for(int i = 0; i < OTHERNUM; ++i)
		{
			if(record.find(other_school[i]) != string::npos)
			{
				category = "����ѧУ:��������ѧУ";
				return true;
			}
		}
	}
/*
	if(record.find("¥") != string::npos)
	{
		category = school_category[HIGH]["¥"];
		return true;
	}

	if(record.find("ϵ") == (record.size()-2))
	{
		category = school_category[HIGH]["ϵ"];
		return true;
	}
*/	
	return false;
}

bool IsSchool(const string &record)
{
	for(int i = 0; i < NOTSCHOOLNUM; ++i)
	{
		if(record.find(not_school[i]) != string::npos)
		{
			for(int j = 0; j < NOTSCHOOLSPECIALNUM; ++j)
			{
				if(record.find(not_school_special[j]) != string::npos)
					return true;
			}
			return false;
		}
	}

	return true;
}

