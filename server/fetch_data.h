#ifndef __FETCH_DATA_H__
#define __FETCH_DATA_H__

#include <string>
#include <vector>
#include <map>

#include "commontype.h"

using namespace std;

class FetchService {
public:
    static int init( const string& config );
    static void release();
    
    static int fetchPP(const string id, vector<string>& results );
    static int fetchPP(const string id, map<string,string>& results );
};

#endif
