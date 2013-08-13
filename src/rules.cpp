#include "rules.h"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <map>
#include <string>
#include "utils/contrib.h"
#include "utils/computeAddressSimilarity.h"
using namespace std;

//char g_szStr[100 * 1024];;
void out_vector(const std::vector<std::string> &v) {
	std::cout << "begin:";
	for (std::vector<std::string>::const_iterator iter = v.begin();
			iter != v.end(); iter++) {
		//std::cout << (*iter).length() << *iter << "--";
		std::cout << *iter << "--";
	}
	std::cout << std::endl;
}

void out_set(const _set &v) {
	std::cout << "begin:";
	for (_set::const_iterator iter = v.begin(); iter != v.end(); iter++) {
		std::cout << *iter << "--";
	}
	std::cout << std::endl;
}

void out_set(const set<int> &v) {
	std::cout << "begin:";
	for (set<int>::const_iterator iter = v.begin(); iter != v.end(); iter++) {
		std::cout << *iter << "--";
	}
	std::cout << std::endl;
}

void out_set(const set<unsigned long long> &v) {
	std::cout << "begin:";
	for (set<unsigned long long>::const_iterator iter = v.begin();
			iter != v.end(); iter++) {
		std::cout << *iter << "--";
	}
	std::cout << std::endl;
}

void out_hashset(const _hashset &v) {
	std::cout << "begin:";
	for (_hashset::const_iterator iter = v.begin(); iter != v.end(); iter++) {
		std::cout << *iter << "--";
	}
	std::cout << std::endl;
}

void out_map(const map<string, bool> &data) {
	map<string, bool>::const_iterator it = data.begin();
	for (; it != data.end(); ++it) {
		if (it->second) {
			cout << ">>>" << it->first << "\t" << it->second << "\n";
		}
	}
}

void out_map(const map<string, string> &data) {
	map<string, string>::const_iterator it = data.begin();
	for (; it != data.end(); ++it) {
		cout << ">>>" << it->first << "\t" << it->second << "\n";
	}
}

void out_map(const map<string, int> &data) {
	map<string, int>::const_iterator it = data.begin();
	for (; it != data.end(); ++it) {
		cout << ">>>" << it->first << "\t" << it->second << "\n";
	}
}

void out_map(const map<int, string> &data) {
	map<int, string>::const_iterator it = data.begin();
	for (; it != data.end(); ++it) {
		cout << ">>>" << it->first << "\t" << it->second << "\n";
	}
}

bool wordIsSame(const std::string &a, const std::string &b) {
	if (a == b)
		return true;
	else if (a.find(b) != std::string::npos || b.find(a) != std::string::npos)
		return true;
	else
		return false;
}

bool wordIsSame(const std::string &a, const std::string &b, int length) {
	if (a == b)
		return true;
	else if (abs((int) a.length() - (int) b.length()) <= length
			&& (a.find(b) != std::string::npos || b.find(a) != std::string::npos))
		return true;
	else
		return false;
}

bool listIsSame(_vector &a, _vector &b) {
	_vectorit ait = a.begin();
	_vectorit bit;
	for (; ait != a.end(); ++ait) {
		for (bit = b.begin(); bit != b.end(); ++bit) {
			if (wordIsSame(*ait, *bit))
				return true;
		}
	}
	return false;
}

int findSameInList(_vector &a, _vector &b, _vector &result) {
	_vectorit ait, bit;
	ait = a.begin();
	for (; ait != a.end(); ++ait) {
		bit = b.begin();
		while (bit != b.end()) {
			if (wordIsSame(*ait, *bit)) {
				result.push_back(*bit);
				bit = b.erase(bit);
			} else
				++bit;
		}
	}
	return 0;
}

int mergeList(_vector &src, _vector &result) {
	std::string delim("#"), tempStr("");
	_vectorit resultIt;
	_vectorit srcIt = src.begin();
	_vector temp;
	if (result.size() == 0) {
		for (; srcIt != src.end(); ++srcIt) {
			tempStr += delim;
			tempStr += *srcIt;
			tempStr += delim;
			result.push_back(tempStr);
			//tempStr.clear();
			tempStr = "";
		}
	} else {
		for (; srcIt != src.end(); ++srcIt) {
			for (resultIt = result.begin(); resultIt != result.end();
					++resultIt) {
				tempStr.clear();
				tempStr += *resultIt;
				tempStr += *srcIt;
				tempStr += delim;
				temp.push_back(tempStr);
			}
		}
		result.swap(temp);
	}
	return 0;
}

//int mergeNumAlp(_vector &src, _vector &result, _hashset &measure) {
//	result.clear();
//	_vectorit srcIt = src.begin();
//	std::string temp("");
//	for (; srcIt != src.end(); ++srcIt) {
//		if (((*srcIt) == "第") || (len(*srcIt) == (*srcIt).length())
//				|| (measure.find(*srcIt) != measure.end())) {
//			temp += *srcIt;
//		} else {
//			if (temp != "") {
//				result.push_back(temp);
//			}
//			result.push_back(*srcIt);
//		}
//	}
//
//	return 0;
//}

//int mergeNumAlp(_vector &src, _vector &result, _hashset &measure) {
//	result.clear();
//	_vectorit srcIt = src.begin();
//	std::string temp("");
//	for (; srcIt != src.end(); ++srcIt) {
//		if ((*srcIt) == "第") {
//			if (srcIt + 1 < src.end() && len(*(srcIt + 1)) == (*(srcIt + 1)).length()){
//				// num
//				if(srcIt +2 < src.end() && measure.find(*(srcIt + 2)) != measure.end()){
//					// measure word
//					temp = (*srcIt) + (*(srcIt + 1)) + (*(srcIt + 2));
//					result.push_back(temp);
//					srcIt += 2;
//				}else{
//					temp = (*srcIt) + (*(srcIt + 1));
//					result.push_back(temp);
//					srcIt += 1;
//				}
//			}else{
//				result.push_back(*srcIt);
//			}
//		} else if (len(*srcIt) == (*srcIt).length()) {
//			if (srcIt + 1 < src.end() && measure.find(*(srcIt + 1)) != measure.end()){
//				temp = (*srcIt) + (*(srcIt + 1));
//				result.push_back(temp);
//				srcIt += 1;
//			}else{
//				result.push_back(*srcIt);
//			}
//		} else {
//			result.push_back(*srcIt);
//		}
//	}
//
//	return 0;
//}

int mergeNumAlp(const std::string &srcStr, _vector &src, _vector &tempVec,
		_vector &tempResultVec, _set &result, const _hashset &measure,
		bool isInBracket) {
	tempVec.clear();
	tempResultVec.clear();
	if (src.size() == 0) {
		return 0;
	}
	_vectorit srcIt = src.begin();
	std::string temp("");
	for (; srcIt < src.end(); ++srcIt) {
		if ((*srcIt) == "第") {
			if (srcIt + 1 < src.end() && isAlpDig(*(srcIt + 1))) {
				// num
				if (srcIt + 2 < src.end()
						&& measure.find(*(srcIt + 2)) != measure.end()) {
					replaceSpecWord(*(srcIt + 2));
					// measure word
					if (*(srcIt + 2) == "店") {
						temp = (*(srcIt + 1));
					} else {
						temp = (*(srcIt + 1)) + (*(srcIt + 2));
					}
					result.insert(temp);
					tempResultVec.push_back(temp);
					srcIt += 2;
				} else {
					temp = (*(srcIt + 1));
					result.insert(temp);
					tempResultVec.push_back(temp);
					srcIt += 1;
				}
			} else {
				tempVec.push_back(*srcIt);
				tempResultVec.push_back(*srcIt);
			}
		} else if (isAlpDig(*srcIt)) {
			if (srcIt + 1 < src.end()
					&& measure.find(*(srcIt + 1)) != measure.end()) {
				replaceSpecWord(*(srcIt + 1));
				if (*(srcIt + 1) == "店") {
					temp = (*srcIt);
				} else {
					temp = *srcIt + *(srcIt + 1);
				}
				result.insert(temp);
				tempResultVec.push_back(temp);
				srcIt += 1;
			} else {
				if (!isInBracket) {
					if (!startswith(srcStr, *srcIt)) {
						result.insert(*srcIt);
						tempResultVec.push_back(*srcIt);
					} else {
						tempVec.push_back(*srcIt);
						tempResultVec.push_back(*srcIt);
					}
				} else {
					//result.insert(*srcIt);
					tempVec.push_back(*srcIt);
					tempResultVec.push_back(*srcIt);
				}
			}
		} else {
			tempVec.push_back(*srcIt);
			tempResultVec.push_back(*srcIt);
		}
	}

	return 0;
}

//int mergeNumAlp(_vector &src, _vector &result, _hashset &measure) {
//	result.clear();
//	_vectorit srcIt = src.begin();
//	std::string temp("");
//	while(srcIt < src.end()) {
//		if ((*srcIt) == "第") {
//			if (srcIt + 1 < src.end() && len(*(srcIt + 1)) == (*(srcIt + 1)).length()){
//				// num
//				if(srcIt +2 < src.end() && measure.find(*(srcIt + 2)) != measure.end()){
//					// measure word
//					temp = (*srcIt) + (*(srcIt + 1)) + (*(srcIt + 2));
//					result.push_back(temp);
//					srcIt = src.erase(srcIt);
//					srcIt = src.erase(srcIt);
//					srcIt = src.erase(srcIt);
//					continue;
//				}else{
//					temp = (*srcIt) + (*(srcIt + 1));
//					result.push_back(temp);
//					srcIt = src.erase(srcIt);
//					srcIt = src.erase(srcIt);
//					continue;
//				}
//			}
//		} else if (len(*srcIt) == (*srcIt).length()) {
//			if (srcIt + 1 < src.end() && measure.find(*(srcIt + 1)) != measure.end()){
//				temp = (*srcIt) + (*(srcIt + 1));
//				result.push_back(temp);
//				srcIt = src.erase(srcIt);
//				srcIt = src.erase(srcIt);
//				continue;
//			}
//		}
//
//		srcIt++;
//	}
//
//	return 0;
//}

int strToPY(const std::string &src, _vector &result, const _mapVec &pinyin) {
	if (result.size() != 0)
		result.clear();
	_vector chWordList;
	split2word(src, chWordList);
	if (vecToPY(chWordList, result, pinyin) == -1)
		return -1;
	return 0;
}

int vecToPY(_vector &src, _vector &result, const _mapVec &pinyin) {
	if (result.size() != 0)
		result.clear();
	_vector temp, tempPY;
	_vectorit it = src.begin();
	_mapVec::const_iterator itMap;

	for (; it != src.end(); ++it) {
		temp.clear();
		itMap = pinyin.find(*it);
		if (itMap != pinyin.end())
			temp.assign(itMap->second.begin(), itMap->second.end());
		if (temp.size() == 0)
			temp.push_back(*it);
		mergeList(temp, result);
	}

	return 0;
}

//void out_vector1(_vector &v ){
//	std::cout<<"begin:";
//	for(_vectorit iter=v.begin();iter!=v.end();iter++){
//		std::cout<<*iter<<"++";
//	}
//	std::cout<<std::endl;
//}

bool pyIsSame(const std::string &a, const std::string &b, const _mapVec &pinyin,
		long int diffLen) {
	_vector av, bv;
	if ((len(a) <= 1 && len(b) > 1) || (len(a) > 1 && len(b) <= 1))
		return false;
	else if (diffLen != -1
			&& abs((long int) len(a) - (long int) len(b)) > diffLen)
		return false;

	if (strToPY(a, av, pinyin) == -1)
		return false;
	if (strToPY(b, bv, pinyin) == -1)
		return false;

	if (listIsSame(av, bv))
		return true;
	else
		return false;
}

/*
 int pyVecIsSame(_vector &a, _vector &b, CacheResult &ar, CacheResult &br,  _mapVec &pinyin)
 {
 _vectorit ait, bit;
 ait = a.begin();
 bit = b.begin();
 bool flag = false;

 while(ait != a.end())
 {
 flag = false;
 while(bit != b.end())
 {
 if(pyIsSame(*ait, *bit, pinyin))
 {
 br.nameSameList.push_back(*bit);
 b.erase(bit);
 flag = true;
 break;
 }
 bit++;
 }
 if(flag)
 {
 ar.nameSameList.push_back(*ait);
 ait = a.erase(ait);
 }
 else
 ++ait;
 }
 return 0;
 }
 */

int pyVecIsSame(_vector &a, _vector &b, _vector &ar, _vector &br,
		_mapVec &pinyin, _hashset &dpmark, long int diffLen) {
	_vectorit ait, bit;
	ait = a.begin();
	bool flag = false;

	while (ait != a.end()) {
		if (dpmark.find(*ait) != dpmark.end()) {
			++ait;
			continue;
		}
		flag = false;
		bit = b.begin();
		while (bit != b.end()) {
			if (dpmark.find(*bit) != dpmark.end()) {
				++bit;
				continue;
			}
			if (pyIsSame(*ait, *bit, pinyin, diffLen)) {
				br.push_back(*bit);
				b.erase(bit);
				flag = true;
				break;
			}
			++bit;
		}
		if (flag) {
			ar.push_back(*ait);
			ait = a.erase(ait);
		} else
			++ait;
	}

	return 0;
}

int pyVecIsSame(_vector &a, _vector &b, _vector &ar, _vector &br,
		_mapVec &pinyin, _hashset &dpmark, std::string &aObjName,
		std::string &bObjName, long int diffLen) {
	_vectorit ait, bit;
	ait = a.begin();
	bool flag = false;

	while (ait != a.end()) {
		if (dpmark.find(*ait) != dpmark.end() || (*ait) == aObjName) {
			++ait;
			continue;
		}
		flag = false;
		bit = b.begin();
		while (bit != b.end()) {
			if (dpmark.find(*bit) != dpmark.end() || (*bit) == bObjName) {
				++bit;
				continue;
			}
			if (pyIsSame(*ait, *bit, pinyin, diffLen)) {
				br.push_back(*bit);
				b.erase(bit);
				flag = true;
				break;
			}
			++bit;
		}
		if (flag) {
			ar.push_back(*ait);
			ait = a.erase(ait);
		} else
			++ait;
	}

	return 0;
}

int pyVecIsSame(_vector &a, _vector &b, _vector &ar, _vector &br,
		const _mapVec &pinyin, long int diffLen) {
	_vectorit ait, bit;
	ait = a.begin();
	bool flag = false;

	while (ait != a.end()) {
		flag = false;
		bit = b.begin();
		while (bit != b.end()) {
			if (pyIsSame(*ait, *bit, pinyin, diffLen)) {
				br.push_back(*bit);
				b.erase(bit);
				flag = true;
				break;
			}
			++bit;
		}
		if (flag) {
			ar.push_back(*ait);
			ait = a.erase(ait);
		} else
			++ait;
	}

	return 0;
}

int S2D(_vector& values, _map& dict) {
	int size = values.size();
	for (int i = 0; i < size; i++) {
		string& word = values[i];
		_mapit iter;
		iter = dict.find(word);
		if (iter != dict.end()) {
			values[i] = iter->second;
		}
	}
	return 0;
}

/*
 int removeBracket(_vector& values, bool only_bracket) {
 const string prefix="(";
 const string suffix=")";
 _vector::iterator iter=values.begin();
 _vector::iterator value_begin=values.begin();
 int count=0;
 while(iter!=values.end()){
 if(*iter==prefix){
 count++;
 }else if(*iter==suffix){
 if(count>0)
 count--;
 }else{
 if(count>0){
 if(only_bracket){
 *value_begin=*iter;
 value_begin++;
 }
 }else{
 *value_begin=*iter;
 value_begin++;
 }
 }
 iter++;
 }
 values.erase(value_begin,values.end());
 return 0;
 }
 */
int removeBracket(_vector &values, CacheResult &result) {
	const std::string prefix = "(";
	const std::string suffix = ")";
	size_t count = 0;
	_vectorit it = values.begin();
	_vectorit curit = it;
	result.inBracketList.clear();
	_vector temp;
	string tempStr("");
	while (it != values.end()) {
		if (*it == prefix) {
			++count;
			++it;
		} else if (*it == suffix) {
			if (count > 0) {
				--count;
				if (count == 0) {
					temp.assign(curit + 1, it);
					vec2str(temp, tempStr);
					temp.clear();
					result.inBracketList.push_back(tempStr);
					it = values.erase(curit, it + 1);
					continue;
				}
			} else {
				it = values.erase(curit, it + 1);
				continue;
			}
			++it;
		} else if (count > 0) {
			++it;
		} else {
			*curit++ = *it++;
		}
	}
	if (curit != it) {
		temp.assign(curit, it);
		vec2str(temp, tempStr);
		values.erase(curit, it);
		temp.clear();
		result.inBracketList.push_back(tempStr);
	}
	return 0;
}

int removeBracket(_vector &values, OriCacheResult &result) {
	const std::string prefix = "(";
	const std::string suffix = ")";
	size_t count = 0;
	_vectorit it = values.begin();
	_vectorit curit = it;
	result.inBracketList.clear();
	_vector temp;
	string tempStr("");
	while (it != values.end()) {
		if (*it == prefix) {
			++count;
			++it;
		} else if (*it == suffix) {
			if (count > 0) {
				--count;
				if (count == 0) {
					temp.assign(curit + 1, it);
					vec2str(temp, tempStr);
					temp.clear();
					result.inBracketList.push_back(tempStr);
					it = values.erase(curit, it + 1);
					continue;
				}
			} else {
				it = values.erase(curit, it + 1);
				continue;
			}
			++it;
		} else if (count > 0) {
			++it;
		} else {
			*curit++ = *it++;
		}
	}
	if (curit != it) {
		temp.assign(curit, it);
		vec2str(temp, tempStr);
		values.erase(curit, it);
		temp.clear();
		result.inBracketList.push_back(tempStr);
	}
	return 0;
}

int removePunc(_vector &values, bool needBra) {
	_vectorit it = values.begin();
	_vectorit curit = it;
	char *temp;
	unsigned short tempIt;
	while (it != values.end()) {
		tempIt = *(unsigned short *) (*it).c_str();
//		if ((tempIt >= 0X40A1 && tempIt <= 0XA1A1)
//				|| (tempIt >= 0X40A2 && tempIt <= 0XA0A2)
//				|| (tempIt >= 0X40A3 && tempIt <= 0XA0A3)
//				|| (tempIt >= 0X40A4 && tempIt <= 0XA0A4)
//				|| (tempIt >= 0X40A5 && tempIt <= 0XA0A5)
//				|| (tempIt >= 0X40A6 && tempIt <= 0XA0A6)
//				|| (tempIt >= 0X40A7 && tempIt <= 0XA0A7)) {
//			++it;
//			continue;
//		}

		temp = NULL;
		if (needBra)
			temp = (char *) memchr(" \t\r\n\\-_[]{}:.,=*&^%$#@!~?<>/|'\"",
					(*it)[0], 31);
		else
			temp = (char *) memchr(" \t\r\n\\-_[](){}:.,=*&^%$#@!~?<>/|'\"",
					(*it)[0], 33);
//		if((*it) == "《" || (*it) == "》"){
//			*temp = 1;
//		}

		if (temp == NULL)
			*curit++ = *it++;
		else {
			//*curit = " ";
			++it;
			//++curit;
		}
	}

	values.erase(curit, values.end());

	return 0;
}

int formatSize(_vector &values, bool to_upper) {
	for (_vectorit iter = values.begin(); iter != values.end(); ++iter) {
		if ((*iter).length() == 1) {
			char c = (*iter)[0];
			if (to_upper) {
				*iter = toupper(c);
			} else {
				*iter = tolower(c);
			}
		}
	}
	return 0;
}

int vec2str(_vector &values, std::string &result, bool del) {
	int dwPos = 0;
	result.clear();
	char g_szStr[10 * 1024];
	g_szStr[0] = 0;
	_vectorit it = values.begin();

//string needDel = " \t\r\n";
	for (; it != values.end() && dwPos < 99 * 1024; ++it) {
		//	if(del && needDel.find(*it) != std::string::npos) continue;
		//else
		memcpy(g_szStr + dwPos, it->c_str(), it->size());
		dwPos += it->size();
		//result += *it;
	}
	g_szStr[dwPos] = 0;
	result = g_szStr;
	return 0;
}

/*
 int filte(_vector &src, CacheResult &result, _hashset &filteTable)
 {
 _vectorit it = src.begin();
 _vectorit curit = it;
 //result.nameDiffFilteList.clear();
 while(it != src.end())
 {
 if(filteTable.find(*it) != filteTable.end())
 {
 result.nameDiffFilteList.push_back(*it);
 ++it;
 }
 else
 *curit++ = *it++;
 }
 src.erase(curit, src.end());
 return 0;
 }
 */

//int filte(_vector &src, _vector &result, _hashset &filteTable) {
//	_vectorit it = src.begin();
//	_vectorit curit = it;
////result.nameDiffFilteList.clear();
//	while (it != src.end()) {
//		if (filteTable.find(*it) != filteTable.end()) {
//			result.push_back(*it);
//			++it;
//		} else
//			*curit++ = *it++;
//	}
//	src.erase(curit, src.end());
//	return 0;
//}
int filte(_vector &src, _vector &result, const _set &filteTable) {
	_vectorit it = src.begin();
	_vectorit curit = it;
//result.nameDiffFilteList.clear();
	while (it != src.end()) {
		if (filteTable.find(*it) != filteTable.end()) {
			result.push_back(*it);
			++it;
		} else
			*curit++ = *it++;
	}
	src.erase(curit, src.end());
	return 0;
}

int filteIfNotNull(_vector &src, _vector &result, const _set &filteTable) {
	vector<string> temp_in_vec;
	vector<string> temp_out_vec;

	if (src.size() > 0) {
		temp_in_vec = src;
		temp_out_vec = result;

		filte(src, result, filteTable);

		if (src.size() == 0) {
			// 西单大悦城  魅族（西单大悦城店）
			src = temp_in_vec;
			result = temp_out_vec;
		}
	}

	return 0;
}
/*
 int filte(_vector &src, _set &filteTable)
 {
 _vectorit it = src.begin();
 while(it != src.end())
 {
 if(filteTable.find(*it) != filteTable.end())
 {
 it = src.erase(it);
 }
 else
 it++;
 }
 return 0;
 }
 */

/*
 int filte(_vector &aDiff, CacheResult &a, CacheResult &b)
 {
 _vectorit it = aDiff.begin();
 _vectorit bit;
 bool flag = false;
 while(it != aDiff.end())
 {
 flag = false;
 for(bit = b.inBracketList.begin(); bit != b.inBracketList.end(); ++bit)
 {
 if(wordIsSame(*it, *bit))
 {
 flag = true;
 break;
 }
 }
 if(flag)
 {
 a.nameSameList.push_back(*it);
 it = aDiff.erase(it);
 }
 else
 ++it;
 }
 return 0;
 }
 */

int filte(_vector &aDiff, _vector &a, _vector &bInBracket) {
	_vectorit it = aDiff.begin();
	_vectorit bit;
	size_t len1, len2;
	bool flag = false;
	while (it != aDiff.end()) {
		flag = false;
		for (bit = bInBracket.begin(); bit != bInBracket.end(); ++bit) {
			len1 = len(*it);
			len2 = len(*bit);
			if (abs((long int) len1 - (long int) len2) < 2
					&& (len1 != (*it).length() || len2 != (*bit).length()))
				continue;
			if (wordIsSame(*it, *bit)) {
				flag = true;
				break;
			}
		}
		if (flag) {
			a.push_back(*it);
			it = aDiff.erase(it);
		} else
			++it;
	}
	return 0;
}

int filte(_vector &aDiff, _vector &a, const _vector &bInBracket,
		const _mapVec &pinyin) {
	_vectorit it = aDiff.begin();
	_vector::const_iterator bit;
	bool flag = false;
	while (it != aDiff.end()) {
		flag = false;
		for (bit = bInBracket.begin(); bit != bInBracket.end(); ++bit) {
			if (pyIsSame(*it, *bit, pinyin)) {
				flag = true;
				break;
			}
		}
		if (flag) {
			a.push_back(*it);
			it = aDiff.erase(it);
		} else
			++it;
	}
	return 0;
}

int filteIfNotNull(_vector &src, _vector &result, const _vector &filterVec,
		const _mapVec &pinyin) {
	vector<string> temp_in_vec;
	vector<string> temp_out_vec;

	if (src.size() > 0) {
		temp_in_vec = src;
		temp_out_vec = result;

		filte(src, result, filterVec, pinyin);

		if (src.size() == 0) {
			// 西单大悦城  魅族（西单大悦城店）
			src = temp_in_vec;
			result = temp_out_vec;
		}
	}

	return 0;
}

int filteEndWith(_vector &src, _vector &result, _hashset &endList,
		_hashset &undel) {
	if (src.size() >= 1) {
		_vectorit it = src.begin();
		if (src.size() == 1 && len(*it) > 4)
			return 0;

		string temp = "";
		while (it != src.end()) {
			if (len(*it) <= 4 && (*it).size() > 2) {
				temp = (*it).substr((*it).size() - 2, 2);
				if (endList.find(temp) != endList.end()
						&& undel.find(*it) == undel.end()) {
					//result.push_back(*it);
					it = src.erase(it);
				} else
					++it;
			} else
				++it;
		}
	}
	return 0;
}

/*
 int lcsSplit(CacheResult &src, std::string &delim, Resource &resource)
 {
 _vector temp, tempWS;
 _vectorit it;
 size_t count = 1;
 if(split(src.formatName, delim, temp) == -1) return -1;
 for(it = temp.begin(); it != temp.end(); ++it, ++count)
 {
 if(len(*it) <= 3)
 {
 if(count % 2 == 1)
 {
 src.nameLCSWordList.push_back(*it);
 src.nameLCSWordList.push_back(delim);
 }
 else
 src.nameLCSWordList.push_back(*it);
 }
 else
 {
 if(resource.ws.split(*it, tempWS) == -1) return -1;
 if(count % 2 == 1)
 {
 src.nameLCSWordList.assign(tempWS.begin(), tempWS.end());
 src.nameLCSWordList.push_back(delim);
 }
 else
 src.nameLCSWordList.assign(tempWS.begin(), tempWS.end());
 }
 }

 src.nameLCSWordSet.insert(src.nameLCSWordList.begin(), src.nameLCSWordList.end());
 return 0;
 }
 */

int lcsSplit(std::string &src, std::string &delim, _vector &result,
		WordSplit &ws) {
	_vector temp, tempWS, tempLCSList;
	_vectorit it;
	size_t tempLen = 0;
	if (len(delim) > 3) {
		if (ws.split(delim, tempLCSList) == -1)
			return -1;
	} else
		tempLCSList.push_back(delim);

	if (split(src, delim, temp) == -1)
		return -1;
	for (it = temp.begin(); it != temp.end(); ++it) {
		tempLen = len(*it);
		if (tempLen <= 3) {
			if (tempLen > 0) {
				result.push_back(*it);
			}
		} else {
			if (ws.split(*it, tempWS) == -1)
				return -1;
			result.insert(result.end(), tempWS.begin(), tempWS.end());
		}
		result.insert(result.end(), tempLCSList.begin(), tempLCSList.end());
	}

	for (size_t i = 0; i < tempLCSList.size(); ++i)
		result.erase(result.end());

	return 0;
}

/*
 int lcsSplit(std::string &src, std::string &delim, _vector &result,
 WordSplit &ws) {
 _vector temp, tempWS, tempLCSList;
 _vectorit it;
 size_t tempLen = 0;

 if (split(src, delim, temp) == -1)
 return -1;
 for (it = temp.begin(); it != temp.end(); ++it) {
 tempLen = len(*it);
 if (tempLen <= 2) {
 if (tempLen > 0) {
 result.push_back(*it);
 }
 } else {
 if (ws.split(*it, tempWS) == -1)
 return -1;
 result.insert(result.end(), tempWS.begin(), tempWS.end());
 }
 result.push_back(delim);
 }

 result.erase(result.end());

 return 0;
 }
 */

int mergeWord(_vector &src, _vector &result) {
	result.clear();
	_vectorit it = src.begin();
	string temp("");
	size_t length = 0;
	for (; it != src.end(); ++it) {
		length = len(*it);
		if (length == 1 && length != (*it).size())
			temp += *it;
		else {
			if (temp.size() != 0) {
				result.push_back(temp);
				temp.clear();
			}
			result.push_back(*it);
		}
	}

//	if (temp.size() != 0) {
//		size_t l = len(temp);
//		if (temp.size() != l && l == 1
//				&& (result.size() > 1
//						|| (result.size() == 1 && len(*result.rbegin()) < 4))) {
//			string last = *(result.rbegin());
//			if (len(last) != last.length()) {
//				string tail = *(result.rbegin());
//				tail += temp;
//				result.erase(result.end());
//				result.push_back(tail);
//			} else
//				result.push_back(temp);
//		} else
//			result.push_back(temp);
//	}

	if (temp.size() != 0) {
		result.push_back(temp);
	}

	return 0;
}

int mergeWord(_vector &src, _vector &result, const Resource &resource) {
	result.clear();
	_vectorit it = src.begin();
	string temp("");
	size_t length = 0;

	if (src.size() > 1) {
		if (len(*it) == 1 && len(*(it + 1)) > 1) {
			result.push_back(*it + *(it + 1));
			it += 2;
		}
	}

	for (; it != src.end(); ++it) {
		length = len(*it);
		if (resource.number2.find(*it) != resource.number2.end()) {
			result.push_back(*it);
		} else if (length == 1 && length != (*it).size()) {
			temp += *it;
		} else {
			if (temp.size() != 0) {
				result.push_back(temp);
				temp.clear();
			}
			result.push_back(*it);
		}
	}

	if (temp.size() != 0) {
		result.push_back(temp);
	}

	return 0;
}

int transWord(_vector &src, _vector &result, _map &transMap) {
	result.clear();
	_vectorit it = src.begin();
	_mapit transIt;
	for (; it != src.end(); ++it) {
		transIt = transMap.find(*it);
		if (transIt != transMap.end()) {
			result.push_back(transIt->second);
		} else
			result.push_back(*it);
	}
	return 0;
}

//int transWord(_vector &src, _map &transMap) {
//	_vectorit it = src.begin();
//	_mapit transIt;
//	int index = 0;
//	while (it != src.end()) {
//		if (*it != "") {
//			transIt = transMap.find(*it);
//			if (transIt != transMap.end()) {
//				src[index] = transIt->second;
//			}
//		}
//		++it;
//		++index;
//	}
//
//	return 0;
//}

int transWord(_vector &src, const _map &transMap) {
	_map::const_iterator transIt;
	for (int i = 0; i < src.size(); ++i) {
		if (src[i] != "") {
			transIt = transMap.find(src[i]);
			if (transIt != transMap.end()) {
				src[i] = transIt->second;
			}
		}
	}

	return 0;
}

int getTransName(_set &src, _set &result, _map &transMap) {
	_setit it = src.begin();
	_mapit itres;
	for (; it != src.end(); ++it) {
		itres = transMap.find(*it);
		if (itres != transMap.end()) {
			result.insert(itres->second);
		}
	}

	return 0;
}

int getTransType(_vector &src, _set &result, _map &transMap) {
	_vectorit it = src.begin();
	_mapit itres;
	for (; it != src.end(); ++it) {
		itres = transMap.find(*it);
		if (itres != transMap.end()) {
			result.insert(itres->second);
		}
	}

	return 0;
}

int getLeftList(_vector &src, _set &result, _map &transMap) {
	_vectorit it = src.begin();
	_mapit itmap;
	while (it != src.end()) {
		itmap = transMap.find(*it);
		if (itmap != transMap.end()) {
			result.insert(itmap->second);
			it = src.erase(it);
		} else
			++it;
	}

	return 0;
}

int findObjName(_vector &src, std::string &objName, double &objMaxDistance,
		_mapStrDbl &objMap) {
	std::string tempObjName("");
	double tempDistance = 1000.0;
	_mapStrDblit distance_it;
	_vectorit it = src.begin();
	for (; it != src.end(); ++it) {
		distance_it = objMap.find(*it);
		if (distance_it != objMap.end()) {
			if (tempObjName == "") {
				tempObjName = distance_it->first;
				tempDistance = distance_it->second;
			} else {
				return 0;
			}
		}
	}

	if (tempObjName != "") {
		objName = tempObjName;
		objMaxDistance = tempDistance;
	}
	return 0;
}

int findObjName(CacheResult &src, Resource &resource) {
	std::string tempObjName("");
	double tempDistance = 1000.0;
	_mapStrDblit distance_it;
	_mapit type_it;

	_vectorit it = src.name.nameWordList.begin();
	for (; it != src.name.nameWordList.end(); ++it) {
		distance_it = resource.objNameMap.find(*it);
		if (distance_it != resource.objNameMap.end()) {
//			if (tempObjName == "") {
//				tempObjName = distance_it->first;
//				tempDistance = distance_it->second;
//			} else {
//				// if have more obj name, return
//				if (distance_it->first != tempObjName) {
//					return 0;
//				}
//			}

			tempObjName = distance_it->first;
			tempDistance = distance_it->second;
		}
	}

	type_it = resource.objTypeMap.find(tempObjName);
	if (tempObjName != ""
			&& (type_it == resource.objTypeMap.end()
					|| startswith(src.type, type_it->second)
					|| startswith(src.sosoAttr.soso_type, type_it->second))) {
		src.objName = tempObjName;
		src.objMaxDistance = tempDistance;
	}
	return 0;
}

int findObjName(OriCacheResult &src, Resource &resource) {
	std::string tempObjName("");
	double tempDistance = 1000.0;
	std::string tempObjType("");
	_mapStrDblit distance_it;
	_mapit type_it;

	_vectorit it = src.name.nameWordList.begin();
	for (; it != src.name.nameWordList.end(); ++it) {
		distance_it = resource.objNameMap.find(*it);
		if (distance_it != resource.objNameMap.end()
				&& startswith(src.type, resource.objTypeMap[*it])) {
			tempObjName = distance_it->first;
			tempDistance = distance_it->second;

		}
	}

	type_it = resource.objTypeMap.find(tempObjName);
	if (tempObjName != ""
			&& (type_it == resource.objTypeMap.end()
					|| startswith(src.type, type_it->second)
					|| startswith(src.soso_type, type_it->second))) {
		src.objName = tempObjName;
		src.objMaxDistance = tempDistance;
	}
	return 0;
}

//int filte(_vector &src, _hashset &filter) {
//	_vectorit it = src.begin();
//	_hashsetit itres;
//	while (it != src.end()) {
//		itres = filter.find(*it);
//		if (itres != filter.end()) {
//			if (len(*it) == 1)
//				it = src.erase(it);
//			else
//				++it;
//		} else
//			++it;
//	}
//	return 0;
//}

int filte(_vector &src, const _hashset &filter) {
	_vectorit it = src.begin();
	_hashsetit itres;
	while (it != src.end()) {
		itres = filter.find(*it);
		if (itres != filter.end()) {
			it = src.erase(it);
		} else
			++it;
	}
	return 0;
}

int filte(_vector &src, _vector &filter) {
	_vectorit srcit, filterit;
	srcit = src.begin();
	bool flag = false;
	while (srcit != src.end()) {
		flag = false;
		for (filterit = filter.begin(); filterit != filter.end(); ++filterit) {
			if (wordIsSame(*srcit, *filterit, 2)) {
				flag = true;
				break;
			}
		}
		if (flag) {
			srcit = src.erase(srcit);
		} else {
			++srcit;
		}
	}
	return 0;
}

void filte(_vector &src, string &filter) {
	if (filter == "") {
		return;
	}

	_vectorit srcit = src.begin();
	while (srcit != src.end()) {
		if (wordIsSame(*srcit, filter, 2)) {
			srcit = src.erase(srcit);
		} else {
			++srcit;
		}
	}
}

int filte(_vector &src, _vector &filter, Resource &resource, int &count) {
	_vectorit srcit, filterit;
	srcit = src.begin();
	bool flag = false;
	while (srcit != src.end()) {
		flag = false;
		for (filterit = filter.begin(); filterit != filter.end(); ++filterit) {
			if (wordIsSame(*srcit, *filterit, 2)) {
				flag = true;
				break;
			}
		}
		if (flag) {
			if (resource.typeword["01"].find(*srcit)
					== resource.typeword["01"].end()) {
				if (count < 2) {
					++srcit;
				} else {
					srcit = src.erase(srcit);
					count--;
				}
			} else {
				srcit = src.erase(srcit);
			}
		} else {
			++srcit;
		}
	}
	return 0;
}

void filte(_vector &src, string &filter, const _mapSet &typeword, int &count) {
	if (filter != "") {
		_mapSet::const_iterator it = typeword.find("01");
		if (it != typeword.end()) {
			_vectorit srcit = src.begin();
			while (srcit != src.end()) {
				if (wordIsSame(*srcit, filter, 2)) {
					if (it->second.find(*srcit) == it->second.end()) {
						if (count < 2) {
							++srcit;
						} else {
							srcit = src.erase(srcit);
							count--;
						}
					} else {
						srcit = src.erase(srcit);
					}
				} else {
					++srcit;
				}
			}
		}
	}
}

int filteDistrict(_vector &src, std::string &addr, const _hashset &xzqh_set) {
	_vectorit srcit;
	srcit = src.begin();
	bool flag = false;
	string tempTail("");
	string tail = "县乡村镇";
	while (srcit != src.end()) {
		if (isDigit(*srcit)) {
			++srcit;
			continue;
		}
		flag = false;
		if ((*srcit).length() < 2) {
			++srcit;
			continue;
		}
		tempTail = (*srcit).substr((*srcit).length() - 2, 2);
		if (tail.find(tempTail) != std::string::npos
				&& xzqh_set.find(*srcit) != xzqh_set.end()) {
			flag = true;
		} else if (addr.find(*srcit) != std::string::npos
				&& xzqh_set.find(*srcit) != xzqh_set.end()) {
			flag = true;
		}

		if (flag) {
			srcit = src.erase(srcit);
		} else {
			++srcit;
		}
	}
	return 0;
}

int filteDistrict(_vector &src, std::string &addr, _hashset &xzqh_set,
		Resource &resource, int &count) {
	_vectorit srcit;
	srcit = src.begin();
	bool flag = false;
	string tempTail("");
	string tail = "县乡村镇";
	while (srcit != src.end()) {
		if (isDigit(*srcit)) {
			++srcit;
			continue;
		}
		flag = false;
		if ((*srcit).length() < 2) {
			++srcit;
			continue;
		}
		tempTail = (*srcit).substr((*srcit).length() - 2, 2);
		if (tail.find(tempTail) != std::string::npos
				&& xzqh_set.find(*srcit) != xzqh_set.end()) {
			flag = true;
		} else if (addr.find(*srcit) != std::string::npos
				&& xzqh_set.find(*srcit) != xzqh_set.end()) {
			flag = true;
		}

		if (flag) {
			if (resource.typeword["01"].find(*srcit)
					== resource.typeword["01"].end()) {
				if (count < 2) {
					++srcit;
				} else {
					srcit = src.erase(srcit);
					count--;
				}
			} else {
				srcit = src.erase(srcit);
			}
		} else {
			++srcit;
		}
	}
	return 0;
}

void filteDistrict(_vector &src, std::string &addr, const _hashset &xzqh_set,
		const _mapSet &typeword, int &count) {
	_mapSet::const_iterator it = typeword.find("01");
	if (it != typeword.end()) {
		_vectorit srcit;
		srcit = src.begin();
		bool flag = false;
		string tempTail("");
		string tail = "县乡村镇";
		while (srcit != src.end()) {
			if (isDigit(*srcit)) {
				++srcit;
				continue;
			}
			flag = false;
			if ((*srcit).length() < 2) {
				++srcit;
				continue;
			}
			tempTail = (*srcit).substr((*srcit).length() - 2, 2);
			if (tail.find(tempTail) != std::string::npos
					&& xzqh_set.find(*srcit) != xzqh_set.end()) {
				flag = true;
			} else if (addr.find(*srcit) != std::string::npos
					&& xzqh_set.find(*srcit) != xzqh_set.end()) {
				flag = true;
			}

			if (flag) {
				if (it->second.find(*srcit) == it->second.end()) {
					if (count < 2) {
						++srcit;
					} else {
						srcit = src.erase(srcit);
						count--;
					}
				} else {
					srcit = src.erase(srcit);
				}
			} else {
				++srcit;
			}
		}
	}
}

int findMark(_vector &src, int &result, _hashset &markSet) {
	result = 0;
	_vectorit it = src.begin();
	for (; it != src.end(); ++it) {
		if (markSet.find(*it) != markSet.end()) {
			result = 1;
			break;
		}
	}
	return 0;
}

int findMark(_vector &src, _set &result, _hashset &markSet) {
	_vectorit it = src.begin();
	for (; it != src.end(); ++it) {
		if (markSet.find(*it) != markSet.end()) {
			result.insert(*it);
		}
	}
	return 0;
}

int findMark(_vector &a, _vector &b, SimScore &score, int &result,
		_hashset &markSet, bool oneNullDiff, bool oneDiff) {

	int distance = 250;
	_set inbraSame, aMark, bMark;
	if (findMark(a, aMark, markSet) == -1)
		return -1;
	if (findMark(b, bMark, markSet) == -1)
		return -1;

	if (aMark.size() > 0 && bMark.size() > 0) {
		result = -1;
		set_intersection(aMark.begin(), aMark.end(), bMark.begin(), bMark.end(),
				insert_iterator<_set>(inbraSame, inbraSame.begin()));
		if ((inbraSame.size() == aMark.size())
				&& (aMark.size() == bMark.size()))
			result = 0;
		else if (oneDiff && score.distance <= distance)
			result = 0;
	} else if (aMark.size() > 0 || bMark.size() > 0) {
		if (a.size() > 0 && b.size() > 0 && oneDiff
				&& score.distance > distance)
			result = -1;
		else if (oneNullDiff && score.distance > distance)
			result = -1;
		else
			result = 0;
	}

	return 0;
}

int filteCount(_vector &src, _vector &result, _hashset &filter, size_t Count) {
	_vectorit it;
	size_t i = 0;
	for (it = src.begin(); it != src.end(); ++it) {
		if (filter.find(*it) != filter.end())
			++i;
	}

	if (i >= Count) {
		it = src.begin();
		while (it != src.end()) {
			if (filter.find(*it) != filter.end()) {
				result.push_back(*it);
				it = src.erase(it);
			} else
				++it;
		}
	}

	return 0;
}

int filteMarkDistrict(_vector &src, _vector &result, _hashset &filter) {
	_vectorit it = src.begin();
	char * pcTemp;
	while (it != src.end()) {
		if (filter.find(*it) != filter.end()) {
			pcTemp = (char*) it->c_str();
			pcTemp += (it->length() - 2);

			if (*(short*) pcTemp == *(short*) "省"
					|| *(short*) pcTemp == *(short*) "市"
					|| *(short*) pcTemp == *(short*) "县"
					|| *(short*) pcTemp == *(short*) "乡"
					|| *(short*) pcTemp == *(short*) "区"
					|| *(short*) pcTemp == *(short*) "村"
					|| *(short*) pcTemp == *(short*) "桥"
					|| *(short*) pcTemp == *(short*) "湖"
					|| *(short*) pcTemp == *(short*) "江"
					|| *(short*) pcTemp == *(short*) "河"
					|| *(short*) pcTemp == *(short*) "道"
					|| *(short*) pcTemp == *(short*) "街"
					|| *(short*) pcTemp == *(short*) "路"
					|| *(short*) pcTemp == *(short*) "巷"
					|| *(short*) pcTemp == *(short*) "镇")
					//endswith((*it), "省") ||
					//endswith((*it), "市") ||
					//endswith((*it), "县") ||
					//endswith((*it), "乡") ||
					//endswith((*it), "区") ||
					//endswith((*it), "村") ||
					//endswith((*it), "桥") ||
					//endswith((*it), "湖") ||
					//endswith((*it), "江") ||
					//endswith((*it), "河") ||
					//endswith((*it), "道") ||
					//endswith((*it), "街") ||
					//endswith((*it), "路") ||
					//endswith((*it), "巷") ||
					//endswith((*it), "镇"))
							{
				//result.push_back(*it);
				it = src.erase(it);
			} else
				++it;
		} else
			++it;
	}
	return 0;
}

bool isNumMeasure(std::string &src, _hashset &filter) {
//if(src == "会所")
//	return false;

	_vector temp;
	if (split2word(src, temp) == -1)
		return false;
	_vectorit it = temp.begin();
	for (; it != temp.end(); ++it) {
		if (len(*it) == (*it).length() || filter.find(*it) != filter.end())
			continue;
		else
			return false;
	}
	return true;
}

int singleZhCount(_vector &src, size_t &count) {
	count = 0;
	_vectorit it = src.begin();
	for (; it != src.end(); ++it) {
		size_t l = len(*it);
		if (l != (*it).length() && l == 1)
			++count;
	}
	return 0;
}

int getTailType(_vector &src, string &type, size_t tailLen, _map &transMap) {
	type = "";
	_mapit transit;
	std::vector<std::string>::reverse_iterator it = src.rbegin();
	size_t count = 0;
	while (it != src.rend() && count < tailLen) {
		transit = transMap.find(*it);
		if (transit != transMap.end()) {
			type = transit->second;
			break;
		}
		++it;
		++count;
	}
	return 0;
}

//bool wordInclude(std::string &a, std::string &b, const _hashset &endList) {
//	if (a.length() > 2 && b.length() > 2) {
//		string at, bt;
//		size_t la, lb;
//		at = a.substr(a.size() - 2, 2);
//		bt = b.substr(b.size() - 2, 2);
//
//		if (at == bt && endList.find(at) != endList.end()) {
//			at = a.substr(0, a.size() - 2);
//			bt = b.substr(0, b.size() - 2);
//			la = len(at);
//			lb = len(bt);
//
//			if (la == 1 || lb == 1)
//				return false;
//			else if (abs((long int) la - (long int) lb) > 2)
//				return false;
//			else if (wordIsSame(at, bt))
//				return true;
//			else
//				return false;
//		} else {
//			return false;
//		}
//	} else {
//		return false;
//	}
//}

bool wordInclude(std::string &a, std::string &b, const _hashset &endList) {
	if (a.length() > 2 && b.length() > 2) {
		string at, bt;
		size_t la, lb;
		at = a.substr(a.size() - 2, 2);
		bt = b.substr(b.size() - 2, 2);
		bool isEndA = false, isEndB = false;
		if(endList.find(at) != endList.end()){
			at = a.substr(0, a.size() - 2);
		} else{
			at = a;
		}

		if(endList.find(bt) != endList.end()){
			bt = b.substr(0, a.size() - 2);
		} else{
			bt = b;
		}

		la = len(at);
		lb = len(bt);

		if (la == 1 || lb == 1)
			return false;
		else if (abs((long int) la - (long int) lb) > 2)
			return false;
		else if (wordIsSame(at, bt))
			return true;
		else
			return false;
	} else {
		return false;
	}
}

//
int filteInclude(_vector &a, _vector &b, _vector &ar, _vector &br,
		const _hashset &endList) {
	if(a.size() == 1 || b.size() == 1){
		return 0;
	}

	_vectorit ait, bit;
	ait = a.begin();
	bool flag = false;

	while (ait != a.end()) {
		flag = false;
		bit = b.begin();
		while (bit != b.end()) {
			if (wordInclude(*ait, *bit, endList)) {
				br.push_back(*bit);
				bit = b.erase(bit);
				flag = true;
				break;
			} else {
				++bit;
			}
		}
		if (flag) {
			ar.push_back(*ait);
			ait = a.erase(ait);
		} else
			++ait;
	}

	return 0;
}

int transNum(_vector &src) {
	_vectorit it = src.begin();
	string temp("");
	for (; it != src.end(); ++it) {
		if (startswith((*it), "no") || startswith((*it), "n0")) {
			temp = (*it).substr(2, temp.length() - 2);
			if (isDigit(temp)) {
				while (temp.length() > 0) {
					if (startswith(temp, "0")) {
						temp = temp.substr(1, temp.length() - 1);
					} else {
						break;
					}
				}
				*it = temp;
			}
		} else if (isDigit(*it)) {
			temp = *it;
			while (temp.length() > 0) {
				if (startswith(temp, "0")) {
					temp = temp.substr(1, temp.length() - 1);
				} else {
					break;
				}
			}
			*it = temp;
		}
	}

	return 0;
}

int transZhNum(_vector &src, _hashset &number, _hashset &number2, int length) {
	string temp(""), num("");
	string result("");
	bool flag = false;
	_vectorit it;
	it = src.begin();
	for (; it != src.end(); ++it) {
		if (!flag) {
			if (number2.find(*it) != number2.end()) {
				temp += *it;
				flag = true;
			} else {
				result += *it;
			}
		} else {
			if (number.find(*it) != number.end()) {
				temp += *it;
			} else {
				if (temp != "" && len(temp) > length) {
					num = Decapitalized(temp);
					flag = false;
					if (num != "")
						result += num;
					else
						result += temp;
					temp = "";
				} else {
					result += temp;
					temp = "";
				}
				result += *it;
			}
		}
	}

	if (temp != "" && len(temp) > length) {
		num = Decapitalized(temp);
		flag = false;
		if (num != "")
			result += num;
		else
			result += temp;
	} else {
		result += temp;
	}

	if (split2word2(result, src) == -1)
		return -1;

	return 0;
}

int transZhNum(std::string &src, std::string &result, _hashset &number,
		_hashset &number2) {
	result = "";
	_vector srcVec;
	split2word2(src, srcVec);

	string temp(""), num("");
	bool flag = false;
	_vectorit it;
	it = srcVec.begin();
	for (; it != srcVec.end(); ++it) {
		if (!flag) {
			if (number2.find(*it) != number2.end()) {
				temp += *it;
				flag = true;
			} else {
				result += *it;
			}
		} else {
			if (number.find(*it) != number.end()) {
				temp += *it;
			} else {
				if (temp != "" && len(temp) > 1) {
					num = Decapitalized(temp);
					flag = false;
					if (num != "")
						result += num;
					else
						result += temp;
					temp = "";
				} else {
					result += temp;
					temp = "";
				}
				result += *it;
			}
		}
	}

	if (temp != "") {
		num = Decapitalized(temp);
		if (num != "")
			result += num;
		else
			result += temp;
	}

	return 0;
}

int transZhNum(std::string &src, std::string &result, _hashset &number,
		_hashset &number2, int length) {
	result = "";
	_vector srcVec;
	split2word2(src, srcVec);

	transZhNum(srcVec, number, number2, length);

	vec2str(srcVec, result);

	return 0;
}

int transZhNum(_vector &src, _vector &result, _hashset &number,
		_hashset &number2) {
	result.clear();
	_vectorit src_it = src.begin();
	std::string tempStr("");
	for (; src_it != src.end(); ++src_it) {
		transZhNum(*src_it, tempStr, number, number2);
		if (tempStr != "") {
			result.push_back(tempStr);
		}
	}

	return 0;
}

int transZhNum(_set &src, _hashset &number, _hashset &number2, int length) {
	_setit src_it = src.begin();
	_set result;
	std::string srcStr("");
	std::string tempStr("");
	for (; src_it != src.end(); ++src_it) {
		srcStr = *src_it;
		transZhNum(srcStr, tempStr, number, number2, length);
		if (tempStr != "") {
			result.insert(tempStr);
		}
	}

	src = result;

	return 0;
}

int delfix(std::string &src, std::string &result, std::string &fix,
		const std::string &type) {
	result = "";
	int dwRet = 0;
	if (src.size() >= fix.size()) {
		if (type == "prefix") {
			if (startswith(src, fix)) {
				result = src.substr(fix.size(), src.size() - fix.size());
				dwRet = 1;
			}
		} else if (type == "suffix") {
			if (endswith(src, fix)) {
				result = src.substr(0, src.size() - fix.size());
				dwRet = 1;
			}
		}
	}

	if (result == "") {
		dwRet = 0;
		result = src;
	}

	return dwRet;
}

bool tailIsSame(const std::string &str1, const std::string &str2, int length) {
	if (str1.length() < length || str2.length() < length) {
		return false;
	}

	if (str1.substr(str1.length() - length, length)
			== str2.substr(str2.length() - length, length)) {
		return true;
	}

	return false;
}

int splitType(const std::string &src, _vector &result) {
	result.clear();
	string temp("");
	if (src.length() % 2 == 0) {
		for (int i = 0; i < src.length(); i = i + 2) {
			temp += src.substr(i, 2);
			result.push_back(temp);
		}
	}
	return 0;
}

int splitType2(const std::string &src, _vector &result) {
	result.clear();
	string temp("");
	if (src.length() % 2 == 0) {
		for (int i = 0; i < src.length(); i = i + 2) {
			result.push_back(src.substr(i, 2));
		}
	}
	return 0;
}

//int findSoSoType(CacheResult &src, _map &sosoTypeMap) {
//	src.sosoAttr.clear();
//	string result("");
//	size_t strLen = src.formatName.length();
//	char *p = (char*) src.formatName.c_str();
//	size_t i = 0, j = 0;
//	int temp = 0;
//	char * pcEnd = (char *) src.formatName.c_str() + strLen;
//	_mapit it;
//	while (*p != '\0' && p < pcEnd) {
//		result = src.formatName.substr(i, strLen - i);
//		it = sosoTypeMap.find(result);
//		if (it != sosoTypeMap.end()) {
//			// 找到后缀
//			src.sosoAttr.shortName = src.formatName.substr(0, i);
//			src.sosoAttr.suffix = result;
//			src.sosoAttr.soso_type = it->second;
//			if (src.sosoAttr.soso_type.size() == 6) {
//				src.sosoAttr.soso_main_type = src.sosoAttr.soso_type.substr(0,
//						2);
//			}
//			break;
//		}
//
//		if (*p >= 0) {
//			i++;
//			p++;
//		} else {
//			p += 2;
//			i += 2;
//		}
//	}
//	return 0;
//}

//// 短词后缀
//int findTailType(std::string &srcStr, SoSoAttr &result, _map &sosoTypeMap) {
//	string temp("");
//	size_t strLen = srcStr.length();
//	_mapit it;
//
//	for (int i = 1; i < strLen; i++) {
//		if (srcStr.at(strLen - i - 1) < 0) {
//			i++;
//		}
//		temp = srcStr.substr(strLen - i, i);
//
//		it = sosoTypeMap.find(temp);
//		if (it != sosoTypeMap.end()) {
//			// 找到后缀
//			result.shortName = srcStr.substr(0, strLen - i);
//			result.suffix = temp;
//			result.soso_type = it->second;
//			if (result.soso_type.size() == 6) {
//				result.soso_main_type = result.soso_type.substr(0, 2);
//			}
//			break;
//		}
//	}
//}

// 长词后缀
int findTailType(const std::string &srcStr, SoSoAttr &result,
		const _map &sosoTypeMap) {
	string temp("");
	size_t strLen = srcStr.length();
	_map::const_iterator it;

	for (int i = 0; i < strLen; i++) {
		temp = srcStr.substr(i, strLen - i);
		it = sosoTypeMap.find(temp);
		if (it != sosoTypeMap.end()) {
			// 找到后缀
//			result.shortName = srcStr.substr(0, i);
//			result.suffix = temp;
			result.soso_type = it->second;
			if (result.soso_type.size() == 6) {
				result.soso_main_type = result.soso_type.substr(0, 2);
				result.soso_sub_type = result.soso_type.substr(0, 4);
			}
			break;
		}
		if (srcStr.at(i) < 0) {
			i++;
		}
	}

	return 0;
}

// 长词后缀
int findTailType(const std::string &srcStr, std::string &soso_type,
		const _map &sosoTypeMap) {
	string temp("");
	size_t strLen = srcStr.length();
	_map::const_iterator it;

	for (int i = 0; i < strLen; i++) {
		temp = srcStr.substr(i, strLen - i);
		it = sosoTypeMap.find(temp);
		if (it != sosoTypeMap.end()) {
			// 找到后缀
			soso_type = it->second;
			break;
		}
		if (srcStr.at(i) < 0) {
			i++;
		}
	}

	return 0;
}

// 长词后缀
int findAreaTail(CacheResult &result, _hashset &areaTailSet) {
	string temp("");
	size_t strLen = result.formatName.length();
	_hashsetit it;

	for (int i = 0; i < strLen; i++) {
		temp = result.formatName.substr(i, strLen - i);
		it = areaTailSet.find(temp);
		if (it != areaTailSet.end()) {
			// 找到后缀
			if (i > 2) {
				result.inBracketList.push_back(temp);
				result.formatName = result.formatName.substr(0, i);
				break;
			}
		}
		if (result.formatName.at(i) < 0) {
			i++;
		}
	}

	return 0;
}

int findAreaTail(OriCacheResult &result, _hashset &areaTailSet) {
	string temp("");
	size_t strLen = result.formatName.length();
	_hashsetit it;

	for (int i = 0; i < strLen; i++) {
		temp = result.formatName.substr(i, strLen - i);
		it = areaTailSet.find(temp);
		if (it != areaTailSet.end()) {
			// 找到后缀
			if (i > 2) {
				result.inBracketList.push_back(temp);
				result.formatName = result.formatName.substr(0, i);
				break;
			}
		}
		if (result.formatName.at(i) < 0) {
			i++;
		}
	}

	return 0;
}

int findSoSoType(CacheResult &src, _map &sosoTypeMap) {
	// 解决需要使用小类进行区分，且小类判断出现在括号内两种情况：
	// 249900	西便门幼儿园
	// 241200	北京市宣武区西便门小学
	// 241100	北京教育学院附属中学(东新开胡同)
	// 241100	北京教育学院（附属中学
	std::string temp_soso_type = "";
	findTailType(src.formatName, src.soso_type, sosoTypeMap);
	if (startswith(src.soso_type, "24")) {
		if (src.inBracketList.size() > 0) {
			findTailType(src.inBracketList[src.inBracketList.size() - 1],
					temp_soso_type, sosoTypeMap);
			if (temp_soso_type != "" && temp_soso_type != src.soso_type
					&& !startswith(temp_soso_type, "2499")
					&& !startswith(temp_soso_type, "26")
					&& !startswith(temp_soso_type, "27")
					&& !startswith(temp_soso_type, "28")) {
				src.soso_type = temp_soso_type;
			}
		}
	}

	return 0;
}

int findSoSoType(OriCacheResult &src, _map &sosoTypeMap) {
	// 解决需要使用小类进行区分，且小类判断出现在括号内两种情况：
	// 249900	西便门幼儿园
	// 241200	北京市宣武区西便门小学
	// 241100	北京教育学院附属中学(东新开胡同)
	// 241100	北京教育学院（附属中学
	std::string temp_soso_type = "";
	findTailType(src.formatName, src.soso_type, sosoTypeMap);
	if (startswith(src.soso_type, "24")) {
		if (src.inBracketList.size() > 0) {
			findTailType(src.inBracketList[src.inBracketList.size() - 1],
					temp_soso_type, sosoTypeMap);
			if (temp_soso_type != "" && temp_soso_type != src.soso_type
					&& !startswith(temp_soso_type, "2499")
					&& !startswith(temp_soso_type, "26")
					&& !startswith(temp_soso_type, "27")
					&& !startswith(temp_soso_type, "28")) {
				src.soso_type = temp_soso_type;
			}
		}
	}

	return 0;
}

int filteLongAlp(_vector &src) {
	_vectorit it = src.begin();
	while (it != src.end()) {
		if (len(*it) == (*it).length() && (*it).length() >= 15
				&& src.size() > 1) {
			it = src.erase(it);
		} else {
			++it;
		}
	}
	return 0;
}

int filteDoorInBrac(_vector &src, _set &doorSet, const Resource &resource) {
	int src_size = src.size();
	if (src_size > 0) {
		if (resource.doorSet.find(src[src_size - 1])
				!= resource.doorSet.end()) {
			doorSet.insert(src[src.size() - 1]);
			src.erase(src.end() - 1);
		}
	}

	return 0;
}

int filteDoor(_vector &src, _set &doorSet, const Resource &resource) {
	if (src.size() > 1) {
		if (resource.doorSet.find(src[src.size() - 1])
				!= resource.doorSet.end()) {
			doorSet.insert(src[src.size() - 1]);
			src.erase(src.end() - 1);
		}
	}

	return 0;
}

int filteDoor2(_vector &src, _set &doorSet, const Resource &resource) {
	_vectorit it = src.begin();
	while (it != src.end()) {
		if (resource.doorSet.find(*it) != resource.doorSet.end()) {
			doorSet.insert(*it);
			it = src.erase(it);
		} else {
			it++;
		}
	}

	return 0;
}

int filteDoor(_set &src, _set &doorSet) {
	_setit it = src.begin();
	while (it != src.end()) {
		if (endswith(*it, "门")) {
			doorSet.insert(*it);
			src.erase(it++);
		} else {
			++it;
		}
	}
	return 0;
}

bool checkSetInclude(const _set &a_set, const _set &b_set) {
	_set::const_iterator it = a_set.begin();
	for (; it != a_set.end(); ++it) {
		if (b_set.find(*it) == b_set.end())
			return false;
	}
	return true;
}

int countNotType(_vector &src, int &count, Resource &resource) {
	_vectorit it = src.begin();
	count = 0;
	for (; it != src.end(); ++it) {
		if (resource.typeword["01"].find(*it)
				== resource.typeword["01"].end()) {
			count++;
		}
	}
	return 0;
}

int countNotType(_vector &src, int &count, const _mapSet &typeword) {
	_vectorit it = src.begin();
	count = 0;
	_mapSet::const_iterator it_map = typeword.find("01");
	if (it_map != typeword.end()) {
		for (; it != src.end(); ++it) {
			if (it_map->second.find(*it) == it_map->second.end()) {
				count++;
			}
		}
	}
	return 0;
}

int replaceSpecWord(std::string &src) {
	if (src == "医院") {
		src = "院";
	} else if (src == "中学") {
		src = "中";
	} else if (src == "小学") {
		src = "小";
	}
	return 0;
}

int filte(_vector &src, _vector &result, _set &filted,
		const _hashset &resource) {
	result.clear();
	_vectorit it = src.begin();
	for (; it != src.end(); ++it) {
		if (resource.find(*it) != resource.end()) {
			filted.insert(*it);
		} else {
			result.push_back(*it);
		}
	}

	return 0;
}

int set_inter(_set &aSet, _set &bSet, _vector &resultVec) {
	resultVec.clear();
	_setit it = aSet.begin();
	for (; it != aSet.end(); ++it) {
		if (bSet.find(*it) != bSet.end()) {
			resultVec.push_back(*it);
		}
	}
	return 0;
}

int set_diff(_set &aSet, _set &bSet, _vector &resultVec) {
	resultVec.clear();
	_setit it = aSet.begin();
	for (; it != aSet.end(); ++it) {
		if (bSet.find(*it) == bSet.end()) {
			resultVec.push_back(*it);
		}
	}
	return 0;
}

int addrIsSame(std::string &str_a, std::string &str_b) {
	double result = computeAddressSimilarity(str_a.c_str(), str_b.c_str());
	cout << "addr similar:" << result << endl;
	return 0;
}

int transWord(_mapStrDbl &objMap, _mapStrDbl &resultMap, _map &transMap) {
	_mapStrDblit it = objMap.begin();
	_mapit it_word;
	for (; it != objMap.end(); ++it) {
		it_word = transMap.find(it->first);
		if (it_word != transMap.end()) {
			resultMap.insert(make_pair(it_word->second, it->second));
		} else {
			resultMap.insert(make_pair(it->first, it->second));
		}
	}
	return 0;
}

int transWord(_map &objMap, _map &resultMap, _map &transMap) {
	_mapit it = objMap.begin();
	_mapit it_word;
	for (; it != objMap.end(); ++it) {
		it_word = transMap.find(it->first);
		if (it_word != transMap.end()) {
			resultMap.insert(make_pair(it_word->second, it->second));
		} else {
			resultMap.insert(make_pair(it->first, it->second));
		}
	}
	return 0;
}

//int transWord(_hashset &src_set, _hashset &result_set, _map &transWord) {
//	_hashsetit it = src_set.begin();
//	_mapit it_word;
//	for (; it != src_set.end(); ++it) {
//		it_word = transWord.find(*it);
//		if (it_word != transWord.end()) {
//			result_set.insert(it_word->second);
//		} else {
//			result_set.insert(*it);
//		}
//	}
//
//	return 0;
//}

int transWord(set<string> &src_set, set<string> &result_set, _map &transMap) {
	set<string>::iterator it = src_set.begin();
	_mapit it_word;
	result_set.clear();
	for (; it != src_set.end(); ++it) {
		it_word = transMap.find(*it);
		if (it_word != transMap.end()) {
			result_set.insert(it_word->second);
		} else {
			result_set.insert(*it);
		}
	}

	return 0;
}

int transWord(_mapSet &oldMap, _mapSet &newMap, _map &transMap) {
	_mapSet::iterator it = oldMap.begin();
	set<string> temp_set;
	for (; it != oldMap.end(); ++it) {
		temp_set.clear();
		transWord(it->second, temp_set, transMap);
		newMap.insert(make_pair(it->first, temp_set));
	}
	return 0;
}

int findMaxMin(_mapStrDbl &src_map, double &max, double &min) {
	_mapStrDblit it = src_map.begin();
	for (; it != src_map.end(); ++it) {
		if (max < it->second) {
			max = it->second;
		}
		if (min > it->second) {
			min = it->second;
		}
	}
	return 0;
}

//int addPrefix(_vector &src_vec, _hashset &word_set) {
//	_vectorit it = src_vec.begin();
//	std::string specSuffix = "省市区县乡镇村";
//	std::string tempLastSuffix(" ");
//	for (; it != src_vec.end(); ++it) {
//		if (word_set.find(*it) != word_set.end()
//				&& specSuffix.find(tempLastSuffix) != std::string::npos) {
//			*it = tempLastSuffix + *it;
//		}
//		if ((*it).length() > 2) {
//			tempLastSuffix = (*it).substr((*it).length() - 2, 2);
//		} else {
//			tempLastSuffix = "";
//		}
//	}
//
//	return 0;
//}

int addPrefix(_vector &src_vec, const _hashset &word_set) {
	std::string specSuffix = "省市区县乡镇村";
	std::string tempLastSuffix(" ");
	for (int i = 0; i < src_vec.size(); ++i) {
		if (word_set.find(src_vec[i]) != word_set.end()
				&& specSuffix.find(tempLastSuffix) != std::string::npos) {
			src_vec[i] = tempLastSuffix + src_vec[i];
		}
		if (src_vec[i].length() > 2) {
			tempLastSuffix = src_vec[i].substr(src_vec[i].length() - 2, 2);
		} else {
			tempLastSuffix = "";
		}
	}

	return 0;
}

int formatGov(_vector &src, Resource &resource) {
	_vectorit it = src.begin();
	std::string str_gov_tail = "县乡镇村";
	std::string str_end("");
	for (; it != src.end(); ++it) {
		if ((*it).length() > 4) {
			str_end = (*it).substr((*it).length() - 2, 2);
			if (resource.typeword["01"].find(*it)
					== resource.typeword["01"].end()
					&& str_gov_tail.find(str_end) != std::string::npos) {
				*it = (*it).substr(0, (*it).length() - 2);
			}
		}
	}

	return 0;
}

int formatGov(_vector &src, OriCacheResult &result, Resource &resource) {
	_vectorit it = src.begin();
	std::string str_gov_tail = "县乡镇村";
	std::string str_end("");
	for (; it != src.end(); ++it) {
		if ((*it).length() > 4) {
			str_end = (*it).substr((*it).length() - 2, 2);
			if (resource.typeword["01"].find(*it)
					== resource.typeword["01"].end()
					&& str_gov_tail.find(str_end) != std::string::npos) {
				*it = (*it).substr(0, (*it).length() - 2);
				result.gov_tail = str_end;
			}
		}
	}

	return 0;
}

int formatGov(std::string &src, Resource &resource) {
	std::string str_gov_tail = "县乡镇村";
	std::string str_end("");
	if (src.length() > 4) {
		str_end = src.substr(src.length() - 2, 2);
		if (resource.typeword["01"].find(src) == resource.typeword["01"].end()
				&& str_gov_tail.find(str_end) != std::string::npos) {
			src = src.substr(0, src.length() - 2);
		}
	}

	return 0;
}

int formatGov(std::string &src, OriCacheResult &result, Resource &resource) {
	std::string str_gov_tail = "县乡镇村";
	std::string str_end("");
	if (src.length() > 4) {
		str_end = src.substr(src.length() - 2, 2);
		if (resource.typeword["01"].find(src) == resource.typeword["01"].end()
				&& str_gov_tail.find(str_end) != std::string::npos) {
			src = src.substr(0, src.length() - 2);
			result.gov_tail = str_end;
		}
	}

	return 0;
}

bool isSpecType(CacheResult &a, CacheResult &b, std::string str_type) {
	if ((startswith(a.type, str_type)
			|| startswith(a.sosoAttr.soso_type, str_type))
			&& (startswith(b.type, str_type)
					|| startswith(b.sosoAttr.soso_type, str_type))) {
		return true;
	} else {
		return false;
	}
	return 0;
}

int mergeTailSglWord(OriCacheResult &result) {
	int nameWordCount = result.name.nameWordList.size();
	if (nameWordCount > 1
			&& (startswith(result.type, "24") || startswith(result.type, "12"))) {
		std::string str_tail = "委办";
		if (str_tail.find(result.name.nameWordList[nameWordCount - 1])
				!= std::string::npos) {
			result.name.nameWordList[nameWordCount - 2] +=
					result.name.nameWordList[nameWordCount - 1];
			result.name.nameWordList.erase(result.name.nameWordList.end());
		}
	}

	return 0;
}

bool isType(std::string str_type) {
	char szTemp[7];
	memcpy(szTemp, str_type.c_str(), 6);
	szTemp[6] = 0;
	if (str_type == "") {
		return true;
	}

	if (str_type.size() != 6) {
		assert(false);
		return false;
	}

	char *p = szTemp;
	char * pcEnd = p + 6;
	while (*p != '\0' && p < pcEnd) {
		if (*p < '0' || *p > '9') {
			assert(false);
			return false;
		}
		p++;
	}

	return true;
}
