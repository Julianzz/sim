#include "process_main.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

#include "rules.h"
#include "tools.h"
#include "init.h"
#include "prepare.h"
#include "utils/MapTools.h"
#include "utils/computeAddressSimilarity.h"
#include "utils/classify_data/classify_data.h"
#include "utils/classify_data/string_process.h"

ProcessResource* ProcessResource::instance_ = NULL ;
ProcessResource& ProcessResource::instance() {
    if( instance_ == NULL ) {
        instance_ = new ProcessResource();
    }
    return *instance_;
}

bool OnlineProcess::initTools() {
    return true;
}

bool OnlineProcess::initResource() {
    
    Resource& resource_ = ProcessResource::instance().resource() ;

	string path = sourceDir_;
	string tempPath("");
	_vector tempVec;

	init_group_score(resource_.m_group_score, resource_.m_sigle_score);

	// pinyin
	tempPath = path + std::string("/pinyin.dic");
	if (load(tempPath, resource_.pinyin, "\t") == -1) {
		std::cout << "pinyin init failed." << std::endl;
		return false;
	}

	// some special words that need filte
	tempVec.clear();
	tempVec.push_back(path + std::string("/measure.dic"));
	tempVec.push_back(path + std::string("/states.dic"));
	if (load(tempVec, resource_.filter) == -1) {
		std::cout << "filter init failed" << std::endl;
		return false;
	}
    
	// measure
	tempVec.clear();
	tempVec.push_back(path + std::string("/measure.dic"));
	if (load(tempVec, resource_.measure) == -1) {
		std::cout << "measure init failed" << std::endl;
		return false;
	}

	// need prefix
	tempVec.clear();
	tempVec.push_back(path + std::string("/needPrefix.dic"));
	if (load(tempVec, resource_.needPrefix) == -1) {
		std::cout << "needPrefix init failed" << std::endl;
		return false;
	}

	// dp mark
	tempVec.clear();
	tempVec.push_back(path + std::string("/DPMark.dic"));
	if (load(tempVec, resource_.DPMark) == -1) {
		std::cout << "DPMark word init failed" << std::endl;
		return false;
	}

	//====================
	// door
	tempVec.clear();
	tempVec.push_back(path + std::string("/door.dic"));
	if (load(tempVec, resource_.doorSet) == -1) {
		std::cout << "door init failed" << std::endl;
		return false;
	}

	// for strategy to find xzqh word
	tempVec.clear();
	tempVec.push_back(path + std::string("/xzqh.dic"));
	if (load(tempVec, resource_.xzqh) == -1) {
		std::cout << "xzqh init failed" << std::endl;
		return false;
	}

	// idf mapping

	tempPath = path + std::string("/idf.dic");
	if (load(tempPath, resource_.idf, "\t", false) == -1) {
		std::cout << "idf init failed." << std::endl;
		return false;
	}
	findMaxMin(resource_.idf, resource_.max_idf, resource_.min_idf);
	cout << "max idf:" << resource_.max_idf << "\t" << resource_.min_idf
			<< endl;
	cout << "idf init:" << resource_.idf.size() << endl;


	// trans name mapping
	tempVec.clear();
	tempVec.push_back(path + std::string("/trans.name.dic"));
	if (loadTrans(tempVec, resource_.transName, "\t", true) == -1) {
		std::cout << "transName init failed." << std::endl;
		return false;
	}

	// trans type mapping
	tempVec.clear();
	tempVec.push_back(path + std::string("/trans.type.dic"));
	if (loadTrans(tempVec, resource_.transType, "\t", true) == -1) {
		std::cout << "transType init failed." << std::endl;
		return false;
	}

	// trans word mapping
	tempVec.clear();
	tempVec.push_back(path + std::string("/trans.word.dic"));
	tempVec.push_back(path + std::string("/trans.number.dic"));
	if (loadTrans(tempVec, resource_.transWord, "\t", true) == -1) {
		std::cout << "transWord init failed." << std::endl;
		return false;
	}

	// for strategy to find endwith
	tempPath = path + std::string("/endwith.dic");
	if (load(tempPath, resource_.endList) == -1) {
		std::cout << "endList init failed." << std::endl;
	}

	// for strategy to find mark word in same part
	tempVec.clear();
	tempVec.push_back(path + std::string("/mark.dic"));
	tempVec.push_back(path + std::string("/school.dic"));
	tempVec.push_back(path + std::string("/office.dic"));
	if (load(tempVec, resource_.markWord) == -1) {
		std::cout << "markWord init failed" << std::endl;
		return false;
	}

	// for strategy to find district word in same part
	tempVec.clear();
	tempVec.push_back(path + std::string("/road.dic"));
	if (load(tempVec, resource_.district) == -1) {
		std::cout << "district init failed" << std::endl;
		return false;
	}

	// for strategy to find mark word in bracket
	tempVec.clear();
	tempVec.push_back(path + std::string("/inBracketMark.dic"));
	if (load(tempVec, resource_.inBracketMark) == -1) {
		std::cout << "inBracketMark word init failed" << std::endl;
		return false;
	}

	// for strategy to find road word
	tempVec.clear();
	tempVec.push_back(path + std::string("/road.dic"));
	if (load(tempVec, resource_.road) == -1) {
		std::cout << "road word init failed" << std::endl;
		return false;
	}

	// for word end with endlist but not need to remove
	tempVec.clear();
	tempVec.push_back(path + std::string("/undel.dic"));
	if (load(tempVec, resource_.undel) == -1) {
		std::cout << "undel word init failed" << std::endl;
		return false;
	}

	// for number
	tempVec.clear();
	tempVec.push_back(path + std::string("/number.dic"));
	if (load(tempVec, resource_.number) == -1) {
		std::cout << "number word init failed" << std::endl;
		return false;
	}

	tempVec.clear();
	tempVec.push_back(path + std::string("/number2.dic"));
	if (load(tempVec, resource_.number2) == -1) {
		std::cout << "number2 word init failed" << std::endl;
		return false;
	}

	// for word end with delBigram but not need to remove
	tempVec.clear();
	tempVec.push_back(path + std::string("/delBigram.dic"));
	if (load(tempVec, resource_.delBigram) == -1) {
		std::cout << "delBigram word init failed" << std::endl;
		return false;
	}

	// for word end with delBigram but not need to remove
	tempVec.clear();
	tempVec.push_back(path + std::string("/spec.dic"));
	if (load(tempVec, resource_.spec) == -1) {
		std::cout << "spec word init failed" << std::endl;
		return false;
	}

	_hashset tempSet;
	transWord(resource_.spec, tempSet, resource_.transWord);
	resource_.spec = tempSet;

	// for word end with delBigram but not need to remove
	tempVec.clear();
	tempVec.push_back(path + std::string("/suffix.dic"));
	if (load(tempVec, resource_.suffix) == -1) {
		std::cout << "spec word init failed" << std::endl;
		return false;
	}

	// for word end with delBigram but not need to remove
	tempPath = path + std::string("/typeword");
	if (load(tempPath, resource_.typeword) == -1) {
		std::cout << "type word init failed" << std::endl;
		return false;
	}

	// name only have type word
	_mapSetit tempSetit;
	_hashsetit temphashit;
	for (tempSetit = resource_.typeword.begin();
			tempSetit != resource_.typeword.end(); ++tempSetit) {
		if (tempSetit->first != "00") {
			temphashit = tempSetit->second.begin();
			for (; temphashit != tempSetit->second.end(); ++temphashit) {
				resource_.typeNameMap.insert(make_pair(*temphashit, 100.0));
			}
		}
	}

	// word that must both in two poi
	tempPath = path + std::string("/unlackword");
	if (load(tempPath, resource_.unlackword) == -1) {
		std::cout << "unlack word init failed" << std::endl;
		return false;
	}

	_mapSet tempMapSet;
	transWord(resource_.unlackword, tempMapSet, resource_.transWord);
	resource_.unlackword = tempMapSet;

	// obj name map
	// must init after transword
	tempVec.clear();
	tempVec.push_back(path + std::string("/obj.dic"));
	if (load(tempVec, resource_.objNameMap, "\t", false) == -1) {
		std::cout << "obj init failed" << std::endl;
		return false;
	}

	_mapStrDbl tempMapStrDbl;
	transWord(resource_.objNameMap, tempMapStrDbl, resource_.transWord);
	resource_.objNameMap = tempMapStrDbl;

	// obj type
	if (loadObjType(tempVec, resource_.objTypeMap, "\t") == -1) {
		std::cout << "obj type init failed" << std::endl;
		return false;
	}

	_map tempObjTypeMap;
	transWord(resource_.objTypeMap, tempObjTypeMap, resource_.transWord);
	resource_.objTypeMap = tempObjTypeMap;

	// spec obj name map
	// must init after transword
	tempVec.clear();
	tempVec.push_back(path + std::string("/specObj.dic"));
	if (load(tempVec, resource_.specObjMap, "\t", false) == -1) {
		std::cout << "obj init failed" << std::endl;
		return false;
	}

	tempMapStrDbl.clear();
	transWord(resource_.specObjMap, tempMapStrDbl, resource_.transWord);
	resource_.specObjMap = tempMapStrDbl;

	// name tail
	tempVec.clear();
	tempVec.push_back(path + std::string("/nameTail.dic"));
	if (load(tempVec, resource_.nameTailMap, "\t", false) == -1) {
		std::cout << "name tail init failed." << std::endl;
		return false;
	}

	// bigram no mean name tail
	tempVec.clear();
	tempVec.push_back(path + std::string("/noMeanBigramTail.dic"));
	if (load(tempVec, resource_.noMeanBgmTailSet) == -1) {
		std::cout << "no mean bigram tail init failed." << std::endl;
		return false;
	}

	_mapit name_tail_it = resource_.nameTailMap.begin();
	for (; name_tail_it != resource_.nameTailMap.end(); ++name_tail_it) {
		resource_.noMeanBgmTailSet.insert(name_tail_it->first);
	}

	//====================
	// area tail
	tempVec.clear();
	tempVec.push_back(path + std::string("/areaTail.dic"));
	if (load(tempVec, resource_.areaTailSet) == -1) {
		std::cout << "area tail init failed." << std::endl;
		return false;
	}

	// type map
	tempVec.clear();
	tempVec.push_back(path + std::string("/typemap.dic"));
	if (loadTypeMap(tempVec, resource_.typeMap, "\t") == -1) {
		std::cout << "type map init failed" << std::endl;
		return false;
	}

	// diff word
	// must init after trans word
	tempVec.clear();
	tempVec.push_back(path + std::string("/diff.dic"));
	if (load(tempVec, resource_.diff) == -1) {
		std::cout << "diff word init failed" << std::endl;
		return false;
	}

	tempSet.clear();
	transWord(resource_.diff, tempSet, resource_.transWord);
	resource_.diff = tempSet;

	// same mean name
	tempPath = path + std::string("/smName.dic");
	if (load(tempPath, resource_.smNameMap, "\t") == -1) {
		std::cout << "same mean name init failed" << std::endl;
		return false;
	}

	// remove type word name data
	tempVec.clear();
	tempVec.push_back(path + std::string("/removeTypeName.dic"));
	if (load(tempVec, resource_.removeTypeNameSet) == -1) {
		std::cout << "removeTypeNameSet init failed" << std::endl;
		return false;
	}

	// district trans map
	tempVec.clear();
	tempVec.push_back(path + std::string("/trans.district.dic"));
	if (load(tempVec, resource_.districtTransMap, "\t", false) == -1) {
		std::cout << "district trans map init failed." << std::endl;
		return false;
	}

	// classity category
	tempPath = path + std::string("/classify/cate_code.txt");
	GetCateCode1(tempPath.c_str(), resource_.cateCodeMap);

	tempPath = path + std::string("/classify/");
	GetKeyCategory(tempPath);

	// state
	tempVec.clear();
	tempVec.push_back(path + std::string("/states.dic"));
	if (load(tempVec, resource_.stateSet) == -1) {
		std::cout << "state init failed" << std::endl;
		return false;
	}

	// province
	tempVec.clear();
	tempVec.push_back(path + std::string("/province.dic"));
	if (load(tempVec, resource_.provinceSet) == -1) {
		std::cout << "province init failed" << std::endl;
		return false;
	}

	// area type
	resource_.areaTypeSet.insert("26");
	resource_.areaTypeSet.insert("28");
	resource_.areaTypeSet.insert("12");

	// direction
	resource_.directionSet.insert("东");
	resource_.directionSet.insert("南");
	resource_.directionSet.insert("西");
	resource_.directionSet.insert("北");
	resource_.directionSet.insert("东北");
	resource_.directionSet.insert("东南");
	resource_.directionSet.insert("西北");
	resource_.directionSet.insert("西南");

	// gov tail
	resource_.govTailSet.insert("区");
	resource_.govTailSet.insert("县");
	resource_.govTailSet.insert("乡");
	resource_.govTailSet.insert("村");
	resource_.govTailSet.insert("镇");
	resource_.govTailSet.insert("街");
	resource_.govTailSet.insert("道");
	resource_.govTailSet.insert("路");
	resource_.govTailSet.insert("桥");
	resource_.govTailSet.insert("巷");

	// 地支
	resource_.dizhiSet.insert("甲");
	resource_.dizhiSet.insert("乙");
	resource_.dizhiSet.insert("丙");
	resource_.dizhiSet.insert("丁");
	resource_.dizhiSet.insert("戊");
	resource_.dizhiSet.insert("己");
	resource_.dizhiSet.insert("庚");
	resource_.dizhiSet.insert("辛");
	resource_.dizhiSet.insert("壬");
	resource_.dizhiSet.insert("癸");
    
    return true;
}
