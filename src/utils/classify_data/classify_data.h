#ifndef classify_data_h
#define classify_data_h
using namespace std;
#include<map>
void GetKeyCategory(const string &filepath);
void ClassifyData(const string &name,const string &category_old,string &result,const map<string,string> &cate_code);

#endif
