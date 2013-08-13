#ifndef PROCESS_GEO_SEARCH_H_NX934AVH
#define PROCESS_GEO_SEARCH_H_NX934AVH
#include <string>
#include <map>
#include <set>
#include "sim.h"


namespace geo {

struct GridKey {
	int lat;
	int lng;
	std::string keyword;

	GridKey(double t,double g,const std::string& key): keyword(key) {
        lat = getLatKey(t);
        lng = getLngKey(g);
	}
    
	GridKey(): lat(0.0),lng(0.0),keyword("") {
	}
    
    GridKey level( int level ) const {
    	GridKey tempKey ;

    	tempKey.lat = this->lat - level;
    	tempKey.lng = this->lng - level;
    	tempKey.keyword = this->keyword;
        return tempKey;
        
    }
    int getLatKey(double lat) {
    	return int(lat * 500) / 15;
    }

    int getLngKey(double lng) {
    	return int(lng * 500) / 10;
    }
    
	bool operator<(const GridKey& key2) const  {

    	if (this->lat < key2.lat) {
    		return true;
    	}
        if (this->lat > key2.lat ) {
            return false;
        }
        
        // lat is equal
		if (this->lng < key2.lng) {
			return true;
        }
		if (this->lng > key2.lng) {
			return false;
        }
        
        //lat lng is equal
		if (this->keyword.compare(key2.keyword) < 0) {
			return true;
        } else { 
			return false;
        }
	}
};

typedef unsigned long long GridItem ;
typedef std::set<GridItem> GridAround;
typedef std::map< GridKey, GridAround > GridMap;
typedef std::set< GridItem > GridResult; 


class GridSearch {
        
public:
    GridSearch () {} ;
    virtual ~GridSearch () {};

public:
    void init(const PreProcMap& preProcMap, const Resource& resource );
    
    void put (const GridKey& key, const GridItem& value );
    void put (const PreProcData& preProcData, const Resource& resource );

    // 根据key获取周边数据
    void search(const PreProcData &preProcData, 
            const GridKey &key,
    		int level,
            GridResult& resultSet );

    void search(const PreProcData &preProcData,
            int level,
    		GridResult &resultSet );
    
private:
    GridMap gridMap_;
};

}

#endif /* end of include guard: PROCESS_GEO_H_NX934AVH */
