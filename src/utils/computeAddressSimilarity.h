/*
 * computeAddressSimilarity.h
 *
 *  Created on: 2010-8-31
 *      Author: pavelliu
 */

#ifndef __COMPUTE_ADDRESS_SIMILARITY_H__
#define __COMPUTE_ADDRESS_SIMILARITY_H__

#include <vector>
#include <map>
#include <set>
#include <string>
#include <stdint.h>

using namespace std;

#define xzqh_road_threshold 0.12
#define road_poi_threshold 0.15
#define community_poi_threshold 0.09 
#define poi_houseNum_threshold 0.10
#define xzqh_poi_threshold 0.115
#define xzqh_threshold 0.08
#define road_threshold 0.10
#define community_threshold 0.07
#define poi_threshold 0.105
#define houseNum_threshold 0.07


void init_group_score(map<string,double> &m_group_score,map<string,double> &m_sigle_score);
int32_t splitString(vector<string> &result, const char *pStr,
		const char *pSeparate);

double computeAddressSimilarity(const char * pAddress1, const char * pAddress2);

int getAddressLevel(const char * pAddress1);

int getSameAddressLevel(const char * pAddress1, const char * pAddress2);

int32_t getPart(const char *pSplitAddress, map<int, string> &xzqhDict
		,map<string, string> &roadDict,set<string> &poiDict);
int32_t getAddressPart(const char *pSplitAddress, map<int, string> &xzqhDict,
		map<string,string> &roadDict,set<string> &communitySet,set<string> &poiSet,set<string> &hnSet,
		set<string> &homeSet,set<string> &floorSet);
int32_t getGbkLen(const char * pData);

class AddressSimilarityValue
{
public:
	/*
	00:xzqhConflict;
	01:xzqhSame
	02:at lease one poi not have xzqh
	10:roadConflict
	11:roadSame
	12:at lease one poi not have road
	20:roadNumConflict
	21:roadNumSame
	22:at lease one poi not have roadNum
	30:poiConflict
	31:poiSame
	32:at lease one poi not have poiAddress
	40:houseNumConflict
	41:houseNumSame
	42:at lease one poi not have houseNum
	*/
	int32_t xzqhSimilarity;
	int32_t roadSimilarity;
	int32_t communitySimilarity;
	//int32_t roadNumSimilarity;
	int32_t poiSimilarity;
	int32_t houseNumSimilarity;
	int32_t level1;
	int32_t level2;

public:
	AddressSimilarityValue(){
		clear();
	}

	void clear(){
		xzqhSimilarity = 0;
		roadSimilarity = 0;
		communitySimilarity = 0;
		poiSimilarity = 0;
		houseNumSimilarity = 0;
		level1 = 0;
		level2 = 0;
	}
};

void splitAddressCorrection(const char* splitAddress,string &correctSplitAddress);
double computeAddressSimilarity(map<int, string> xzqhDict1
		, map<int, string> xzqhDict2, map<string, string> roadDict1
		,map<string, string> roadDict2, set<string> communityDict1,set<string> communityDict2,set<string> poiDict1
		,set<string> poiDict2,set<string> houseNumSet1,set<string> houseNumSet2,
		set<string> homeSet1,set<string> homeSet2,set<string> floorSet1,set<string> floorSet2
		,AddressSimilarityValue &myaddressSimilarityValue,map<string,double> m_group_score,map<string,double> m_sigle_score);
double computeTwoSplitAddressSimilarity(const char* splitAddress1,const char* splitAddress2,AddressSimilarityValue &myaddressSimilarityValue,map<string,double> m_group_score,map<string,double> m_sigle_score);

void getCorrectionAddressPart(const char* splitAddress,map<int, string> &xzqhDict,map<string,string> &roadDict,set<string> &communitySet,set<string> &poiSet,set<string > &hnSet,set<string> &homeSet,set<string> &floorSet);
#endif /* __COMPUTE_ADDRESS_SIMILARITY_H__ */
