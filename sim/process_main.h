#ifndef __ONLINE_CONFIG_H__
#define __ONLINE_CONFIG_H__

#include "json/json.h"
#include "process_rule.h"
#include "process_strategy.h"
#include "process_batch.h"
#include "process_sync.h"
#include "process_output.h"

#include "sim.h"

#include <assert.h> 
#include <string>
#include <fstream>
#include "sim.h"

#define DEFAULT_WORKERS 5

class PoiSimCfg;
class Tools;
class ResultItem;

class OnlineProcess {
    
private:
    std::string fileName_;
    std::ifstream ifs_;
    Json::Value configs_;
    
    std::string sourceDir_;
    std::vector<std::string> wordSplitDicVec_;
    
    std::map<std::string,ProcessStrategy>  strategies_;    
        
public:
    
    OnlineProcess(const std::string& name );
    ~OnlineProcess();
    Resource& getResource() {
        return ProcessResource::instance().resource();
    }
    
public:
    bool init();
    void dump();
    
private:
    
    bool initResource();
    bool initTools();
    
public:
    bool batchProcess( const std::string& strategy, const std::string& baseName, 
                    const std::string& incName );
         
    bool compute( const std::string& strategy,const Record& first, 
                    const Record& second, ResultItem* result ) ;
    
};

class ProcessEngine;

class BatchProcess:public BatchWorker {
public:
    friend class ProcessEngine;
    
protected:
    BatchRecords* baseRecords_;
    BatchRecords* incRecords_;
    
    ProcessStrategy& strategy_;
    size_t batchNum_;
    
    Locker writeLocker_;
    BasicOutput* output_;
    
    ProcessEngine* baseEngine_;
    ProcessEngine* incEngine_;
    
public:
    
    BatchProcess(ProcessStrategy& s,size_t batchNum = DEFAULT_WORKERS );
    virtual ~BatchProcess();
    
    bool loadFinish();
    bool init(const std::string& base,const std::string inc );
        
    virtual bool process() ;
    virtual bool process(const PreProcData& base, BatchRecords& records, bool isBase);
    
    /*virtual bool processSim(const PreProcData& record, BatchRecords& records,
         geo::GridResult& sameIds,bool isBase);*/
             
    virtual bool processOutput(StrategyResult& result,bool isBase);
    
public:
    ProcessStrategy& getStrategy() {
        return strategy_;
    }
    
public:
    DISALLOW_COPY_AND_ASSIGN(BatchProcess);
};


#endif