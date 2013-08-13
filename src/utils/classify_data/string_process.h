#ifndef STRING_PROCESS_H
#define STRING_PROCESS_H
#include<iostream>
#include<map>
#include<vector>
using namespace std;

void FullWidthToHalfWidth(string &str);
void LowerWord(string &str);
void GetCateCode1(const char *filename,map<string,string> &cate_code);
void GetCateCode2(const char *filename,map<string,string> &code_cate,map<string,string> &cate_code);
void ReadMapFile(const char *filename,vector<string> &keys,map<string,string> &key_category);
#endif
