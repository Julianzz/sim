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
	
	if(record.substr(rlen-2,2).compare("ÃÅ") == 0)
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
			//**Ò½Ôº**·ÖÔº
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
					//category = "ÆäËü";
					return true;
				}
			}
			return false;  // ²»ÄÜ·µ»Øtrue,·µ»Øºó»¹ÐèÒªÅÐ¶ÏÊÇ·ñÊÇ¸½ÊôÒ½Ôº
		}
	}

	return false;
}

bool IsAffilyHospital(const string &record,const string &last_key,string &category)
{
	if(last_key.compare("Ò½Ôº") == 0)
	{
		string::size_type pos1 = record.rfind(last_key);
		string::size_type pos2 = record.find(last_key);
			
		if(pos2 != string::npos && pos2 < pos1)
		{
			if(pos1 != record.size() - 4)
				if(ProcessLastWord(record) == false)
					category = "ÆäËü";
			
			return true;
		}		
	}

	return false;
}

bool IsDepartment(const string &record,string &last_key,vector<string> &v_keys)
{
	string::size_type pos1 = 0,pos2 = 0;
	const int OTHERSHINUM = 2;
	string other_shi[2] = {"Ôº³¤ÊÒ","°ì¹«ÊÒ"};

	for(int i = 0; i < GENERALNUM; ++i)
	{
		if(last_key.compare(general_hospital[i]) == 0 || last_key.substr(last_key.size()-2,2).compare("Ôº") == 0 
				|| last_key.substr(last_key.size()-2,2).compare("Ò½") == 0)
		{
			pos1 = record.rfind(last_key);
			if(record.substr(record.size()-2,2).compare("¿Æ") == 0)
			{
				last_key = record.substr(pos1+last_key.size(),record.size() - pos1 - last_key.size());
				v_keys.push_back(last_key);
				return true;
			}

			if(last_key.substr(last_key.size()-2,2).compare("ÊÒ") != 0)
			{
				if(record.substr(record.size()-2,2).compare("ÊÒ") == 0)
				{
					for(int j = 0; j < OTHERSHINUM; ++j)
						if(record.size() >= 6 && record.substr(record.size()-6,6).compare(other_shi[j]) == 0)
							return false;
					last_key = record.substr(pos1+last_key.size(),record.size() - pos1 - last_key.size());
					v_keys.push_back(last_key);
					return true;
				}
			}

			if(record.substr(record.size()-4,4).compare("ÖÐÐÄ") == 0)
			{
				if(last_key.compare("Ò½ÁÆÖÐÐÄ") == 0 || last_key.compare("ÀÏÄê½¡¿µ·þÎñÖÐÐÄ") == 0                                                      || last_key.compare("±£½¡ÖÐÐÄ") == 0 )
					return false;
				last_key = record.substr(pos1+last_key.size(),record.size() - pos1 - last_key.size());
				v_keys.push_back(last_key);
				return true;
			}
			
			if(record.size() >= 4)
			{
				string str = record.substr(record.size()-4,4);
				if(str.compare("¼±Õï") == 0)
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

	if(record.find("ÉçÇøÒ½ÁÆÖÐÐÄ") != string::npos || record.find("Ò½ÔºÉçÇø") != string::npos                                      || record.find("ÉçÇø½¡¿µÖÐÐÄ") != string::npos || record.find("ÉçÇø¿µ¸´±£½¡ÖÐÐÄ") != string::npos)
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
	
	if(last_word.compare("Õ¾") == 0 || last_word.compare("²¿") == 0 || last_word.compare("ÊÒ") == 0 
	   || last_word.compare("Çø") == 0)
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
	if(key.compare("ÎÀÉú·þÎñ") == 0)
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
				category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:¿ÆÊÒ";
			else
				category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:ÆäËü×¨¿ÆÒ½Ôº";

			return true;
		}
	}

	//×¡Ôº¼¸²¿
	string::size_type pos1 = 0,pos2 = 0;
	while((pos2 = record.find("×¡Ôº",pos1)) != string::npos)
	{
		if(pos2 + 8 > record.size())
			break;
		if(record.substr(pos1+6,2).compare("²¿") == 0)
		{
			for(int i = 0; i < CSCHOOLNO; ++i)
			{
				if(record.substr(pos2+4,2).compare(school_number_c[i]) == 0)
				{
					category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:¿ÆÊÒ";
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
	string keys1[2] = {"Ôº","Ò½"};
	string keys2[6] = {"ÖÐ","¸½","¶«","Î÷","ÄÏ","±±"};
	
	for(int i  = 0; i < 2; ++i)
	{
		while((pos2 = record.find(keys1[i],pos1)) != string::npos)
		{
			if(pos2 <= 1)
			{
				pos1 = pos2 + 2;
				continue;
			}
			//¼¸Ôº
			for(int j = 0; j < CSCHOOLNO; ++j)
			{
				if(record.substr(pos2-2,2).compare(school_number_c[j]) == 0)
				{
					akey = record.substr(pos2-2,4);
					return true;
				}
			}
			//*ÖÐÔº,*¸½Ôº,*¶«Ôº
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
	//ÎÀÉú*ÊÒ(Ëù),ÎÀÉú·ÖÊÒ
	while((pos2 = record.find("ÎÀÉú",pos1)) != string::npos)
	{
		if(pos2 + 8 > record.size())
			break;
		if(record.substr(pos2+6,2).compare("ÊÒ") == 0 || record.substr(pos2+6,2).compare("Ëù") == 0)
		{
			for(int i = 0; i < CSCHOOLNO; ++i)
			{
				if(record.substr(pos2+4,2).compare(school_number_c[i]) == 0)
				{
					akey = record.substr(pos2,8);
					return true;
				}
			}
			
			if(record.substr(pos2+4,2).compare("·Ö") == 0)
			{
				akey = record.substr(pos2,8);
				return true;
			}
		}
		pos1 = pos2 + 4;
		if(pos1 > record.size())
			break;
	}
	
	//Ò½Ôº²¿ÃÅ
	for(int i = 0; i < HOSKEY; ++i)
	{
		if(record.find(hos_key[i]) != string::npos)
			return true;
	}

	for(int i = 0; i < ABBREAREANUM; ++i)
	{
		//**Â¥,**²¡·¿,**²¡Çø
		string::size_type len1 = record.size();
		string::size_type len2 = (abbre_area[i]).size();
		if(len1 >= len2)
			if(record.substr(len1-len2,len2).compare(abbre_area[i]) == 0)
				return true;
	}

	//À¨ºÅÀïÃæ³öÏÖÒ½Ôº
	if(record.find("Ò½Ôº") != string::npos)
		return true;

	return false;
}

bool IsAbbreSpecialist(const string &record)
{
	//×¨¿ÆÒ½Ôº
	for(int i = 0; i < ABBRESPECNUM; ++i)
	{
		if(record.find(abbre_spec_hospital[i]) != string::npos)
			return true;
	}

	return false;
}

bool IsAbbreHospital(const string &record,string &last_key)
{
	//¿ÉÄÜÊÇÒ½Ôº
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
		if(record.substr(record.size()-4,4).compare("ÎÀÉú") == 0)
		{
			last_key = "ÎÀÉú";
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
	if(category.compare("Ò½ÁÆ±£½¡:Ò©·¿Ò©µê") != 0)
		k = 3;
	
	for(int i = k; i < PHARMACYNUM; ++i)
	{
		if(record.size() >= (pharmacy[i]).size() && record.substr(record.size() - (pharmacy[i]).size(),(pharmacy[i]).size()).compare(pharmacy[i]) == 0)
		{
			if((pharmacy[i]).compare("Ò½Ò©") == 0 && record.find("Ò½Ò©´óÑ§") == string::npos)
				return true;
			if((pharmacy[i]).compare("Ò½Ò©") != 0)
				return true;
		}
	}

	string key1[4] = {"Ò©","Ò©Òµ","Ò©Æ·","±£½¡Æ·"};
	for(int i = 0; i < 4; ++i)
	{
		if(record.size() >= (key1[i]).size())
			if(record.substr(record.size()-(key1[i]).size(),(key1[i]).size()).compare(key1[i]) == 0)
				return true;
	}

	string key[5] = {"µê","Â·","µÀ","½Ö","³¬ÊÐ"};
	if(record.find("Ò©") != string::npos)
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
	if(category.compare("Ò½ÁÆ±£½¡:Ò©·¿Ò©µê") == 0)
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
	//Ò©·¿Ò©µê
	if(IsPharmacy(record,category) == true)
	{
		category = "Ò½ÁÆ±£½¡:Ò©·¿Ò©µê";
		return true;
	}
	
	//¼±¾È
	if(IsEmergencyCenter(record))
	{
		category = "Ò½ÁÆ±£½¡:¼±¾ÈÖÐÐÄ";
		return true;
	}
	
	//ÉúÖ³,¼Æ»®ÉúÓý,¼ÆÉú
	if(IsReproduct(record,category) == true)
	{
		category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:ÆäËü×¨¿ÆÒ½Ôº";
		return true;
	}
	
	//¼²²¡Ô¤·À
	if(IsDissPreve(record) == true)
	{
		category = "Ò½ÁÆ±£½¡:¼²²¡Ô¤·À";
		return true;
	}

	//×¨¿ÆÒ½Ôº
	if(category.find("Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº") == string::npos && (IsAbbreSpecialist(record) == true || IsSpecialistHospital(record) == true))
	{
		category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:ÆäËü×¨¿ÆÒ½Ôº";
		return true;
	}

	if(category.find("Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº") != string::npos)
	{
		if(record.find("¶ù¿ÆÒ½Ôº") != string::npos || record.find("¶ù¿ÆÔº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:¶ù¿Æ";
			return true;
		}
		if(record.find("²ú¿ÆÒ½Ôº") != string::npos || record.find("²úÔº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:¸¾²ú¿Æ";
			return true;
		}
		if(record.find("ÄÔ¿ÆÒ½Ôº") != string::npos || record.find("ÄÔ¿ÆÔº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:ÄÔ¿Æ";
			return true;
		}
		if(record.find("Ö×ÁöÒ½Ôº") != string::npos || record.find("Ö×ÁöÔº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:Ö×Áö";
			return true;
		}
		if(record.find("¹Ç¿ÆÒ½Ôº") != string::npos || record.find("¹Ç¿ÆÔº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:¹Ç¿Æ";
			return true;
		}
		if(record.find("ÐØ¿ÆÒ½Ôº") != string::npos || record.find("ÐØ¿ÆÔº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:ÐØ¿Æ";
			return true;
		}
		if(record.find("¶ú±ÇºíÒ½Ôº") != string::npos || record.find("¶ú±ÇºíÔº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:¶ú±Çºí";
			return true;
		}
		if(record.find("ÑÛ¿ÆÒ½Ôº") != string::npos || record.find("ÑÛ¿ÆÔº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:ÑÛ¿Æ";
			return true;
		}
		if(record.find("ÕûÐÎÒ½Ôº") != string::npos || record.find("ÕûÐÎÔº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:ÕûÐÎ";
			return true;
		}
		if(record.find("´«È¾²¡Ò½Ôº") != string::npos || record.find("´«È¾²¡Ôº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:´«È¾²¡Ò½Ôº";
			return true;
		}
		if(record.find("¾«Éñ²¡Ò½Ôº") != string::npos || record.find("¾«Éñ²¡Ôº") != string::npos)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:¾«Éñ²¡Ò½Ôº";
			return true;
		}
		if(category.compare("Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:ÆäËü×¨¿ÆÒ½Ôº") == 0 && IsSpecialistHospital(record) == true)
		{
			category = "Ò½ÁÆ±£½¡:×¨¿ÆÒ½Ôº:ÆäËü×¨¿ÆÒ½Ôº";
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
		//À¨ºÅÀïÃæ³öÏÖÒ½Ôº
		string::size_type pos1 = raw_record.rfind("(");
		if(pos1 != string::npos)
		{
			if(raw_record.find("Ò½Ôº",pos1) != string::npos)
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
	//Ò»Ôº(¼¸Ôº),×¡Ôº²¿Ö®ÀàµÄ
	if(IsAbbreviation(record,akey) == true)
		flag = true;
	
	//²»°üº¬Ò½ÔºÀàµÄ¹Ø¼ü´Ê
	if((flag == false) && i >= GENERALNUM)
	{
		//Ò©·¿Ò©µê
		//if(IsPharmacy(record,category) == true)
		//{
		//	category = "Ò½ÁÆ±£½¡:Ò©·¿Ò©µê";
		//	return true;
		//}
		
		//**¿Æ(ÒªÇø·Ö×¨¿ÆºÍÒ½Ôº¿ÆÊÒ)
		//if(IsAbbreDepartment(record,category) == true)
		//	return true;
		
		if(category.compare("Ò½ÁÆ±£½¡:ÕïËù") == 0)
			return false;
		
		if(category.find("Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº") == string::npos)
			return false;
		
		//**ÖÐÐÄ
		if((record.size() >= 4) && (record.substr(record.size()-4,4).compare("ÖÐÐÄ") == 0))
		{
			int j = 0;
			for(j = 0; j < CENTERNUM; ++j)
			{
				if(record.find(center[j]) != string::npos)
					break;
			}
			
			if( j == CENTERNUM)
			{
				category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:¿ÆÊÒ";
				return true;
			}
		}

		//**ÊÒ,**²¿
		string str = "";
		if(record.size() > 2)
			str = record.substr(record.size()-2,2);
		if(str.compare("ÊÒ") == 0 || str.compare("²¿") == 0)
		{
			if(record.find("°ì¹«ÊÒ") == string::npos && record.find("Ôº³¤ÊÒ") == string::npos)
			{
				category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:¿ÆÊÒ";
				return true;
			}
		}	
		
		//¿ÉÄÜÊÇÒ½Ôº
		if(IsAbbreHospital(record,last_key) == true && (last_key.find("ÃÅÕï") != string::npos || last_key.find("¼±Õï") != string::npos))
		{
			category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:¿ÆÊÒ";	
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
			if((*viter).compare("Â¥") == 0 && pos2 != record.size()-2)
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

	//Ò»Ôº..¼òÐ´¹Ø¼ü´Ê
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
		category = "×ÛºÏÒ½Ôº";

	for(int i = 0; i < CLINICNUM; ++i)
		if(record.find(clinic[i]) != string::npos)
			 return false;
	
	//ÉçÇøÎÀÉú·þÎñ»ú¹¹
	if(IsCommunityHealthServices(record,last_key,v_keys) == true)
	{
		category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:ÉçÇøÎÀÉú·þÎñ»ú¹¹";
		return true;
	}
	
	//**Ò½Ôº**¿Æ(ÖÐÐÄ)
	if(IsDepartment(record,last_key,v_keys) == true)
	{
		category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:¿ÆÊÒ";
		return true;
	}
	
	//*Ò½Ôº*·ÖÔº
	if(IsHospitalBranch(record,category,last_key,general_hospital,GENERALNUM,v_keys) == true)
	{
		if(category.compare("ÆäËü") == 0)
		{
			category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:ÆäËü";
			return true;
		}
	/*	
		//Ò½Ôº*Ò½Ôº
		if(IsAffilyHospital(record,last_key,category) == true)
		{
			category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:¹ØÁªÒ½Ôº";
			return true;
		}
	*/
		category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº";
		return true;
	}
/*
	if(IsAffilyHospital(record,last_key,category) == true)
	{
		if(category.compare("ÆäËü") == 0)
			category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:ÆäËü";
		else
			category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:¹ØªÒ½Ôº";

		return true;
	}
*/
	//cout << record << "\t" << last_key;
	//getchar();
	//*Ò½Ôº
	for(int i = 0; i < GENERALNUM; ++i)
	{
		string::size_type pos = record.rfind(last_key);
		if(last_key.compare(general_hospital[i]) == 0 || last_key.find("ÎÀÉú·þÎñ") != string::npos)
		{
			if(pos == record.size() - last_key.size() || ProcessLastWord(record) == true || EndWithDoor(record) == true)
			{	
				category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº";
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
						category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:ÉçÇøÎÀÉú·þÎñ»ú¹¹";
						return true;
					}
				}
				category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:ÆäËü";
				return true;
			}
		}
		if(category.compare("×ÛºÏÒ½Ôº") == 0)
		{
			 category = "Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº";
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
	
	if(category.compare("Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:ÆäËü") == 0 && msize >= 1)
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
		if(pos1+last_key.size() < record.size() && category.compare("Ò½ÁÆ±£½¡:×ÛºÏÒ½Ôº:ÉçÇøÎÀÉú·þÎñ»ú¹¹") != 0)
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
	if(record.size() >= 4 && record.substr(record.size()-4,4).compare("¹«Ë¾") == 0)
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
		category = "·ÇÒ½Ôº";
		return true;
	}
	
	if(IsGeneralHospital(record,category,m_keys,last_key,raw_record) == true)
	{
		GetHospitalKeyWord(record,raw_record,category,key_word,m_keys);
		return true;
	}
	
	return false;
}
















