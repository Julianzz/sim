/*
 * LevelIO.h
 *
 *  Created on: 2012-8-30
 *      Author: lzz
 */

#ifndef __LEVELIO_H__
#define __LEVELIO_H__

#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "cpper/common.h"
     /*
#include "StorageReqParam.h"
#include "StorageResParam.h"
#include "StorageApiConfig.h"
#include "StorageApi.h"
#include "DBError.h"
     */
#include "basic_io.h"
#include "commontype.h"

using namespace std;
using namespace cpper::core;
using namespace cpper::collection;

class QueryResParam;
class QueryReqParam;
class StorageApi;

class CLevelIO {
    
public:
    CLevelIO(const string& conf ):m_conns_(NULL),
                    configFile_(conf),maxretrytime(3) { };
    ~CLevelIO(){
    	this->free();
    };
public:
    int init();    
    int connect();
    int reconnect();
    int reset();
    void free();
 
    int get(const IDType id, QueryResParam& oQueryResParam,QueryReqParam& oQueryReqParam); 
    int get(const string& tableName, const IDType id, 
            const vector<string>& fields, vector<string>& results );
    int get(const string& tableName, const IDType id,
             const vector<string>& fields, map<string,string>& results );
   
private:
    int build( const string& tableName, const IDType id, 
            const vector<string>& fields, QueryReqParam &oQueryReqParam );

private:
    StorageApi* m_conns_;
    std::string configFile_;
    int maxretrytime ;
};

class CLevelIOFactory {
public:
    CLevelIOFactory(const string& conf):config_(conf) {
    };

    CLevelIO* build() {
        CLevelIO* io =  new CLevelIO(config_);
        io->init();
        return io;
    };

    void release( CLevelIO* io_){
        delete io_;
    }
private:
    string config_; 
   
};


typedef class IOPool<CLevelIOFactory, CLevelIO> CLevelIOPool;


#endif /* LEVELIO_H_ */
