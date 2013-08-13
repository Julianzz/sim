#include<fstream>
#include<map>
#include<vector>
#include"string_process.h"
#include"classify_school.h"
#include"classify_hospital.h"
#include"StringUtility.h"
using namespace std;

//map<string,string> cate_code;

void ProcessSchoolData(const string &record, const string &name_raw,
		const string &name_process, const string &name_nobraket,
		const string & category_old, string &record_new,
		const map<string, string> &cate_code) {
	string key_word;
	string category_new = category_old;
	map<string, string>::const_iterator code_it;

	if (IsSchool(name_nobraket) == false) {
		//record_new = record + "\t" + "非学校#" + cate_code.find("非学校")->second + "#" + "1.0" + "\n";
		record_new = record + "\t" + "非学校#" + cate_code.find("非学校")->second
				+ "#" + "1.0#";
		return;
	}

	if (category_old.compare("教育学校:幼儿园") == 0
			|| category_old.compare("教育学校:培训") == 0
			|| category_old.compare("教育学校:职业技术学校") == 0
			|| category_old.compare("教育学校:其它教育学校") == 0
			|| category_old.compare("教育学校:成人教育") == 0) {
		if (GetSchoolCategory(name_nobraket, category_new, key_word,
				name_process) == true) {
			//record_new = record + "\t" + category_new + "#" + cate_code.find(category_new)->second + "#" + "1.0#" + key_word + "\n";
			code_it = cate_code.find(category_new);
			if (code_it != cate_code.end()) {
				record_new = record + "\t" + category_new + "#"
						+ code_it->second + "#" + "1.0#" + key_word;
			}
		} else
			record_new = record + "\t" + category_old + "#"
					+ cate_code.find(category_old)->second + "#" + "1.0#";
		return;
	}

	if (GetSchoolCategory(name_nobraket, category_new, key_word, name_process)
			== false)
		record_new = record + "\t" + "非学校#" + cate_code.find("非学校")->second
				+ "#" + "1.0#";
	else {
		code_it = cate_code.find(category_new);
		if (code_it != cate_code.end()) {
			record_new = record + "\t" + category_new + "#"
					+ code_it->second + "#" + "1.0#" + key_word;
		}
//		record_new = record + "\t" + category_new + "#"
//				+ cate_code.find(category_new)->second + "#" + "1.0#"
//				+ key_word;
	}

}

void ProcessHospitalData(const string &record, const string &name_raw,
		const string &name_process, const string &name_nobraket,
		const string & category_old, string &record_new,
		const map<string, string> &cate_code) {
	string key_word = "";
	string category_new = category_old;
	map<string, string>::const_iterator code_it;

	if (GetHospitalCategory(name_nobraket, category_new, key_word, name_process)
			== false)
		//record_new = record + "\t" + "非医院#" + cate_code["非医院"] + "#" + "1.0#" + "\n";
		record_new = "";
	else{
		code_it = cate_code.find(category_new);
		if (code_it != cate_code.end()) {
			record_new = record + "\t" + category_new + "#"
					+ code_it->second + "#" + "1.0#" + key_word;
		}
//		record_new = record + "\t" + category_new + "#"
//				+ cate_code.find(category_new)->second + "#" + "1.0#"
//				+ key_word;
	}
}

string DeleteBracket(string record) {
	string precord = record;
	string::size_type pos1 = 0, pos2 = 0;

	while ((pos1 = record.find("(")) != string::npos) {
		pos2 = record.find(")", pos1);
		if (pos2 == string::npos)
			pos2 = record.size() - 1;
		precord = record.substr(0, pos1)
				+ record.substr(pos2 + 1, record.size() - pos2 - 1);
		record = precord;
	}

	while ((pos1 = record.find("<")) != string::npos) {
		pos2 = record.find(">", pos1);
		if (pos2 == string::npos)
			pos2 = record.size() - 1;
		precord = record.substr(0, pos1)
				+ record.substr(pos2 + 1, record.size() - pos2 - 1);
		record = precord;
	}

	return precord;
}

void DeleteBlanket(string &record) {
	string::size_type len = record.size();
	string temp;

	for (int i = 0; i < len; ++i) {
		if (record[i] != ' ')
			temp += record[i];
	}

	record = temp;
}

void GetKeyCategory(const string &filepath) {
	GetSchoolKeyCategory(filepath);
	GetHospitalKeyCategory(filepath);
}

void GetNameAndCategory(const string &record, string &name, string &category) {

	string::size_type pos = record.find("\t");
	category = record.substr(pos + 1, record.size() - pos - 1);
	name = record.substr(0, pos);
	/*
	 vector<string> fields;
	 StringUtility::Split(record,"\t",fields,true);

	 if(fields.size() >= 51)
	 {
	 name = fields[27];
	 category = fields[25];
	 category = StringUtility::ReplaceStr(category, ";", ":");
	 }
	 */
}

void ClassifyData(const string &name, const string &category, string &result,
		const map<string, string> &cate_code) {
	//GetKeyCategory();
	//map<string,string> ode_cate;
	//GetCateCode("cate_code.txt",code_cate,cate_code);

	string record, record_new, key_word;
	string name_raw, name_process, name_nobraket;
	string::size_type pos = 0;
	string category_old = StringUtility::ReplaceStr(category, ";", ":");

	record = name;
	record_new = "";
	name_raw = name;

	if (name_raw.compare("") != 0) {
		name_process = name_raw;
		FullWidthToHalfWidth(name_process);
		name_nobraket = DeleteBracket(name_process);
		DeleteBlanket(name_nobraket);

		if (category_old.find("教育学校") != string::npos)
			ProcessSchoolData(record, name_raw, name_process, name_nobraket,
					category_old, record_new, cate_code);
		if (category_old.find("医疗保健") != string::npos)
			ProcessHospitalData(record, name_raw, name_process, name_nobraket,
					category_old, record_new, cate_code);
	}

	if (record_new.compare("") != 0) {
		pos = record_new.find("\t");
		result = record_new.substr(pos + 1, record_new.size() - pos - 1);
	} else
		result = "";
}

