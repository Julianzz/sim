#include "init.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "common.h"
#include "rules.h"

bool judge_dir(const char *path) {
	DIR * dir;
	dir = opendir(path);
	if (!dir) {
		return false;
	} else
		return true;
}

int load(const std::string &path, _mapSet &result) {
	DIR * dir;
	struct dirent * ent;
	string dirpath = path;
	if (dirpath[dirpath.length() - 1] != '/') {
		dirpath += "/";
	}
	//cout << "the directory path is :" << dirpath << endl;
	dir = opendir(dirpath.c_str());
	if (!dir) {
		return -1;
	}

	while (ent = readdir(dir)) {
		if ((strcmp(ent->d_name, ".") == 0)
				|| (strcmp(ent->d_name, "..") == 0)) {
			continue;
		}
		string filename = dirpath + ent->d_name;
		if (!judge_dir(filename.c_str())) {
			string strname = ent->d_name;
			_hashset tempSet;
			//cout << "Processing the file:" << filename << endl;
			load(filename, tempSet);
			result.insert(make_pair(ent->d_name, tempSet));
			//cout << "size:" << result[ent->d_name].size() << endl;
		}
	}
	closedir(dir);

	_mapSetit itResult = result.begin();
	_mapSetit itType;
	_vector tempVec;
	_vectorit itVec;
	for (; itResult != result.end(); ++itResult) {
//		if(itResult->first != "00" && itResult->first != "01"){
//			itResult->second.insert(result["00"].begin(), result["00"].end());
//			itResult->second.insert(result["01"].begin(), result["01"].end());
//		}
		if (itResult->first != "00") {
			itResult->second.insert(result["00"].begin(), result["00"].end());
		}
		splitType(itResult->first, tempVec);
		if (tempVec.size() > 1) {
			for (itVec = tempVec.begin(); itVec != tempVec.end(); ++itVec) {
				if (itResult->first == *itVec) {
					continue;
				}
				itType = result.find(*itVec);
				if (itType != result.end()) {
					result[itResult->first].insert(itType->second.begin(),
							itType->second.end());
					result["01"].insert(itType->second.begin(),
							itType->second.end());
				}
			}
		}
	}

//	for (itResult = result.begin(); itResult != result.end(); ++itResult) {
//		cout << "size:" << itResult->first << "\t" << itResult->second.size()
//				<< endl;
//	}

	return 0;
}

int load(const std::string &path, _map &result, const std::string &delim,
		bool rever, bool clear) {
	std::ifstream fin(path.c_str(), std::ios::in);
	if (fin.fail()) {
		std::cout << "[ERR]: File open error in " << __FILE__ << std::endl;
		return -1;
	}

	std::string line("");
	std::vector<std::string> strList;
	if (clear)
		result.clear();
	while (std::getline(fin, line)) {

		if (split(line, delim, strList) == -1)
			return -1;

		if (strList.size() == 2) {
			if (rever)
				result.insert(make_pair(strList[1], strList[0]));
			else
				result.insert(make_pair(strList[0], strList[1]));
		}
	}

	return 0;
}

int load(const std::string &path, _hashset &result, bool clear) {
	std::ifstream fin(path.c_str(), std::ios::in);
	if (fin.fail()) {
		std::cout << "[ERR]: File open error in " << __FILE__ << std::endl;
		return -1;
	}

	std::string line("");
	if (clear)
		result.clear();
	while (std::getline(fin, line)) {
		if (line.length() >= 1) {
			result.insert(line);
		}
	}
	return 0;
}

int load(const std::string &path, _vector &result, bool clear) {
	std::ifstream fin(path.c_str(), std::ios::in);
	if (fin.fail()) {
		std::cout << "[ERR]: File open error in " << __FILE__ << std::endl;
		return -1;
	}

	std::string line("");
	if (clear)
		result.clear();
	while (std::getline(fin, line)) {
		result.push_back(line);
	}
	return 0;
}

int load(const std::string &path, _mapVec& result, const std::string &delim,
		bool rever, bool clear) {
	std::ifstream fin(path.c_str(), std::ios::in);
	if (fin.fail()) {
		std::cout << "[ERR]: File open error in " << __FILE__ << std::endl;
		return -1;
	}

	std::string line("");
	std::string chWord("");
	_vector strList;
	_mapVecit it;
	if (clear)
		result.clear();
	while (std::getline(fin, line)) {
		if (split(line, delim, strList) == -1)
			return -1;

		if (strList.size() == 2) {
			if (rever)
				reverse(strList.begin(), strList.end());
			it = result.find(strList[0]);
			if (it != result.end()) {
				it->second.push_back(strList[1]);
			} else {
				chWord = strList[0];
				strList.erase(strList.begin());
				result.insert(make_pair(chWord, strList));
			}
		}
	}

	return 0;
}

int loadTypeMap(const std::string &path, _mapSet &result,
		const std::string &delim, bool rever, bool clear) {
	std::ifstream fin(path.c_str(), std::ios::in);
	if (fin.fail()) {
		std::cout << "[ERR]: File open error in " << __FILE__ << std::endl;
		return -1;
	}

	std::string line("");
	std::string chWord("");
	_vector strList;
	_mapSetit it1, it2;
	if (clear)
		result.clear();
	while (std::getline(fin, line)) {
		if (split(line, delim, strList) == -1)
			return -1;

		if (strList.size() == 2) {
			if (rever)
				reverse(strList.begin(), strList.end());
			it1 = result.find(strList[0]);
			it2 = result.find(strList[1]);
			if (it1 != result.end()) {
				it1->second.insert(strList[1]);
			} else {
				_hashset tempSet;
				tempSet.insert(strList[1]);
				result.insert(make_pair(strList[0], tempSet));
			}

			if (it2 != result.end()) {
				it2->second.insert(strList[0]);
			} else {
				_hashset tempSet;
				tempSet.insert(strList[0]);
				result.insert(make_pair(strList[1], tempSet));
			}
		}
	}

	return 0;
}

int load(const std::string &path, _mapStrDbl& result, const std::string &delim,
		bool rever, bool clear) {
	std::ifstream fin(path.c_str(), std::ios::in);
	if (fin.fail()) {
		std::cout << "[ERR]: File open error in " << __FILE__ << std::endl;
		return -1;
	}

	std::string line("");
	std::vector<std::string> strList;
	if (clear)
		result.clear();
	while (std::getline(fin, line)) {

		if (split(line, delim, strList) == -1)
			return -1;

		if (strList.size() >= 2) {
			if (rever)
				result.insert(make_pair(strList[1], Str2Double(strList[0])));
			else
				result.insert(make_pair(strList[0], Str2Double(strList[1])));
		}
	}

	return 0;
}

int load(const std::string &path, _mapStrIntSet &result,
		const std::string &delim, bool clear) {
	std::ifstream fin(path.c_str(), std::ios::in);
	if (fin.fail()) {
		std::cout << "[ERR]: File open error in " << __FILE__ << std::endl;
		return -1;
	}

	std::string line("");
	std::vector<std::string> strVec;
	if (clear)
		result.clear();
	int no = 0;
	_vectorit it;
	while (std::getline(fin, line)) {
		if (split(line, delim, strVec) == -1)
			return -1;
		it = strVec.begin();
		for (; it != strVec.end(); ++it) {
			if (result.find(*it) != result.end()) {
				result[*it].insert(no);
			} else {
				set<int> tempset;
				tempset.insert(no);
				result.insert(make_pair(*it, tempset));
			}
		}
		no++;
	}

	return 0;
}

int loadTrans(const std::string &path, _map &result, const std::string &delim,
		bool rever, bool clear) {
	std::ifstream fin(path.c_str(), std::ios::in);
	if (fin.fail()) {
		std::cout << "[ERR]: File open error in " << __FILE__ << std::endl;
		return -1;
	}

	std::string line(""), value("");
	_vector strList;
	_vectorit it;
	if (clear)
		result.clear();
	while (std::getline(fin, line)) {

		if (split(line, delim, strList) == -1)
			return -1;

		if (strList.size() >= 1) {
			it = strList.begin();
			value = *it;
			for (; it != strList.end(); ++it) {
				result.insert(make_pair(*it, value));
			}
		}
	}

	return 0;
}

int loadObjType(const std::string &path, _map &result, const std::string &delim,
		bool clear) {
	std::ifstream fin(path.c_str(), std::ios::in);
	if (fin.fail()) {
		std::cout << "[ERR]: File open error in " << __FILE__ << std::endl;
		return -1;
	}

	std::string line("");
	_vector strList;
	if (clear)
		result.clear();
	while (std::getline(fin, line)) {
		if (split(line, delim, strList) == -1)
			return -1;

		if (strList.size() >= 3) {
			result.insert(make_pair(strList[0], strList[2]));
		}
	}

	return 0;
}

int load(_vector &path, _map &result, const std::string &delim, bool rever) {
	result.clear();
	_vectorit it = path.begin();
	for (; it != path.end(); ++it)
		if (load(*it, result, delim, rever, false) == -1)
			return -1;
	return 0;
}

int load(_vector &path, _hashset &result) {
	result.clear();
	_vectorit it = path.begin();
	for (; it != path.end(); ++it)
		if (load(*it, result, false) == -1)
			return -1;
	return 0;
}

int load(_vector &path, _vector &result) {
	result.clear();
	_vectorit it = path.begin();
	for (; it != path.end(); ++it)
		if (load(*it, result, false) == -1)
			return -1;
	return 0;
}

int load(_vector &path, _mapVec& result, const std::string &delim, bool rever) {
	result.clear();
	_vectorit it = path.begin();
	for (; it != path.end(); ++it)
		if (load(*it, result, delim, rever, false) == -1)
			return -1;
	return 0;
}

int loadTypeMap(_vector &path, _mapSet& result, const std::string &delim) {
	result.clear();
	_vectorit it = path.begin();
	for (; it != path.end(); ++it) {
		if (loadTypeMap(*it, result, delim, false, false) == -1)
			return -1;
		if (loadTypeMap(*it, result, delim, true, false) == -1)
			return -1;
	}
	return 0;
}

int load(_vector &path, _mapStrDbl& result, const std::string &delim,
		bool rever) {
	result.clear();
	_vectorit it = path.begin();
	for (; it != path.end(); ++it)
		if (load(*it, result, delim, rever, false) == -1)
			return -1;

	return 0;
}

int loadTrans(_vector &path, _map &result, const std::string &delim,
		bool rever) {
	result.clear();
	_vectorit it = path.begin();
	for (; it != path.end(); ++it)
		if (loadTrans(*it, result, delim, rever, false) == -1)
			return -1;
	return 0;
}

int loadObjType(_vector &path, _map &result, const std::string &delim) {
	_vectorit it = path.begin();
	for (; it != path.end(); ++it)
		if (loadObjType(*it, result, delim, false) == -1)
			return -1;
	return 0;
}

int load(_vector &path, _mapStrIntSet &result, const std::string &delim) {
	_vectorit it = path.begin();
	for (; it != path.end(); ++it)
		if (load(*it, result, delim, false) == -1)
			return -1;
	return 0;
}
