#ifndef classify_school_h
#define classify_school_h
#include<iostream>
using namespace std;

bool SetKeyType(const string &,string &,string,string,string,string part[],int num);

bool GetSchoolCategory(const string &record,string &category,string &key_word,const string &raw_record);

bool IsSchool(const string &record);

void GetSchoolKeyCategory(const string &filepath);

#endif
