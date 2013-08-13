#include "process_geo_search.h"
#include "sim.h"
#include "data.h"

namespace geo {
    
void GridSearch::put (const GridKey& key, const GridItem& item ) {
            
    GridAround& around = gridMap_[key];
    around.insert(item);
}

void GridSearch::put (const PreProcData& preProcData, const Resource& resource ) {
    
    const std::set<std::string>& gridNames = preProcData.m_grid_name_set; 
	std::set<std::string>::const_iterator wordsIter = gridNames.begin();
	for ( ;wordsIter != gridNames.end(); ++wordsIter) {
        
		if (!startswith(preProcData.m_type, "27")
            && resource.delBigram.find(*wordsIter) != resource.delBigram.end()) {
			continue;
		}    	
        GridKey tempKey(preProcData.m_lat,preProcData.m_lng, *wordsIter );
        put(tempKey, preProcData.m_id);
	}
    
}

void GridSearch::init(const PreProcMap& preProcMap, const Resource& resource ) {
	PreProcMap::const_iterator it = preProcMap.begin();
	for (; it != preProcMap.end(); ++it) {
		put(it->second, resource);
	}
}

// 根据key获取周边数据
void GridSearch::search(const PreProcData &preProcData, 
        const GridKey &key,
		int level,
        GridResult& resultSet ) {
            
	GridKey tempKey = key.level(level);

	set<unsigned long long> tempSet;

	GridMap::const_iterator it;
	for (int i = 0; i <= 2 * level; i++) {
		tempKey.lng = key.lng - level;
		for (int j = 0; j <= 2 * level; j++) {
            
			it = gridMap_.find(tempKey);
			if (it != gridMap_.end()) {
				tempSet.insert(it->second.begin(), it->second.end());
			}
			tempKey.lng += 1;
		}
		tempKey.lat += 1;
	}

	if (tempSet.size() < 500 || preProcData.m_del_punc_name_len <= 4) {
		resultSet.insert(tempSet.begin(), tempSet.end());
	}
    
}

void GridSearch::search (const PreProcData &preProcData,
        int level,
        GridResult& resultSet ) {
            
	resultSet.clear();
    
    const std::set<std::string>& gridNames = preProcData.m_grid_name_set; 
	std::set<std::string>::const_iterator wordsit = gridNames.begin();
	for (; wordsit != gridNames.end(); ++wordsit) {
		if (*wordsit != "") {
        	GridKey tempKey(preProcData.m_lat, preProcData.m_lng, *wordsit ) ;
			search(preProcData, tempKey, level, resultSet );
		}
	}
}

}