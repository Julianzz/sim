/*
 * computeAddressSimilarity.cpp
 *
 *  Created on: 2010-8-31
 *      Author: pavelliu
 */

#include <string>
#include <map>
#include <set>
#include <vector>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "computeAddressSimilarity.h"
#include "classify_data/StringUtility.h"

using namespace std;

#define UNKNOWN 0
#define PARALLEL 1
#define ORIENTATION 2
#define OTHER_NUMBER 3
#define OTHER_DATA 4

#define PROVINCE 10
#define CITY 11
#define DISTRICT 12
#define SUBDISTRICT 13
#define ROAD 14
#define POI 15
#define HOUSE_NUMBER 16
#define NUMBER_SUFFIX 5

#define COMMUNITY_KEY 20
#define COMMUNITY_SUFFIX 24
#define ROAD_SUFFIX 25
#define POI_SUFFIX 26

#define PROVINCE_FLAG 20
#define CITY_FLAG 21
#define DISTRICT_FLAG 22
#define SUBDISTRICT_FLAG 23
#define ROAD_FLAG 24
#define POI_FLAG 25
#define HOUSE_NUMBER_FLAG 26

int32_t getGbkLen(const char * pData) {
	if (pData == NULL) {
		return -1;
	}

	int32_t iLen = 0;
	while (*pData != '\0') {
		if (*pData < 0) {
			pData += 2;
		} else {
			pData += 1;
		}
		iLen++;
	}

	return iLen;
}

void convertChineseToDigit(string &retNumber, const char * pData) {
	map<string, int> m_chineseCorrespondDigit;
	vector<string> v_chineseCorrespondDigit;
	vector<string> v_chineseCorrespondClass;
	map<string, int>::iterator it_m_chineseCorrespondDigit;
	vector<string>::iterator it_v_chineseCorrespondDigit;
	vector<string>::iterator it_v_chineseCorrespondClass;
	m_chineseCorrespondDigit.insert(pair<string, int>("һ", 1));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 2));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 3));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 4));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 5));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 6));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 7));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 8));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 9));
	m_chineseCorrespondDigit.insert(pair<string, int>("ʮ", 10));
	m_chineseCorrespondDigit.insert(pair<string, int>("Ҽ", 1));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 2));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 3));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 4));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 5));
	m_chineseCorrespondDigit.insert(pair<string, int>("½", 5));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 7));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 8));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 9));
	m_chineseCorrespondDigit.insert(pair<string, int>("ʰ", 10));
	m_chineseCorrespondDigit.insert(pair<string, int>("�f", 10000));
	m_chineseCorrespondDigit.insert(pair<string, int>("Ǫ", 1000));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 100));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 0));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 0));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 1));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 2));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 3));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 4));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 5));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 6));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 7));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 8));
	m_chineseCorrespondDigit.insert(pair<string, int>("��", 9));
	v_chineseCorrespondDigit.push_back("ʮ");
	v_chineseCorrespondDigit.push_back("��");
	v_chineseCorrespondDigit.push_back("ǧ");
	v_chineseCorrespondDigit.push_back("��");
	v_chineseCorrespondDigit.push_back("ʰ");
	v_chineseCorrespondDigit.push_back("Ǫ");
	v_chineseCorrespondDigit.push_back("��");
	v_chineseCorrespondDigit.push_back("�f");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("_");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	v_chineseCorrespondClass.push_back("��");
	int number = 0;
	string temp = "";
	if (pData == NULL)
		return;
	//cout<<pData<<endl;
	while (*pData != '\0') {
		if (*pData < 0) {
			temp = "";
			temp += (*pData);
			pData += 1;
			temp += (*pData);
			pData += 1;
			//cout<<"temp="<<temp<<endl;
			if ((it_m_chineseCorrespondDigit = m_chineseCorrespondDigit.find(
					temp)) != m_chineseCorrespondDigit.end()) {
				if ((it_v_chineseCorrespondDigit = find(
						v_chineseCorrespondDigit.begin(),
						v_chineseCorrespondDigit.end(), temp))
						!= v_chineseCorrespondDigit.end()) {
					number *= (*it_m_chineseCorrespondDigit).second;
				} else {
					number *= 10;
					number += (*it_m_chineseCorrespondDigit).second;
				}
			} else {
				if ((it_v_chineseCorrespondClass = find(
						v_chineseCorrespondClass.begin(),
						v_chineseCorrespondClass.end(), temp))
						!= v_chineseCorrespondClass.end()) {
					if (number > 0) {
						char c[100];
						sprintf(c, "%d", number);
						retNumber += c;
						number = 0;
					}
					retNumber += temp;
				} else
					break;
			}
		} else {
			if (*pData > '0' && *pData <= '9') {
				number *= 10;
				number += (*pData - '0');
				pData += 1;
			} else {
				temp = "";
				temp += *pData;
				if (*pData == '0' || (*pData >= 'A' && *pData <= 'Z')
						|| (*pData >= 'a' && *pData <= 'z')
						|| (it_v_chineseCorrespondClass = find(
								v_chineseCorrespondClass.begin(),
								v_chineseCorrespondClass.end(), temp))
								!= v_chineseCorrespondClass.end()) {
					if (number > 0) {
						char c[100];
						//itoa(number,c,10);
						sprintf(c, "%d", number);
						retNumber += c;
						number = 0;
					}
					retNumber += *pData;
					pData += 1;
				} else
					break;
			}
		}
	}
	if (number > 0) {
		char c[100];
		//itoa(number,c,10);
		sprintf(c, "%d", number);
		retNumber += c;
	}
}

void getNumber(string &retNumber, const char * pData) {
	retNumber.clear();
	if (pData == NULL) {
		return;
	}

	const char * pTempData = pData;
	while (*pTempData != '\0') {
		if (*pTempData < 0) {
			break;
		} else {
			if (*pTempData >= '0' && *pTempData <= '9') {
				retNumber.append(pTempData, 1);
				pTempData += 1;
			} else {
				break;
			}
		}
	}

	pTempData = pData;
	if (retNumber.size() == 0 && *pTempData != '\0') {
		if (*pTempData < 0) {
			pTempData += 2;
		} else {
			pTempData += 1;
		}
		while (*pTempData != '\0') {
			if (*pTempData < 0) {
				break;
			} else {
				if (*pTempData >= '0' && *pTempData <= '9') {
					retNumber.append(pTempData, 1);
					pTempData += 1;
				} else {
					break;
				}
			}
		}
	}

	return;
}
//�ַ����з�
int32_t splitString(vector<string> &result, const char *pStr,
		const char *pSeparate) {

	if (pStr == NULL || pSeparate == NULL) {
		return -1;
	}

	result.clear();
	char separateBuf[128];
	memset(separateBuf, 0, sizeof(separateBuf));
	while (*pSeparate > 0) {
		separateBuf[(uint8_t) (*pSeparate++)] = 1;
	}

	const char *pFront, *pBack;
	pFront = pBack = pStr;
	while (*pFront != '\0') {
		if (*pFront < 0) {
			pFront += 2;
		} else {
			if (separateBuf[(uint8_t) (*pFront)] == 1) {
				result.push_back(string(pBack, pFront - pBack));
				pFront++;
				pBack = pFront;
			} else {
				pFront++;
			}
		}
	}

	result.push_back(string(pBack, pFront - pBack));
	return 0;
}
//���ݵ�ַ�зֵõ��õ�ַ�е�ʡ��������·������poi
int32_t getPart(const char *pSplitAddress, map<int, string> &xzqhDict,
		map<string, string> &roadDict, set<string> &poiDict) {

	int32_t iRet = 0;
	set<string> unknownDict;

	vector<string> addressVector;
	vector<string> typeVector;
	splitString(addressVector, pSplitAddress, "!");
	vector<string>::iterator addressIter;
	for (addressIter = addressVector.begin();
			addressIter != addressVector.end();) {
		splitString(typeVector, addressIter->c_str(), "|");
		if (typeVector.size() <= 1) {
			iRet = -1;
			return iRet;
		}

		const char * addressItem = typeVector[0].c_str();
		bool isXZQH = false;
		for (uint32_t uiIndex = 1; uiIndex < typeVector.size(); uiIndex++) {
			uint32_t uiCurrentType = strtoul(typeVector[uiIndex].c_str(), NULL,
					0);
			if ((uiCurrentType >= PROVINCE && uiCurrentType <= DISTRICT)
					|| uiCurrentType == OTHER_DATA) {
				isXZQH = true;
				break;
			}
		}

		if (isXZQH) {
			for (uint32_t uiIndex = 1; uiIndex < typeVector.size(); uiIndex++) {
				uint32_t uiCurrentType = strtoul(typeVector[uiIndex].c_str(),
						NULL, 0);
				if (xzqhDict.find(uiCurrentType) == xzqhDict.end()) {
					xzqhDict.insert(
							pair<int, string>(uiCurrentType, addressItem));
				}
			}
			addressIter++;
		} else {
			break;
		}
	}

	for (; addressIter != addressVector.end();) {
		splitString(typeVector, addressIter->c_str(), "|");
		if (typeVector.size() <= 1) {
			iRet = -1;
			return iRet;
		}
		const char * pAddressItem = typeVector[0].c_str();
		int32_t iAddressItemLen = getGbkLen(pAddressItem);
		assert(iAddressItemLen!=-1);
		for (uint32_t uiIndex = 1; uiIndex < typeVector.size(); uiIndex++) {
			uint32_t uiCurrentType = strtoul(typeVector[uiIndex].c_str(), NULL,
					0);
			if (uiCurrentType == ROAD) {
				if (roadDict.find(pAddressItem) == roadDict.end()) {
					roadDict.insert(pair<string, string>(pAddressItem, ""));
				}
				map<string, string>::iterator roadMapIter;
				roadMapIter = roadDict.find(pAddressItem);
				if (addressIter + 1 != addressVector.end()) {
					string retNumber;
					getNumber(retNumber, (addressIter + 1)->c_str());
					if (retNumber.size() != 0) {
						addressIter++;
						roadMapIter->second = retNumber;
					}
				}

			} else if (uiCurrentType == POI) {
				if (poiDict.find(pAddressItem) == poiDict.end()) {
					poiDict.insert(pAddressItem);
				}
			} else if ((uiCurrentType == SUBDISTRICT || uiCurrentType == UNKNOWN)
					&& iAddressItemLen > 2 && iAddressItemLen <= 8) {
				if (unknownDict.find(pAddressItem) == unknownDict.end()) {
					unknownDict.insert(pAddressItem);
				}
			}
		}

		addressIter++;
	}

	for (set<string>::iterator iter = unknownDict.begin();
			iter != unknownDict.end(); iter++) {
		poiDict.insert(*iter);
	}

	return iRet;
}
//�������poi�����ƶ�
uint32_t computePoiSimilarity(set<string> &poiDict1, set<string> &poiDict2) {

	uint32_t uiSameTimes = 0;
	for (set<string>::iterator iter1 = poiDict1.begin();
			iter1 != poiDict1.end(); iter1++) {
		const char *pPoi1 = iter1->c_str();
		bool isFind = false;
		for (set<string>::iterator iter2 = poiDict2.begin();
				iter2 != poiDict2.end(); iter2++) {
			const char * pPoi2 = iter2->c_str();
			if (strstr(pPoi1, pPoi2) != NULL || strstr(pPoi2, pPoi1) != NULL) {
				isFind = true;
				break;
			}
		}

		if (isFind) {
			uiSameTimes++;
		}
	}

	if (poiDict1.size() == 0 || poiDict2.size() == 0) {
		return 0; //��֪��
	} else {

		if (uiSameTimes == 0) {
			return 2; //��ͻ
		} else if (uiSameTimes == 1) {
			if (poiDict1.size() == 1 && poiDict2.size() == 1) {
				return 11; //һ��POI��ͬû������POI;
			} else {
				return 10; //һ��POI��ͬ����������POI;
			}

		} else {
			return 12; //���ڵ�������POI��ͬ
		}
	}
}
uint32_t computeCommunitySimilarity(set<string> &communityDict1,
		set<string> &communityDict2) {
	uint32_t uiSameTimes = 0;
	for (set<string>::iterator iter1 = communityDict1.begin();
			iter1 != communityDict1.end(); iter1++) {
		const char *pCommunity1 = iter1->c_str();
		bool isFind = false;
		for (set<string>::iterator iter2 = communityDict2.begin();
				iter2 != communityDict2.end(); iter2++) {
			const char *pCommunity2 = iter2->c_str();
			if (strstr(pCommunity1, pCommunity2) != NULL
					|| strstr(pCommunity2, pCommunity1) != NULL) {
				isFind = true;
				break;
			}
		}
		if (isFind)
			uiSameTimes++;
	}
	if (communityDict1.size() == 0 || communityDict2.size() == 0)
		return 0; //��֪��
	else {
		if (uiSameTimes == 0)
			return 2; //��ͻ
		else if (uiSameTimes == 1) {
			if (communityDict1.size() == 1 && communityDict2.size() == 1)
				return 11;
			else
				return 10;
		} else
			return 12;
	}
}
//����ʡ����������ƶ�
uint32_t computeXZQHSimilarity(map<int, string> &xzqhDict1,
		map<int, string> &xzqhDict2) {
	map<int, string>::iterator iter1;
	map<int, string>::iterator iter2;
	int PROVINCE_tag = 0, CITY_tag = 0, DISTRICT_tag = 0;
	iter1 = xzqhDict1.find(PROVINCE);
	iter2 = xzqhDict2.find(PROVINCE);
	if (iter1 != xzqhDict1.end() && iter2 != xzqhDict2.end()
	&& strstr(iter1->second.c_str(), iter2->second.c_str()) == NULL
	&& strstr(iter2->second.c_str(), iter1->second.c_str()) == NULL) {

		PROVINCE_tag = 2; //ʡ��ͻ
	} else if (iter1 == xzqhDict1.end() || iter2 == xzqhDict2.end()) {
		PROVINCE_tag = 1; //ʡȱʧ
	}
	iter1 = xzqhDict1.find(CITY);
	iter2 = xzqhDict2.find(CITY);
	if (iter1 != xzqhDict1.end() && iter2 != xzqhDict2.end()
	&& strstr(iter1->second.c_str(), iter2->second.c_str()) == NULL
	&& strstr(iter2->second.c_str(), iter1->second.c_str()) == NULL) {
		CITY_tag = 2; //�г�ͻ
	} else if (iter1 == xzqhDict1.end() || iter2 == xzqhDict2.end()) {
		CITY_tag = 1;
	}
	iter1 = xzqhDict1.find(DISTRICT);
	iter2 = xzqhDict2.find(DISTRICT);
	if (iter1 != xzqhDict1.end() && iter2 != xzqhDict2.end()
	&& strstr(iter1->second.c_str(), iter2->second.c_str()) == NULL
	&& strstr(iter2->second.c_str(), iter1->second.c_str()) == NULL) {
		DISTRICT_tag = 2; //����ͻ
	} else if (iter1 == xzqhDict1.end() || iter2 == xzqhDict2.end()) {
		DISTRICT_tag = 1;
	}
	if (PROVINCE_tag == 1 && CITY_tag == 1 && DISTRICT_tag == 1)
		return 0; //ʡ����ȱʧ
	//2-��ͻ
	//1-����ͻ
	//0-ȱ��ʡ����
	if (PROVINCE_tag == 2 || CITY_tag == 2)
		return 2;
	if (DISTRICT_tag == 2) {
		return 10;
	} else if (DISTRICT_tag == 1) {
		return 11;
	} else
		return 12;
}
//�����·������ƶ�
uint32_t computeRoadSimilarity(map<string, string> &roadDict1,
		map<string, string> &roadDict2) {
	uint32_t uiRet = 0;
	uint32_t uiSameTimes = 0;
	uint32_t uiMinRoadNumberDistance = 999999;
	for (map<string, string>::iterator iter1 = roadDict1.begin();
			iter1 != roadDict1.end(); iter1++) {
		const char * pRoad1 = iter1->first.c_str();
		const char * pNumber1 = iter1->second.c_str();

		bool isFind = false;
		const char * pRoad2 = NULL;
		const char * pNumber2 = NULL;
		for (map<string, string>::iterator iter2 = roadDict2.begin();
				iter2 != roadDict2.end(); iter2++) {
			pRoad2 = iter2->first.c_str();
			pNumber2 = iter2->second.c_str();

			if (strstr(pRoad1, pRoad2) != NULL || strstr(pRoad2, pRoad1) != NULL) {
				isFind = true;
				break;
			}
		}

		if (isFind) {
			uiSameTimes += 1;
			assert(pRoad1!=NULL&&pNumber1!=NULL&&pRoad2!=NULL&&pNumber2!=NULL);
			int32_t iRoadNumber1 = atoi(pNumber1);
			int32_t iRoadNumber2 = atoi(pNumber2);
			if (iRoadNumber1 > 0 && iRoadNumber2 > 0) {
				if (abs(iRoadNumber1 - iRoadNumber2)
						< uiMinRoadNumberDistance) {
					uiMinRoadNumberDistance = abs(iRoadNumber1 - iRoadNumber2);
				}
			}

		}

	}

	if (roadDict1.size() == 0 || roadDict2.size() == 0) {
		uiRet = 0; //��֪��
	} else {
		if (uiSameTimes == 0) {
			uiRet = 2; //��ͻ
		} else if (uiSameTimes == 1) {
			if (uiMinRoadNumberDistance == 0) {
				uiRet = 12; //���ƺ���ͬ
			} else if (uiMinRoadNumberDistance > 0
					&& uiMinRoadNumberDistance <= 15) {
				uiRet = 11; //���ƺ����
			} else if (uiMinRoadNumberDistance > 15
					&& uiMinRoadNumberDistance != 999999) {
				uiRet = 10;
			} else {
				if (roadDict1.size() == 1 && roadDict2.size() == 1) {
					uiRet = 10; //һ��·��ͬ��û��������·��Ϣ
				} else {
					uiRet = 10; //һ��·��ͬ������������·��Ϣ
				}
			}
		} else { //same times >=2;
			if (uiMinRoadNumberDistance == 0) {
				uiRet = 12; //���ƺ���ͬ
			} else if (uiMinRoadNumberDistance > 0
					&& uiMinRoadNumberDistance <= 15) {
				uiRet = 11; //���ƺ����
			} else if (uiMinRoadNumberDistance > 15
					&& uiMinRoadNumberDistance != 999999) {
				uiRet = 10;
			} else {
				uiRet = 10; //����·��ͬ
			}
		}
	}

	return uiRet;
}
//���㷿��ŵ����ƶ�
uint32_t computeHouseNumSimilarity(set<string> &houseNumSet1,
		set<string> &houseNumSet2, set<string> &homeSet1, set<string> &homeSet2,
		set<string> &floorSet1, set<string> &floorSet2) {
	uint32_t houseNum_tag = 0, home_tag = 0, floor_tag = 0;
	uint32_t uiSameTimes = 0;
	for (set<string>::iterator iter1 = houseNumSet1.begin();
			iter1 != houseNumSet1.end(); iter1++) {
		const char *pHouseNum1 = iter1->c_str();
		bool isFind = false;
		for (set<string>::iterator iter2 = houseNumSet2.begin();
				iter2 != houseNumSet2.end(); iter2++) {
			const char * pHouseNum2 = iter2->c_str();
			if (strstr(pHouseNum1, pHouseNum2) != NULL
					|| strstr(pHouseNum2, pHouseNum1) != NULL) {
				isFind = true;
			}
			if (isFind)
				uiSameTimes++;
		}
	}
	if (houseNumSet1.size() == 0 || houseNumSet2.size() == 0)
		houseNum_tag = 0; //��֪��
	else {
		if (uiSameTimes == 0) {
			houseNum_tag = 2;
		} else if (uiSameTimes == 1) {
			if (houseNumSet1.size() == 1 && houseNumSet2.size() == 1)
				houseNum_tag = 12; //һ��houseNum��ͬû������houseNum;
			else
				houseNum_tag = 11; //һ��houseNum��ͬ����������houseNum;
		} else
			houseNum_tag = 12; //���ڵ�������houseNum��ͬ
	}
	uiSameTimes = 0;
	for (set<string>::iterator iter1 = homeSet1.begin();
			iter1 != homeSet1.end(); iter1++) {
		const char *pHome1 = iter1->c_str();
		bool isFind = false;
		for (set<string>::iterator iter2 = homeSet2.begin();
				iter2 != homeSet2.end(); iter2++) {
			const char * pHome2 = iter2->c_str();
			if (strstr(pHome1, pHome2) != NULL || strstr(pHome2, pHome1) != NULL) {
				isFind = true;
			}
			if (isFind)
				uiSameTimes++;
		}
	}
	if (homeSet1.size() == 0 || homeSet2.size() == 0)
		home_tag = 0; //��֪��
	else {
		if (uiSameTimes == 0) {
			home_tag = 2;
		} else if (uiSameTimes == 1) {
			if (homeSet1.size() == 1 && homeSet2.size() == 1)
				home_tag = 12; //һ��home��ͬû������home;
			else
				home_tag = 11; //һ��home��ͬ����������home;
		} else
			home_tag = 12; //���ڵ�������home��ͬ
	}
	uiSameTimes = 0;
	for (set<string>::iterator iter1 = floorSet1.begin();
			iter1 != floorSet1.end(); iter1++) {
		const char *pFloor1 = iter1->c_str();
		bool isFind = false;
		for (set<string>::iterator iter2 = floorSet2.begin();
				iter2 != floorSet2.end(); iter2++) {
			const char * pFloor2 = iter2->c_str();
			if (strstr(pFloor1, pFloor2) != NULL
					|| strstr(pFloor1, pFloor2) != NULL) {
				isFind = true;
			}
			if (isFind)
				uiSameTimes++;
		}
	}
	if (floorSet1.size() == 0 || floorSet2.size() == 0)
		floor_tag = 0; //��֪��
	else {
		if (uiSameTimes == 0)
			floor_tag = 2;
		else if (uiSameTimes == 1)
			if (floorSet1.size() == 1 && floorSet2.size() == 1)
				floor_tag = 12; //һ��home��ͬû������floor;
			else
				floor_tag = 11; //һ��floor��ͬ����������floor;
		else
			floor_tag = 12; //���ڵ�������floor��ͬ
	}
//	cout << "houseNum_tag=" << houseNum_tag << " home_tag=" << home_tag
//			<< " floor_tag=" << floor_tag << endl;
	if (houseNum_tag == 0 && home_tag == 0 && floor_tag == 0)
		return 0; //��֪��
	else if (houseNum_tag == 2 || home_tag == 2 || floor_tag == 2)
		return 2; //��ͻ
	else if (houseNum_tag == 0 || home_tag == 0 || floor_tag == 0)
		return 11; //��ͬ
	else
		return 12;
}
double computeAddressSimilarity(const char * pAddress1,
		const char * pAddress2) {
	if (pAddress1 == NULL || pAddress2 == NULL || *pAddress1 == '\0'
			|| *pAddress2 == '\0') {
		return 0.0;
	}

	map<int, string> xzqhDict1, xzqhDict2;
	map<string, string> roadDict1, roadDict2;
	set<string> poiDict1, poiDict2;
	double dRet;

	getPart(pAddress1, xzqhDict1, roadDict1, poiDict1);
	getPart(pAddress2, xzqhDict2, roadDict2, poiDict2);

	uint32_t uiSubdistrictSimilarity = 0;
	uint32_t uiXZQHSimilarity = computeXZQHSimilarity(xzqhDict1, xzqhDict2);
	uint32_t uiRoadSimilarity = computeRoadSimilarity(roadDict1, roadDict2);
	uint32_t uiPoiSimilarity = computePoiSimilarity(poiDict1, poiDict2);

	if (uiXZQHSimilarity == 0) { //����������ͻ
		dRet = 0.0;
	} else {
		if (uiSubdistrictSimilarity == 1) {
			dRet = 0.3;
		} else {
			if (uiRoadSimilarity == 0) {
				if (uiPoiSimilarity == 0) {
					dRet = 0.5;
				} else if (uiPoiSimilarity == 1) {
					dRet = 0.45;
				} else {
					if (uiPoiSimilarity == 4) {
						dRet = 0.99;
					} else {
						if (uiPoiSimilarity == 2) {
							dRet = 0.85;
						} else {
							dRet = 0.75;
						}
					}
				}
			} else if (uiRoadSimilarity == 1) {
				if (uiPoiSimilarity == 4) {
					dRet = 0.97;
				} else if (uiPoiSimilarity == 2 && uiPoiSimilarity == 3) {
					dRet = 0.75;
				} else {
					dRet = 0.4;
				}
			} else {
				if (uiRoadSimilarity == 2) {
					dRet = 1.0;
				} else if (uiRoadSimilarity == 3) {
					dRet = 0.95;
				} else if (uiRoadSimilarity == 4) {
					dRet = 0.48;
				} else if (uiRoadSimilarity == 5 || uiRoadSimilarity == 6) {
					if (uiPoiSimilarity >= 2) {
						dRet = 0.97;
					} else if (uiRoadSimilarity == 5 && uiPoiSimilarity == 2) {
						dRet = 0.78;
					} else {
						dRet = 0.65;
					}
				} else {
					assert(uiRoadSimilarity==7);
					dRet = 0.96;
				}
			}
		}
	}

	return dRet;
}

void init_group_score(map<string, double> &m_group_score,
		map<string, double> &m_sigle_score) {
	m_group_score.insert(pair<string, double>("12#12", 1.0));
	m_group_score.insert(pair<string, double>("12#11", 0.98));
	m_group_score.insert(pair<string, double>("12#0", 0.97));
	m_group_score.insert(pair<string, double>("12#10", 0.96));
	m_group_score.insert(pair<string, double>("11#12", 0.94));
	m_group_score.insert(pair<string, double>("11#11", 0.92));
	m_group_score.insert(pair<string, double>("11#0", 0.82));
	m_group_score.insert(pair<string, double>("12#2", 0.78));
	m_group_score.insert(pair<string, double>("10#12", 0.76));
	m_group_score.insert(pair<string, double>("11#10", 0.72));
	m_group_score.insert(pair<string, double>("10#11", 0.70));
	m_group_score.insert(pair<string, double>("10#0", 0.60));
	m_group_score.insert(pair<string, double>("0#12", 0.75));
	m_group_score.insert(pair<string, double>("10#10", 0.45));
	m_group_score.insert(pair<string, double>("0#11", 0.60));
	m_group_score.insert(pair<string, double>("0#0", 0.50));
	m_group_score.insert(pair<string, double>("11#2", 0.40));
	m_group_score.insert(pair<string, double>("0#10", 0.30));
	m_group_score.insert(pair<string, double>("10#2", 0.20));
	m_group_score.insert(pair<string, double>("0#2", -1.0));
	m_group_score.insert(pair<string, double>("2#12", 0.16));
	m_group_score.insert(pair<string, double>("2#11", 0.12));
	m_group_score.insert(pair<string, double>("2#0", 0.10));
	m_group_score.insert(pair<string, double>("2#10", 0.08));
	m_group_score.insert(pair<string, double>("2#2", -3.0));

	m_sigle_score.insert(pair<string, double>("2", -1.8));
	m_sigle_score.insert(pair<string, double>("0", 0.5));
	m_sigle_score.insert(pair<string, double>("10", 0.65));
	m_sigle_score.insert(pair<string, double>("11", 0.88));
	m_sigle_score.insert(pair<string, double>("12", 1.0));
}

double computeAddressSimilarity(map<int, string> xzqhDict1,
		map<int, string> xzqhDict2, map<string, string> roadDict1,
		map<string, string> roadDict2, set<string> communityDict1,
		set<string> communityDict2, set<string> poiDict1, set<string> poiDict2,
		set<string> houseNumSet1, set<string> houseNumSet2,
		set<string> homeSet1, set<string> homeSet2, set<string> floorSet1,
		set<string> floorSet2, AddressSimilarityValue &myaddressSimilarityValue,
		map<string, double> m_group_score, map<string, double> m_sigle_score) {

	if ((xzqhDict1.size() == 0 && roadDict1.size() == 0 && poiDict1.size() == 0)
			|| (xzqhDict2.size() == 0 && roadDict2.size() == 0
					&& poiDict2.size() == 0)) {
		return 0.0;
	}

	double dRet = 0;

	uint32_t uiSubdistrictSimilarity = 0;
	//����ʡ���������ƶȡ���·���ƶȡ�����poi�����ƶ�
	uint32_t uiXZQHSimilarity = computeXZQHSimilarity(xzqhDict1, xzqhDict2);
	//cout << "uiXZQHSimilarity=" << uiXZQHSimilarity << endl;
	uint32_t uiRoadSimilarity = computeRoadSimilarity(roadDict1, roadDict2);
	uint32_t uiCommunitySimilarity = computeCommunitySimilarity(communityDict1,
			communityDict2);
	//cout << "uiRoadSimilarity=" << uiRoadSimilarity << endl;
	uint32_t uiPoiSimilarity = computePoiSimilarity(poiDict1, poiDict2);
	//cout << "uiPoiSimilarity=" << uiPoiSimilarity << endl;
	uint32_t uihouseNumSimilarity = computeHouseNumSimilarity(houseNumSet1,
			houseNumSet2, homeSet1, homeSet2, floorSet1, floorSet2);
	//cout << "uihouseNumSimilarity=" << uihouseNumSimilarity << endl;
	myaddressSimilarityValue.xzqhSimilarity = uiXZQHSimilarity;
	myaddressSimilarityValue.roadSimilarity = uiRoadSimilarity;
	myaddressSimilarityValue.communitySimilarity = uiCommunitySimilarity;
	myaddressSimilarityValue.poiSimilarity = uiPoiSimilarity;
	myaddressSimilarityValue.houseNumSimilarity = uihouseNumSimilarity;

	string xzqh_sim = "";
	string road_sim = "";
	string community_sim = "";
	string poi_sim = "";
	string houseNum_sim = "";
	double d_xzqh_sim, d_road_sim, d_community_sim, d_poi_sim, d_houseNum_sim;
	char c[100];
	sprintf(c, "%d", myaddressSimilarityValue.xzqhSimilarity);
	xzqh_sim += c;
	sprintf(c, "%d", myaddressSimilarityValue.roadSimilarity);
	road_sim += c;
	sprintf(c, "%d", myaddressSimilarityValue.communitySimilarity);
	community_sim += c;
	sprintf(c, "%d", myaddressSimilarityValue.poiSimilarity);
	poi_sim += c;
	sprintf(c, "%d", myaddressSimilarityValue.houseNumSimilarity);
	houseNum_sim += c;
	d_xzqh_sim = (*(m_sigle_score.find(xzqh_sim))).second * xzqh_threshold;
	d_road_sim = (*(m_sigle_score.find(road_sim))).second * road_threshold;
	d_community_sim = (*(m_sigle_score.find(community_sim))).second
			* community_threshold;
	d_poi_sim = (*(m_sigle_score.find(poi_sim))).second * poi_threshold;
	d_houseNum_sim = (*(m_sigle_score.find(houseNum_sim))).second
			* houseNum_threshold;
	string group_xzqh_road = "";
	string group_road_poi = "";
	string group_community_poi = "";
	string group_poi_houseNum = "";
	string group_xzqh_poi = "";
	double d_group_xzqh_road, d_group_road_poi, d_group_community_poi,
			d_group_poi_houseNum, d_group_xzqh_poi;
	group_xzqh_road += road_sim;
	group_xzqh_road += "#";
	group_xzqh_road += xzqh_sim;

	group_road_poi += poi_sim;
	group_road_poi += "#";
	group_road_poi += road_sim;

	group_community_poi += poi_sim;
	group_community_poi += "#";
	group_community_poi += community_sim;

	group_poi_houseNum += poi_sim;
	group_poi_houseNum += "#";
	group_poi_houseNum += houseNum_sim;

	group_xzqh_poi += poi_sim;
	group_xzqh_poi += "#";
	group_xzqh_poi += xzqh_sim;

	d_group_xzqh_road = (*(m_group_score.find(group_xzqh_road))).second
			* xzqh_road_threshold;
	d_group_road_poi = (*(m_group_score.find(group_road_poi))).second
			* road_poi_threshold;
	d_group_community_poi = (*(m_group_score.find(group_community_poi))).second
			* community_poi_threshold;
	d_group_poi_houseNum = (*(m_group_score.find(group_poi_houseNum))).second
			* poi_houseNum_threshold;
	d_group_xzqh_poi = (*(m_group_score.find(group_xzqh_poi))).second
			* xzqh_poi_threshold;

	dRet += d_xzqh_sim;
	dRet += d_road_sim;
	dRet += d_community_sim;
	dRet += d_poi_sim;
	dRet += d_houseNum_sim;
	dRet += d_group_xzqh_road;
	dRet += d_group_road_poi;
	dRet += d_group_community_poi;
	dRet += d_group_poi_houseNum;

	/*
	 if (uiXZQHSimilarity == 2)
	 { //����������ͻ
	 dRet = 0.0;
	 }
	 else if(uiXZQHSimilarity == 0)
	 {
	 if(uiRoadSimilarity == 12)
	 dRet+=0.21;
	 else if(uiRoadSimilarity == 11)
	 dRet+=0.16;
	 else if(uiRoadSimilarity == 10)
	 dRet+=0.11;
	 else if(uiRoadSimilarity == 2)
	 dRet+=0.0;
	 }
	 else
	 {
	 dRet+=0.05;
	 if(uiRoadSimilarity == 12)
	 dRet+=0.25;
	 else if(uiRoadSimilarity == 11)
	 dRet+=0.20;
	 else if(uiRoadSimilarity == 10)
	 dRet+=0.15;
	 else if(uiRoadSimilarity == 2)
	 dRet+=0.0;
	 else
	 dRet+=0.05;
	 }
	 if(uiRoadSimilarity==2)
	 return dRet;
	 else if(uiRoadSimilarity==0)
	 {
	 if(uiPoiSimilarity==12)
	 dRet+=0.46;
	 else if(uiPoiSimilarity==11)
	 dRet+=0.41;
	 else if(uiPoiSimilarity==10)
	 dRet+=0.36;
	 else if(uiPoiSimilarity==2)
	 dRet+=0.0;
	 else
	 dRet+=0.05;
	 }
	 else
	 {
	 if(uiPoiSimilarity==12)
	 dRet+=0.51;
	 else if(uiPoiSimilarity==11)
	 dRet+=0.46;
	 else if(uiPoiSimilarity==10)
	 dRet+=0.41;
	 else if(uiPoiSimilarity==2)
	 dRet+=0.00;
	 else
	 dRet+=0.05;
	 }
	 if(uiPoiSimilarity==2)
	 return dRet;
	 else if(uiPoiSimilarity==0)
	 {
	 if(uihouseNumSimilarity==12)
	 dRet+=0.21;
	 else if(uihouseNumSimilarity==11)
	 dRet+=0.16;
	 else if(uihouseNumSimilarity==10)
	 dRet+=0.11;
	 else if(uiPoiSimilarity==2)
	 dRet+=0.0;
	 else
	 dRet+=0.05;
	 }
	 else
	 {
	 if(uihouseNumSimilarity==12)
	 dRet+=0.31;
	 else if(uihouseNumSimilarity==11)
	 dRet+=0.26;
	 else if(uihouseNumSimilarity==10)
	 dRet+=0.21;
	 else if(uiPoiSimilarity==2)
	 dRet+=0.09;
	 else
	 dRet+=0.15;
	 }
	 */
	return dRet;
}
//�ӵ�ַ�зִ��еõ�ʡ���С�����·������poi������ź�¥����Ϣ��
int32_t getAddressPart(const char *pSplitAddress, map<int, string> &xzqhDict,
		map<string, string> &roadDict, set<string> &communitySet,
		set<string> &poiSet, set<string> &hnSet, set<string> &homeSet,
		set<string> &floorSet) {
	xzqhDict.clear();
	roadDict.clear();
	poiSet.clear();
	hnSet.clear();
	floorSet.clear();
	if (pSplitAddress == NULL) {
		return 0;
	}
	vector<string> addressVector;
	vector<string>::iterator addressIter;
	splitString(addressVector, pSplitAddress, "!");
	//StringUtility::SplitString(pSplitAddress,"!",addressVector,true);
	for (addressIter = addressVector.begin();
			addressIter != addressVector.end(); addressIter++) {

		vector<string> typeVector;
		splitString(typeVector, addressIter->c_str(), "|");
		if (typeVector.size() <= 1) {
			continue;
		}

		const char * pAddressValue = typeVector[0].c_str();
		for (uint32_t uiIndex = 1; uiIndex < typeVector.size(); uiIndex++) {
			uint32_t uiTempType = strtoul(typeVector[uiIndex].c_str(), NULL, 0);
			if (uiTempType == PROVINCE) {
				if (xzqhDict.find(PROVINCE) == xzqhDict.end()) {
					xzqhDict.insert(pair<int, string>(PROVINCE, pAddressValue));
				}
			} else if (uiTempType == CITY) {
				if (xzqhDict.find(CITY) == xzqhDict.end()) {
					xzqhDict.insert(pair<int, string>(CITY, pAddressValue));
				}
			} else if (uiTempType == DISTRICT) {
				if (xzqhDict.find(DISTRICT) == xzqhDict.end()) {
					xzqhDict.insert(pair<int, string>(DISTRICT, pAddressValue));
				}
			} else if (uiTempType == SUBDISTRICT || uiTempType == COMMUNITY_KEY
					|| uiTempType == COMMUNITY_SUFFIX) {
				communitySet.insert(pAddressValue);
			} else if (uiTempType == ROAD || uiTempType == ROAD_SUFFIX) {
				if (roadDict.find(pAddressValue) == roadDict.end()) {
					roadDict.insert(pair<string, string>(pAddressValue, ""));
				}
				map<string, string>::iterator roadMapIter;
				roadMapIter = roadDict.find(pAddressValue);
				if (addressIter + 1 != addressVector.end()) {
					string retNumber;
					getNumber(retNumber, (addressIter + 1)->c_str());
					if (retNumber.size() != 0) {
						addressIter++;
						roadMapIter->second = retNumber;
						hnSet.insert(retNumber);
					}
				}
			} else if (uiTempType == POI || uiTempType == POI_SUFFIX) {
				int tag_next = 0, tag_up = 0;
				vector<string> vTemp;
				uint32_t uiTemp;

				if (addressIter + 1 != addressVector.end()) {
					splitString(vTemp, (addressIter + 1)->c_str(), "|");
					uiTemp = strtoul(vTemp[1].c_str(), NULL, 0);
					if (uiTemp != ORIENTATION) {
						tag_next = 1;
					}
				} else {
					tag_next = 1;
				}

				vTemp.clear();
				if (addressIter != addressVector.begin()) {
					splitString(vTemp, (addressIter - 1)->c_str(), "|");
					uiTemp = strtoul(vTemp[1].c_str(), NULL, 0);
					if (uiTemp != ORIENTATION) {
						tag_up = 1;
					}
				} else {
					tag_up = 1;
				}

				if (tag_next == 1 && tag_up == 1) {
					poiSet.insert(pAddressValue);
				}
			} else if (uiTempType == HOUSE_NUMBER) {
				string retNumber = "";
				//getNumber(retNumber, pAddressValue);
				convertChineseToDigit(retNumber, pAddressValue);
				if (strstr(pAddressValue, "��¥") != NULL
						|| strstr(pAddressValue, "��Ԫ") != NULL
						|| strstr(pAddressValue, "��") != NULL
						|| strstr(pAddressValue, "#") != NULL
						|| strstr(pAddressValue, "��Ժ") != NULL
						|| strstr(pAddressValue, "Ժ") != NULL
						|| strstr(pAddressValue, "��") != NULL
						|| strstr(pAddressValue, "��") != NULL
						|| strstr(pAddressValue, "��") != NULL) {
					hnSet.insert(retNumber);
				} else if (strstr(pAddressValue, "��") != NULL) {
					homeSet.insert(retNumber);
				} else if (strstr(pAddressValue, "��") != NULL
						|| strstr(pAddressValue, "¥") != NULL) {
					floorSet.insert(retNumber);
				} else {
					homeSet.insert(retNumber);
				}
			} else if (uiTempType == NUMBER_SUFFIX) {
				string retNumber = "";
				convertChineseToDigit(retNumber, pAddressValue);
				homeSet.insert(retNumber);
			}
		}

	}

	return 0;
}
//��ַ�еĴ���������򷵻�1���д�������������򷵻�2�����򷵻�0
int computeIncludeStr(set<string> &set1, set<string> &set2) {
	if (set1.size() == 0 || set2.size() == 0) {
		return 2;
	}

	for (set<string>::iterator iter1 = set1.begin(); iter1 != set1.end();
			iter1++) {
		const char * pStr1 = iter1->c_str();
		for (set<string>::iterator iter2 = set2.begin(); iter2 != set2.end();
				iter2++) {
			const char * pStr2 = iter2->c_str();

			if (strstr(pStr1, pStr2) != NULL || strstr(pStr2, pStr1) != NULL) {
				return 1;
			}
		}
	}
	return 0;
}
//��ַ�ж�û�����򷵻�ģ��ֵ2��������ͬ�������򷵻�1�����򷵻�0
int computeEqualNum(set<string> &set1, set<string> &set2) {
	if (set1.size() == 0 || set2.size() == 0) {
		return 2;
	}

	for (set<string>::iterator iter1 = set1.begin(); iter1 != set1.end();
			iter1++) {
		const char * pStr1 = iter1->c_str();
		for (set<string>::iterator iter2 = set2.begin(); iter2 != set2.end();
				iter2++) {
			const char * pStr2 = iter2->c_str();

			if (strcmp(pStr1, pStr2) == 0) {
				return 1;
			}
		}
	}
	return 0;
}
/*
 int getSameAddressLevel(const char * pAddress1, const char * pAddress2)
 {
 if (pAddress1 == NULL || pAddress2 == NULL || *pAddress1 == '\0'
 || *pAddress2 == '\0')
 {
 return -1;
 }

 map<int, string> xzqhDict1, xzqhDict2;
 set<string> roadSet1, roadSet2;
 set<string> poiSet1, poiSet2;
 set<string> hnSet1, hnSet2;
 set<string> floorSet1, floorSet2;

 getAddressPart(pAddress1, xzqhDict1, roadSet1, poiSet1, hnSet1, floorSet1);
 getAddressPart(pAddress2, xzqhDict2, roadSet2, poiSet2, hnSet2, floorSet2);

 uint32_t uiXZQHSimilarity = computeXZQHSimilarity(xzqhDict1, xzqhDict2);
 uint32_t uiRoadSimilarity = computeIncludeStr(roadSet1, roadSet2);
 uint32_t uiPoiSimilarity = computeIncludeStr(poiSet1, poiSet2);
 uint32_t uiHnSimilarity = computeEqualNum(hnSet1, hnSet2);
 uint32_t uiFloorSimilarity = computeEqualNum(floorSet1, floorSet2);

 if (uiXZQHSimilarity == 0)
 {
 return 1;
 } else
 {
 if (uiRoadSimilarity == 0)
 {
 if (uiPoiSimilarity == 0)
 {
 return 3;
 }
 else if (uiPoiSimilarity == 1)
 {
 if (uiHnSimilarity == 1 && uiFloorSimilarity == 1)
 {
 return 0;
 }
 else if (uiHnSimilarity == 2 && uiFloorSimilarity == 2)
 {
 return 0;
 }
 else if (uiHnSimilarity == 1 && uiFloorSimilarity == 2)
 {
 return 0;
 }
 else if (uiHnSimilarity == 2 && uiFloorSimilarity == 1)
 {
 return 0;
 }
 else
 {
 return 6;
 }
 }
 else
 {
 return 3;
 }
 }
 else
 {
 if (uiPoiSimilarity == 0)
 {
 return 5;
 }
 else
 {
 if (uiHnSimilarity == 1 && uiFloorSimilarity == 1)
 {
 return 0;
 }
 else if (uiHnSimilarity == 2 && uiFloorSimilarity == 2)
 {
 return 0;
 }
 else if (uiHnSimilarity == 1 && uiFloorSimilarity == 2)
 {
 return 0;
 }
 else if (uiHnSimilarity == 2 && uiFloorSimilarity == 1)
 {
 return 0;
 }
 else
 {
 return 6;
 }
 }
 }
 }
 }
 */
//����ֵΪ0��11~15
int getAddressLevel(const char * pAddress1) {
	int ret = UNKNOWN;
	if (pAddress1 == NULL) {
		return ret;
	}

	vector<string> addressVector;
	splitString(addressVector, pAddress1, "!");
	vector<string>::iterator addressIter;
	for (addressIter = addressVector.begin();
			addressIter != addressVector.end(); addressIter++) {
		vector<string> typeVector;
		splitString(typeVector, addressIter->c_str(), "|");
		if (typeVector.size() <= 1) {
			continue;
		}

		for (uint32_t uiIndex = 1; uiIndex < typeVector.size(); uiIndex++) {
			//strtoul���ַ���ת�����޷��ų�����
			uint32_t uiTempType = strtoul(typeVector[uiIndex].c_str(), NULL, 0);
			if (uiTempType > HOUSE_NUMBER || uiTempType < PROVINCE) {
				continue;
			}

			if (uiTempType > ret) {
				ret = uiTempType;
			}
		}
	}

	return ret;
}

void splitAddressCorrection(const char* splitAddress,
		string &correctSplitAddress) {
	vector<string> vDict1, vDict2;
	vector<string>::iterator it_vDict;
	vDict1.push_back("0");
	vDict1.push_back("1");
	vDict1.push_back("2");
	vDict1.push_back("3");
	//vDict1.push_back("4");
	vDict1.push_back("5");
	vDict1.push_back("6");
	vDict1.push_back("7");
	vDict1.push_back("8");
	vDict1.push_back("9");
	vDict1.push_back("28");
	vDict1.push_back("29");
	vDict1.push_back("30");
	vDict2.push_back("12");
	vDict2.push_back("13");
	vDict2.push_back("14");
	vDict2.push_back("15");
	vDict2.push_back("26");
	vector<string> vTemp;
	splitString(vTemp, splitAddress, "!");
	string prefix = "";
	string prefix_attribute = "";
	for (int i = 0; i < vTemp.size(); i++) {
		vector<string> vTemp_attribute;
		splitString(vTemp_attribute, vTemp[i].c_str(), "|");
		if (vTemp_attribute.size() <= 1)
			continue;
		else {
			string Temp_attribute = "";
			for (int i = 1; i < vTemp_attribute.size(); i++)
				Temp_attribute = vTemp_attribute[i];
			if ((it_vDict = find(vDict1.begin(), vDict1.end(), Temp_attribute))
					!= vDict1.end()) {
				prefix += vTemp_attribute[0];
				prefix_attribute = Temp_attribute;
			} else if ((it_vDict = find(vDict2.begin(), vDict2.end(),
					Temp_attribute)) != vDict2.end()) {
				if (strcmp(prefix_attribute.c_str(), "16") == 0
						&& strcmp(Temp_attribute.c_str(), "15") != 0) {
					if (strcmp(correctSplitAddress.c_str(), "") != 0)
						correctSplitAddress += "!";
					correctSplitAddress += prefix;
					correctSplitAddress += "|";
					correctSplitAddress += prefix_attribute;
					correctSplitAddress += "!";
					correctSplitAddress += vTemp_attribute[0];
					correctSplitAddress += "|";
					correctSplitAddress += Temp_attribute;
				} else {
					prefix += vTemp_attribute[0];
					if (strcmp(correctSplitAddress.c_str(), "") != 0)
						correctSplitAddress += "!";
					correctSplitAddress += prefix;
					correctSplitAddress += "|";
					correctSplitAddress += Temp_attribute;
				}
				prefix = "";
				prefix_attribute = "";
			} else {
				if (strcmp(correctSplitAddress.c_str(), "") != 0)
					correctSplitAddress += "!";
				if (strcmp(prefix.c_str(), "") != 0) {
					correctSplitAddress += prefix;
					correctSplitAddress += "|";
					correctSplitAddress += prefix_attribute;
					correctSplitAddress += "!";
					prefix = "";
					prefix_attribute = "";
				}
				correctSplitAddress += vTemp_attribute[0];
				correctSplitAddress += "|";
				correctSplitAddress += Temp_attribute;
			}
		}
	}
	if (strcmp(prefix.c_str(), "") != 0) {
		if (strcmp(correctSplitAddress.c_str(), "") != 0)
			correctSplitAddress += "!";
		correctSplitAddress += prefix;
		correctSplitAddress += "|";
		correctSplitAddress += prefix_attribute;
	}
}
double computeTwoSplitAddressSimilarity(const char* splitAddress1,
		const char* splitAddress2,
		AddressSimilarityValue &myaddressSimilarityValue,
		map<string, double> m_group_score, map<string, double> m_sigle_score) {
	map<int, string> xzqhDict1, xzqhDict2;
	map<string, string> roadDict1, roadDict2;
	set<string> communitySet1, communitySet2;
	set<string> poiSet1, poiSet2;
	set<string> hnSet1, hnSet2;
	set<string> homeSet1, homeSet2;
	set<string> floorSet1, floorSet2;

	int level1 = getAddressLevel(splitAddress1);
	int level2 = getAddressLevel(splitAddress2);

	myaddressSimilarityValue.level1 = level1;
	myaddressSimilarityValue.level2 = level2;

	getCorrectionAddressPart(splitAddress1, xzqhDict1, roadDict1, communitySet1,
			poiSet1, hnSet1, homeSet1, floorSet1);
	getCorrectionAddressPart(splitAddress2, xzqhDict2, roadDict2, communitySet2,
			poiSet2, hnSet2, homeSet2, floorSet2);
	map<int, string>::iterator it_xzqh;
	map<string, string>::iterator it_road;
	set<string>::iterator it_poi;
	set<string>::iterator it_hn, it_home, it_floor;
	double ret = 0;
	ret = computeAddressSimilarity(xzqhDict1, xzqhDict2, roadDict1, roadDict2,
			communitySet1, communitySet2, poiSet1, poiSet2, hnSet1, hnSet2,
			homeSet1, homeSet2, floorSet1, floorSet2, myaddressSimilarityValue,
			m_group_score, m_sigle_score);
	return ret;
}

void getCorrectionAddressPart(const char* splitAddress,
		map<int, string> &xzqhDict, map<string, string> &roadDict,
		set<string> &communitySet, set<string> &poiSet, set<string> &hnSet,
		set<string> &homeSet, set<string> &floorSet) {
	vector<string> vTemp, vTemp1;

	char * address = (char*) splitAddress;
	int level1 = getAddressLevel(address);

    vector<string> splitParts;
    splitString(vTemp, address, "!");
    int begin = -1, end = -1;
   
    for( int i =0 ; i< vTemp.size(); i++ ) {
        if( begin == -1 ) {
            if( vTemp[i].find_first_of("(") != string::npos || vTemp[i].find_first_of("��") != string::npos ) {
                begin = i;
            }
        }else if( end == -1 ) {
            if( vTemp[i].find_first_of(")") != string::npos || vTemp[i].find_first_of("��") != string::npos ) {
                end = i;
            }
        }
    }
    if ( begin != -1 && end != -1 ) {
        vTemp1.push_back("");
        int i = 0;
        for(  i= 0; i< begin; i++ ) {
            vTemp1[0] +=  vTemp[i];
        }
        vTemp1.push_back("");
        for( i= begin+1 ; i< end ;i++ ) {
            vTemp1[1] += vTemp[i];
        }
       
        for( i= end+1 ; i< vTemp.size(); i++ ) {
            vTemp1[0] +=  vTemp[i];
        }
    }else {
        vTemp1.push_back(address);
    }

    /*std::cout<< vTemp1[0] ;
    if (vTemp1.size() >1) std::cout<< "+++++++++++++++" << vTemp1[1];
    std::cout<<std::endl; 
    */
	map<int, string> temp_xzqhDict;
	map<string, string> temp_roadDict;
	set<string> temp_communitySet;
	set<string> temp_poiSet;
	set<string> temp_hnSet;
	set<string> temp_homeSet;
	set<string> temp_floorSet;
	if (vTemp1.size() > 1) {
		if (xzqhDict.size() == 0) {
			if (roadDict.size() == 0) {
				if (poiSet.size() == 0)
					getAddressPart(vTemp1[1].c_str(), xzqhDict, roadDict,
							temp_communitySet, poiSet, hnSet, homeSet,
							floorSet);
				else
					getAddressPart(vTemp1[1].c_str(), xzqhDict, roadDict,
							temp_communitySet, temp_poiSet, temp_hnSet,
							temp_homeSet, temp_floorSet);
			} else {
				if (poiSet.size() == 0)
					getAddressPart(vTemp1[1].c_str(), xzqhDict, temp_roadDict,
							temp_communitySet, poiSet, hnSet, homeSet,
							floorSet);
				else
					getAddressPart(vTemp1[1].c_str(), xzqhDict, temp_roadDict,
							temp_communitySet, temp_poiSet, temp_hnSet,
							temp_homeSet, temp_floorSet);
			}
		} else {
			if (roadDict.size() == 0) {
				if (poiSet.size() == 0)
					getAddressPart(vTemp1[1].c_str(), temp_xzqhDict, roadDict,
							temp_communitySet, poiSet, hnSet, homeSet,
							floorSet);
				else
					getAddressPart(vTemp1[1].c_str(), temp_xzqhDict, roadDict,
							temp_communitySet, temp_poiSet, temp_hnSet,
							temp_homeSet, temp_floorSet);
			} else {
				if (poiSet.size() == 0)
					getAddressPart(vTemp1[1].c_str(), temp_xzqhDict,
							temp_roadDict, temp_communitySet, poiSet, hnSet,
							homeSet, floorSet);
				else
					getAddressPart(vTemp1[1].c_str(), temp_xzqhDict,
							temp_roadDict, temp_communitySet, temp_poiSet,
							temp_hnSet, temp_homeSet, temp_floorSet);
			}
		}
	}
}

