#ifndef TYPEDEFINE_H_
#define TYPEDEFINE_H_

#include <string>
#include <map>
#include <vector>
#include <set>
#include <tr1/unordered_set>
#include <tr1/unordered_map>
#include "utils/wordsplit/wordsplit.h"
#include <pthread.h>
#include <unistd.h>

typedef std::set<std::string> _set;
typedef _set::iterator _setit;

//typedef std::tr1::unordered_map<std::string, std::string> _map;
//typedef _map::iterator _mapit;

typedef std::map<std::string, std::string> _map;
typedef _map::iterator _mapit;

typedef std::map<std::string, int> _mapInt;
typedef _mapInt::iterator _mapIntit;

typedef std::map<std::string, set<int> > _mapStrIntSet;
typedef _mapStrIntSet::iterator _mapStrIntSetit;

//typedef std::tr1::unordered_set<std::string> _hashset;
//typedef _hashset::iterator _hashsetit;

typedef std::set<std::string> _hashset;
typedef _hashset::iterator _hashsetit;

//typedef std::tr1::unordered_map<std::string, double> _mapStrDbl;
//typedef _mapStrDbl::iterator _mapStrDblit;

typedef std::map<std::string, double> _mapStrDbl;
typedef _mapStrDbl::iterator _mapStrDblit;

typedef std::vector<std::string> _vector;
typedef _vector::iterator _vectorit;

//typedef std::tr1::unordered_map<std::string, _vector> _mapVec;
//typedef _mapVec::iterator _mapVecit;

typedef std::map<std::string, _vector> _mapVec;
typedef _mapVec::iterator _mapVecit;

//typedef std::tr1::unordered_map<std::string, _hashset> _mapSet;
//typedef _mapSet::iterator _mapSetit;

typedef std::map<std::string, _hashset> _mapSet;
typedef _mapSet::iterator _mapSetit;

//typedef std::vector<double> _nameScoreVec;
//typedef nameScoreVec::iterator _nameScoreVecit;

typedef std::vector<int> _ivec;
typedef _ivec::iterator _ivecit;

typedef std::vector<double> _dvec;
typedef _dvec::iterator _dvecit;

typedef map<size_t, _dvec> _scoreMap;
typedef _scoreMap::iterator _scoreMapit;

class IDF {
public:
	_mapStrDbl word_idf_map;
	unsigned int poi_record_num;
	double max_idf;
	double min_idf;

	void clear() {
		word_idf_map.clear();
		poi_record_num = 0;
		max_idf = 0.0;
		min_idf = 0.0;
	}

	IDF(){
		clear();
	}
};

class POIRecord {
public:
	unsigned long long id;
	std::string name;
	std::string address;
	std::string splitaddr;
	std::string addrlevel;
	std::string telephone;
	std::string type;
	std::string main_type;
	std::string sub_type;
	std::string source;
	_vector district;
	std::string sim_type;
//	std::string city;
//	std::string province;
//	std::string district;
	double lat;
	double lng;

	void clear() {
		id = 0;
		name = "";
		address = "";
		splitaddr = "";
		addrlevel = "";
		telephone = "";
		type = "";
		main_type = "";
		sub_type = "";
		source = "";
		district.clear();
		lat = 0.0;
		lng = 0.0;
		sim_type = "";
	}

	POIRecord(){
		clear();
	}

	POIRecord &operator=(POIRecord &src) {
		id = src.id;
		name = src.name;
		address = src.address;
		splitaddr = src.splitaddr;
		addrlevel = src.addrlevel;
		telephone = src.telephone;
		type = src.type;
		main_type = src.main_type;
		sub_type = src.sub_type;
		source = src.source;
		district = src.district;
		lat = src.lat;
		lng = src.lng;
		sim_type = src.sim_type;
		return *this;
	}
};

class OriNameCacheResult {
public:
	_vector nameWordList; // wordsplit result
	_set nameWordSet; // wordsplit result in set

	void clear() {
		nameWordList.clear();
		nameWordSet.clear();
	}

	OriNameCacheResult(){
		clear();
	}

	OriNameCacheResult &operator=(OriNameCacheResult &src) {
		nameWordList = src.nameWordList;
		nameWordSet = src.nameWordSet;
		return *this;
	}
};

class NameCacheResult: public OriNameCacheResult {
public:
	_vector nameDiffList; // diffrent word from other name
	_vector nameDiffFilteList; // the filted word list that from different part
	_vector nameSameList; // move from different list, that use pinyin, same mean, and so on strategy

	void clear() {
		OriNameCacheResult::clear();
		nameDiffList.clear();
		nameDiffFilteList.clear();
		nameSameList.clear();
	}

	NameCacheResult(){
		clear();
	}

	NameCacheResult &operator=(NameCacheResult &src) {
		OriNameCacheResult::operator=(src);
		nameDiffList = src.nameDiffList;
		nameDiffFilteList = src.nameDiffFilteList;
		nameSameList = src.nameSameList;
		return *this;
	}

	NameCacheResult &operator=(OriNameCacheResult &src) {
		OriNameCacheResult::operator=(src);
		return *this;
	}
};


class SoSoAttr {
public:
	std::string soso_main_type;
	std::string soso_sub_type;
	std::string soso_type;
//	std::string suffix;
//	std::string shortName;
//	_vector shortNameList;
//	_set shortNameSet;
//	_vector suffixNameList;
//	_set suffixNameSet;

	void clear() {
		soso_main_type = "";
		soso_sub_type = "";
		soso_type = "";
//		suffix = "";
//		shortName = "";
//		shortNameList.clear();
//		shortNameSet.clear();
//		suffixNameList.clear();
//		suffixNameSet.clear();
	}

	SoSoAttr(){
		clear();
	}

	SoSoAttr &operator=(SoSoAttr &src) {
		soso_main_type = src.soso_main_type;
		soso_sub_type = src.soso_sub_type;
		soso_type = src.soso_type;
//		suffix = src.suffix;
//		shortName = src.shortName;
//		shortNameList = src.shortNameList;
//		shortNameSet = src.shortNameSet;
//		suffixNameList = src.suffixNameList;
//		suffixNameSet = src.suffixNameSet;
		return *this;
	}
};

class OriCacheResult {
public:
	unsigned long long id;
	//unsigned long long sosopoiid;
	double lat;
	double lng;

	std::string oriName; // original name string
	std::string source;
	std::string formatName; // format name string
	std::string delPuncName; // del all punc from name
	std::string objName;
	std::string bigramName;

	std::string splitaddr;
	std::string address;
//	std::string addrlevel;
	std::string type;
	std::string soso_type;
	std::string inner_type;
	_vector district;

	std::string main_type;
	std::string sub_type;

	_set numNameSet;
	_set doorSet;
	_set dpMarkSet;
	double objMaxDistance;
	_vector inBracketList; // after format, the deleted string that in brackets
//	_vector zhSingleWordList; // split chinese word to single word
//	_vector zhenSingleWordList; // split chinese and english to single word

	OriNameCacheResult name;
	OriNameCacheResult inBracketName;

	_set telephoneSet;


	//SoSoAttr sosoAttr;
	set<int> smNameGroupSet;

	string sim_type_code;
	string sim_keyword;
	string gov_tail;


	void clear() {
		id = 0;
		source = "";
		oriName = "";
		objName = "";
		formatName = "";
		delPuncName = "";
		bigramName = "";
		objMaxDistance = 1000.0;
		//sosopoiid = 0;
		inBracketList.clear();
//		zhSingleWordList.clear();
//		zhenSingleWordList.clear();
		numNameSet.clear();
		doorSet.clear();
		dpMarkSet.clear();

		name.clear();
		inBracketName.clear();

		telephoneSet.clear();
		smNameGroupSet.clear();

		soso_type = "";
		inner_type = "";
		//sosoAttr.clear();

		district.clear();
		splitaddr = "";
		address = "";
//		addrlevel = "";
		type = "";
		main_type = "";
		sub_type = "";

		lat = 0.0;
		lng = 0.0;

		sim_type_code = "";
		sim_keyword = "";
		gov_tail = "";
	}

	OriCacheResult(){
		clear();
	}

	OriCacheResult &operator=(OriCacheResult &src) {
		id = src.id;
		source = src.source;
		oriName = src.oriName;
		objName = src.objName;
		bigramName = src.bigramName;
		objMaxDistance = src.objMaxDistance;
		formatName = src.formatName;
		delPuncName = src.delPuncName;
		//sosopoiid = src.sosopoiid;
		numNameSet = src.numNameSet;
		doorSet = src.doorSet;
		dpMarkSet = src.dpMarkSet;
		inBracketList = src.inBracketList;
//		zhSingleWordList = src.zhSingleWordList;
//		zhenSingleWordList = src.zhenSingleWordList;
		name = src.name;
		inBracketName = src.inBracketName;
		telephoneSet = src.telephoneSet;
		smNameGroupSet = src.smNameGroupSet;

		soso_type = src.soso_type;
		//sosoAttr = src.sosoAttr;

		district = src.district;
		splitaddr = src.splitaddr;
		address = src.address;
//		addrlevel = src.addrlevel;
		type = src.type;
		main_type = src.main_type;
		sub_type = src.sub_type;
		inner_type = src.inner_type;

		lat = src.lat;
		lng = src.lng;

		sim_type_code =src.sim_type_code;
		sim_keyword = src.sim_keyword;
		gov_tail = src.gov_tail;
		return *this;
	}
};

class CacheResult: public OriCacheResult {
public:
	//POIRecord ori;
	SoSoAttr sosoAttr;

	size_t delPuncNameLen;

	NameCacheResult name;
	NameCacheResult inBracketName;

	void clear() {
		OriCacheResult::clear();
		sosoAttr.clear();
		delPuncNameLen = 0;
		//ori.clear();
		name.clear();
		inBracketName.clear();
	}

	CacheResult(){
		clear();
	}

	CacheResult &operator=(CacheResult &src) {
		OriCacheResult::operator=(src);
		//ori = src.ori;
		sosoAttr = src.sosoAttr;
		delPuncNameLen = src.delPuncNameLen;
		name = src.name;
		inBracketName = src.inBracketName;
		return *this;
	}

	CacheResult &operator=(OriCacheResult &src) {
		OriCacheResult::operator=(src);
		name = src.name;
		inBracketName = src.inBracketName;
		return *this;
	}
};

class ShareResult {
public:
	std::string lcsName;
	std::string type;
	double addrSim;
	bool nameInBracket;
	std::string objName;

	_vector nameShareList;
	_vector nameInBraShareList;

	_set telShareSet;

	void clear() {
		lcsName = "";
		type = "";
		objName = "";
		addrSim = 0.0;
		nameInBracket = false;
		nameShareList.clear();
		nameInBraShareList.clear();

		telShareSet.clear();
	}

	ShareResult(){
		clear();
	}
};

class Resource {
public:
	_mapVec pinyin; // pinyin
	_mapStrDbl idf; // idf table, key is string, value is double
	_hashset filter; // filter set
	_map transName; // trans name mapping
	_map transWord; // trans word mapping
	_map transType;

	_hashset endList; // special word that name end with
	_hashset markWord; // type word use to mark
	_hashset DPMark; // mark word for different part of name
	_hashset inBracketMark;
	_hashset district;
	_hashset road;
	_hashset measure;
	_hashset undel;
	_hashset xzqh;
	_hashset number;
	_hashset number2;
	_hashset delBigram;
	_hashset spec;
	_hashset needPrefix;
	_mapStrDbl objNameMap;
	//_vector prefix;
	_vector suffix;
	_hashset diff; // 如果某词出现在不同部分，认为不同，如售票处

	_mapSet typeword;
	_mapSet unlackword;
	_mapSet typeMap;
	_map objTypeMap;
	_mapStrDbl specObjMap;

	_map nameTailMap;
	_hashset areaTailSet;
	_mapInt uselessTelMap;
	_mapStrDbl typeNameMap;
	_mapStrIntSet smNameMap;
	_hashset doorSet;
	_hashset removeTypeNameSet;
	_hashset noMeanBgmTailSet; // bigram分解后无意义的末尾类型词
	_map districtTransMap; // 将 “新疆维吾尔自治区” 转为 “新疆”
	set<string> areaTypeSet;
	set<string> directionSet;
	set<string> govTailSet;
	map<string, string> cateCodeMap;
	set<string> stateSet;
	set<string> dizhiSet; // 地支：甲乙丙丁
	set<string> provinceSet; // 省份名称

	map<string,double> m_group_score;
	map<string,double> m_sigle_score;

	double max_idf;
	double min_idf;

	void clear() {
		pinyin.clear();
		idf.clear();
		filter.clear();
		transName.clear();
		transWord.clear();
		transType.clear();
		endList.clear();
		markWord.clear();
		DPMark.clear();
		inBracketMark.clear();
		district.clear();
		road.clear();
		measure.clear();
		undel.clear();
		xzqh.clear();
		number.clear();
		number2.clear();
		delBigram.clear();
		spec.clear();
		needPrefix.clear();
		objNameMap.clear();
		//prefix.clear();
		suffix.clear();
		typeword.clear();
		unlackword.clear();
		typeMap.clear();
		nameTailMap.clear();
		areaTailSet.clear();
		objTypeMap.clear();
		uselessTelMap.clear();
		typeNameMap.clear();
		smNameMap.clear();
		doorSet.clear();
		removeTypeNameSet.clear();
		noMeanBgmTailSet.clear();
		districtTransMap.clear();
		areaTypeSet.clear();
		directionSet.clear();
		govTailSet.clear();
		cateCodeMap.clear();
		stateSet.clear();
		dizhiSet.clear();
		provinceSet.clear();
		m_group_score.clear();
		m_sigle_score.clear();
		max_idf = 0.0;
		min_idf = 999.0;
	}

	Resource(){
		clear();
	}
};

class NameScore {
public:

	double diffScore; // score about nameDiffList
	double diffSubScore; // score about nameDiffFilteList
	double sameScore; // score about shareList
	double sameSubScore; // score about nameSameList

	double c_nameNormal; // (a * sameScore + b * sameSubScore + c) - (d * diffScore + e * diffSubScore + f)
	double c_nameSame; // if c_nameNormal > 0, c_nameSame = 1, if c_nameNormal = 0, c_nameSame = 0, else c_nameSame = -1
	double c_nameSameRate; // (a * sameScore + b * sameSubScore + c) / [(a * sameScore + b * sameSubScore + c)  + (d * diffScore + e * diffSubScore + f)]
	double c_nameDiffRate; //  (d * diffScore + e * diffSubScore + f) / [(a * sameScore + b * sameSubScore + c)  + (d * diffScore + e * diffSubScore + f)]

	double c_nameWeight; // c_nameSame * c_nameRate
	int c_nameConfirm; // if score in one part are all 0, example: diffScore = 0, diffSubScore = 0, and sameScore or sameSubScore is not 0, c_nameConfirm = 1, else c_nameConfirm = -1, if all part score are not all 0, c_nameConfirm = 0;
	int c_nameMainConfirm; // if score in main part are confirm. example: diffScore = 0, sameScore != 0, no matter others are what, c_nameMainConfirm = 1, else c_nameMainConfirm = -1, if both are 0, c_nameMainConfirm = 0;

	size_t singleCount; // single chinese word count
	size_t wordCount; // all word in list count

	void clear() {
		diffScore = 0.0;
		diffSubScore = 0.0;
		sameScore = 0.0;
		sameSubScore = 0.0;

		c_nameNormal = 0.0;
		c_nameSame = 0.0;
		c_nameSameRate = 0.0;
		c_nameDiffRate = 0.0;
		c_nameWeight = 0.0;
		c_nameConfirm = 0;
		c_nameMainConfirm = 0;

		singleCount = 0;
		wordCount = 0;
	}

	NameScore(){
		clear();
	}
};

/*
 class ChainScore: NameScore
 {
 public:
 bool nameScore; // true when is chain name
 int typeScore; // 1 confirm same, -1 confirm not same, 0 not confirm

 void clear():
 {
 NameScore::clear();
 nameScore = false;
 typeScore = 0;
 }
 };
 */

//===========================================================
//	value's detail mean in src/digital/strategyDigital.h
//===========================================================
class StrategyScore {
public:
	int s_nameSPEndWith;
	int s_nameSPMark;
	int s_nameSPRoad; // same part only a road word
	int s_nameSPAlpNumMeasure;
	int s_nameSPScore;
	int s_nameSubSPScore;
	int s_nameSPNull;

	int s_nameDPMark; // use DPMark.dic to check
	int s_nameDPMark2; // use mark.dic to check
	int s_nameDPOnePartNull; // 1 is null
	int s_nameDPScore; // diff part score more than threshold
	int s_nameDPFilterScore;
	int s_nameDPRoad;
	int s_nameDPAlpNumMeasure;
	int s_nameDPType;
	int s_nameDPEndWith;
	int s_nameDPOnlyXZQH;

	int s_nameSubNull; // all sub part is null
	int s_nameTailType; // last word type

	void clear() {
		s_nameSPEndWith = 0;
		s_nameSPMark = 0;
		s_nameSPRoad = 0;
		s_nameSPAlpNumMeasure = 0;
		s_nameSPScore = 0;
		s_nameSubSPScore = 0;

		s_nameDPMark = 0;
		s_nameDPOnePartNull = 0;
		s_nameDPScore = 0;
		s_nameDPFilterScore = 0;
		s_nameDPRoad = 0;
		s_nameDPAlpNumMeasure = 0;
		s_nameDPType = 0;
		s_nameDPEndWith = 0;
		s_nameDPOnlyXZQH = 0;

		s_nameSubNull = 0;
		s_nameTailType = 0;

	}

	StrategyScore(){
		clear();
	}
};

class BipartScore {
public:
	double diffScore; // score about different
	double diffSubScore; // score about dub different
	double sameScore; // score about same
	double sameSubScore; // score about sub same

	double c_normal; // (a * sameScore + b * sameSubScore + c) - (d * diffScore + e * diffSubScore + f)
	double c_same; // if c_nameNormal > 0, c_nameSame = 1, if c_nameNormal = 0, c_nameSame = 0, else c_nameSame = -1
	double c_sameRate; // (a * sameScore + b * sameSubScore + c) / [(a * sameScore + b * sameSubScore + c)  + (d * diffScore + e * diffSubScore + f)]
	double c_diffRate; //  (d * diffScore + e * diffSubScore + f) / [(a * sameScore + b * sameSubScore + c)  + (d * diffScore + e * diffSubScore + f)]

	double c_weight; // c_nameSame * c_nameRate
	int c_confirm; // if score in one part are all 0, example: diffScore = 0, diffSubScore = 0, and sameScore or sameSubScore is not 0, c_nameConfirm = 1, else c_nameConfirm = -1, if all part score are not all 0, c_nameConfirm = 0;
	int c_mainConfirm; // if score in main part are confirm. example: diffScore = 0, sameScore != 0, no matter others are what, c_nameMainConfirm = 1, else c_nameMainConfirm = -1, if both are 0, c_nameMainConfirm = 0;

	void clear() {
		diffScore = 0.0;
		diffSubScore = 0.0;
		sameScore = 0.0;
		sameSubScore = 0.0;

		c_normal = 0.0;
		c_same = 0.0;
		c_sameRate = 0.0;
		c_diffRate = 0.0;
		c_weight = 0.0;
		c_confirm = 0;
		c_mainConfirm = 0;
	}

	BipartScore(){
		clear();
	}

};

class SimScore {
public:
	int isChainScore; // 2 if one is and other is not, 1 if can judge yes, 0 can not judge, -1 judge not
	double calScore;
	double distance;
	double telScore;
	double addrScore;
	double tailScore; // if one in endswith other
	double cateScore;
	double simTypeScore;

	BipartScore defaultScore;
	//BipartScore lcsScore;

	NameScore nameScore;
	//NameScore nameLCSScore;

	//StrategyScore nameSTGScore;
	//StrategyScore nameSTGLCSScore;

	void clear() {
		isChainScore = 0;
		calScore = 0.0;
		distance = 0.0;
		telScore = 0.0;
		addrScore = 0.0;
		tailScore = 0.0;
		cateScore = 0.0;
		simTypeScore = 0.0;

		defaultScore.clear();
		//lcsScore.clear();

		nameScore.clear();
		//nameLCSScore.clear();

		//nameSTGScore.clear();
		//nameSTGLCSScore.clear();
	}

	SimScore(){
		clear();
	}
};

class Result {
public:
	double simScore;
	std::string computeStep;
	double telScore;
	unsigned int lenDiff;
	double distance;

	void clear() {
		simScore = 0.0;
		telScore = 0.0;
		computeStep = "";
		distance = 10000.0;
		lenDiff = 999;
	}

	Result &operator=(Result &src) {
		simScore = src.simScore;
		telScore = src.telScore;
		computeStep = src.computeStep;
		lenDiff = src.lenDiff;
		distance = src.distance;
        return *this;
	}

	Result(){
		clear();
	}
};

//typedef std::tr1::unordered_map<unsigned long long, CacheResult> cacheResultMap;
typedef map<unsigned long long, CacheResult> cacheResultMap;
//typedef map<unsigned long long, OriCacheResult> cacheResultMap;
typedef cacheResultMap::iterator cacheResultMapit;

#endif
