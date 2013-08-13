#ifndef classify_hospital_h
#define classify_hospital_h
#include<iostream>
using namespace std;


bool GetHospitalCategory(const string &record,string &category,string &key_word,const string &raw_record);

bool IsHospital(const string &record);

void GetHospitalKeyCategory(const string &filepath);


#endif
																		                 
