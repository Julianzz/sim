#include "common.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

int split(const std::string &src, const std::string &delimit,
		std::vector<std::string> &v, std::string null_subst) {
	v.clear();
	if (src.empty() || delimit.empty())
		return -1;

	size_t deli_len = delimit.size();
	long index = -1, last_search_position = 0;
	while ((index = src.find(delimit, last_search_position)) != -1) {
		if (index == last_search_position)
			v.push_back(null_subst);
		else
			v.push_back(
					src.substr(last_search_position,
							index - last_search_position));
		last_search_position = index + deli_len;
	}
	std::string last_one = src.substr(last_search_position);
	v.push_back(last_one.empty() ? null_subst : last_one);
	return 0;
}

int split(const std::string &src, const std::string &delimit,
		std::set<std::string> &v, std::string null_subst) {
	v.clear();
	if (src.empty() || delimit.empty())
		return -1;

	size_t deli_len = delimit.size();
	long index = -1, last_search_position = 0;
	while ((index = src.find(delimit, last_search_position)) != -1) {
		if (index == last_search_position)
			v.insert(null_subst);
		else
			v.insert(
					src.substr(last_search_position,
							index - last_search_position));
		last_search_position = index + deli_len;
	}
	std::string last_one = src.substr(last_search_position);
	v.insert(last_one.empty() ? null_subst : last_one);
	return 0;
}

int split2(const std::string &src, const std::string &tok,
		std::vector<std::string> &v, bool trim, std::string null_subst) {
	v.clear();
	if (src.empty() || tok.empty())
		return -1;

	size_t pre_index = 0, index = 0, len = 0;
	while ((index = src.find_first_of(tok, pre_index)) != std::string::npos) {
		if ((len = index - pre_index) != 0)
			v.push_back(src.substr(pre_index, len));
		else if (trim == false)
			v.push_back(null_subst);
		pre_index = index + 1;
	}
	std::string endstr = src.substr(pre_index);
	if (trim == false)
		v.push_back(endstr.empty() ? null_subst : endstr);
	else if (!endstr.empty())
		v.push_back(endstr);
	return 0;
}

int split2word(const std::string &s, std::vector<std::string> &result) {
	result.clear();
	int len = 2;
	size_t strLen = s.length();
	char *p = (char*) s.c_str();
	size_t i = 0, j = 0;
	char * pcEnd = (char *) s.c_str() + strLen;
	while (*p != '\0' && p < pcEnd) {
		if (*p >= 0) {
			++p;
			++i;
		} else {
			if (i > j) {
				result.push_back(s.substr(j, i - j));
				j = i;
			}
			if ((i + len) <= strLen) {
				result.push_back(s.substr(i, len));
				p += len;
				i += len;
				j = i;
			} else {
				result.push_back(s.substr(i, 1));
				++p;
				++i;
				j = i;
				break;
			}
		}
	}
	if (i > j)
		result.push_back(s.substr(j, i - j));

	return 0;
}

int split2word2(const std::string &s, std::vector<std::string> &result) {
	result.clear();
	int temp = 0;
	size_t strLen = s.length();
	char *p = (char*) s.c_str();
	size_t i = 0;
	char * pcEnd = (char *) s.c_str() + strLen;
	while (*p != '\0' && p < pcEnd) {
		if (*p >= 0) {
			temp = 1;
			result.push_back(s.substr(i, temp));

		} else {
			temp = 2;
			result.push_back(s.substr(i, temp));
			p++;
			i++;
		}
		p++;
		i++;
	}
	return 0;
}

void spliteBigram(const std::string &src, std::vector<std::string> &vec_words) {
	vec_words.clear();
	char *p_start = (char *) src.c_str();
	char *sp = p_start;
	char *cp = p_start;
	char *p_end = p_start + src.length();
	bool flag = true; // 标记单字要不要插入，true需要插入

	while (*cp != '\0' && cp < p_end) {
		if (*cp >= 0) {
			// english
			// 处理英文之前的中文
			if (cp - sp == 4) {
				vec_words.push_back(src.substr(sp - p_start, 4));
				flag = true;
			} else if (cp - sp == 2 && flag) {
				vec_words.push_back(src.substr(sp - p_start, 2));
			}

			sp = cp;

			// 找到所有连续的英文
			while (*cp != '\0' && cp < p_end) {
				cp++;
				if (*cp < 0) {
					break;
				}
			}

			vec_words.push_back(src.substr(sp - p_start, cp - sp));
			sp = cp;
		} else {
			// chinese
			if (cp - sp == 4) {
				vec_words.push_back(src.substr(sp - p_start, 4));
				sp += 2;
				flag = false;
			}
			cp += 2;
		}
	}

	if (p_end - sp == 4) {
		vec_words.push_back(src.substr(sp - p_start, 4));
	} else if (sp < p_end && flag) {
		vec_words.push_back(src.substr(sp - p_start, p_end - sp));
	}
}

int replace(std::string &str, const std::string &oldValue,
		const std::string &newValue, std::string &newStr) {
	newStr = str;
	for (size_t pos = 0; pos != std::string::npos; pos += newValue.length()) {
		if ((pos = newStr.find(oldValue, pos)) != std::string::npos)
			newStr.replace(pos, oldValue.length(), newValue);
		else
			break;
	}
	return 0;
}

string& replaceStr(string& str, const string& old_value,
		const string& new_value) {
	for (string::size_type pos(0); pos != string::npos; pos +=
			new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return str;
}

int LCS(std::vector<std::string> &s1, std::vector<std::string> &s2,
		std::vector<std::string> &result) {
	result.clear();
	if (s1.size() == 0 || s2.size() == 0)
		return 0;

	int c[s1.size()];
	int max, maxj, i, j;
	maxj = 0;
	max = 0;
	for (i = 0; (size_t) i <= s2.size() - 1; i++)
		for (j = s1.size() - 1; j >= 0; j--) {
			if (s2[i] == s1[j]) {
				if (i == 0 || j == 0)
					c[j] = 1;
				else
					c[j] = c[j - 1] + 1;
			} else
				c[j] = 0;
			if (c[j] > max) {
				max = c[j];
				maxj = j;
			}
		}
	result.clear();
	if (max == 0)
		return 0;
	for (j = maxj - max + 1; j <= maxj; j++)
		result.push_back(s1[j]);
	return 0;
}

size_t len(const std::string &a) {
	size_t count = 0;
	int dwPos = 0;
	int dwStrLen = (int) a.size();
	while (dwPos < dwStrLen) {
		if (a[dwPos] < 0) {
			dwPos++;
		}
		dwPos++;
		++count;
	}

	return count;
}

double Str2Double(const std::string& str) {
	// atof在windows和linux上定义相同
	return atof(str.c_str());
}

bool endswith(const std::string &src, const std::string &endStr) {
	if (src.length() < endStr.length())
		return false;

	std::string temp = src.substr(src.length() - endStr.length());
	if (strcmp((char*) &src[src.length() - endStr.length()], endStr.c_str())
			== 0) {
		return true;
	}

	return false;
}

bool startswith(const std::string &src, const std::string &startStr) {
	if (src.length() < startStr.length())
		return false;

	char * pcTemp = (char *) src.c_str();
	char cTemp = *(pcTemp + startStr.length());
	*(pcTemp + startStr.length()) = 0;
	if (strcmp(pcTemp, startStr.c_str()) == 0) {
		*(pcTemp + startStr.length()) = cTemp;
		return true;
	}
	*(pcTemp + startStr.length()) = cTemp;
	return false;
}

bool startsWithSet(const std::string &src, const set<string> &dataSet) {
	if (src.size() > 2) {
		string temp_str = src.substr(0, 2);
		if (dataSet.find(temp_str) != dataSet.end()) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool endsWithSet(const std::string &src, const set<string> &dataSet) {
	if (src.size() > 2) {
		string temp_str = src.substr(src.size() - 2, 2);
		if (dataSet.find(temp_str) != dataSet.end()) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool isDigit(std::string &src) {
	std::string::iterator it = src.begin();
	bool flag = true;
	for (; it != src.end(); ++it) {
		if (!ISDIGIT(*it)) {
			flag = false;
			break;
		}
	}

	if (flag)
		return true;
	else
		return false;
}

bool isAlpha(std::string &src) {
	std::string::iterator it = src.begin();
	bool flag = true;
	for (; it != src.end(); ++it) {
		if (!ISALPHA(*it)) {
			flag = false;
			break;
		}
	}

	if (flag)
		return true;
	else
		return false;
}

bool isAlpDig(std::string &src) {
	std::string::iterator it = src.begin();
	bool flag = true;
	for (; it != src.end(); ++it) {
		if (!ISALPHA(*it) && !ISDIGIT(*it)) {
			flag = false;
			break;
		}
	}

	if (flag)
		return true;
	else
		return false;
}

size_t levDis(const std::string &s1, const std::string &s2) {
	const size_t m(s1.size());
	const size_t n(s2.size());
	if (m == 0)
		return n;
	if (n == 0)
		return m;
	size_t *costs = new size_t[n + 1];

	for (size_t k = 0; k <= n; k++) {
		costs[k] = k;
	}

	size_t i = 0;
	for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end();
			++it1, ++i) {
		costs[0] = i + 1;
		size_t corner = i;
		size_t j = 0;
		for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end();
				++it2, ++j) {
			size_t upper = costs[j + 1];
			if (*it1 == *it2) {
				costs[j + 1] = corner;
			} else {
				size_t t(upper < corner ? upper : corner);
				costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
			}
			corner = upper;
		}
	}
	size_t result = costs[n];
	delete[] costs;
	return result;
}
