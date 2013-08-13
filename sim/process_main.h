#ifndef __ONLINE_CONFIG_H__
#define __ONLINE_CONFIG_H__

#include "json/json.h"
#include "process_rule.h"
#include "process_strategy.h"
#include "process_batch.h"
#include "process_sync.h"
#include "sim.h"

#include <assert.h> 
#include <string>
#include <fstream>
#include "sim.h"

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

class BatchProcess:public BatchWorker {
    
protected:
    BatchRecords* baseRecords_;
    BatchRecords* incRecords_;
    
    ProcessStrategy& strategy_;
    size_t batchNum_;
    
    Locker writeLocker_;
    
public:
    
    BatchProcess(ProcessStrategy& s,size_t batchNum = 1):
        baseRecords_(NULL),
        incRecords_(NULL),
        strategy_(s),
        batchNum_(batchNum) { 
    }
    virtual ~BatchProcess() {
        std::cout<<"destroy batch process" <<std::endl;
    }
    bool loadFinish();
    bool init(const std::string& base,const std::string inc );
    void output(StrategyResult& result, bool isBase);
    
    virtual bool process() ;
    virtual bool process(const PreProcData& base, BatchRecords& records, bool isBase);
    
    virtual bool processSim(const PreProcData& record, BatchRecords& records,
         geo::GridResult& sameIds,bool isBase);
    virtual bool processOutput(StrategyResult& result,bool isBase);
public:
    DISALLOW_COPY_AND_ASSIGN(BatchProcess);
};


class AllMatchBatchProcess: public BatchProcess {
    
public:
    AllMatchBatchProcess(ProcessStrategy& s,size_t batchNum = 1):
        BatchProcess(s,batchNum) {} 
    virtual ~AllMatchBatchProcess() {
        std::cout<<"destroy batch process" <<std::endl;
    }
};


class MaxMatchBatchProcess: public BatchProcess {
public:
    MaxMatchBatchProcess(ProcessStrategy& s,size_t batchNum = 1):
        BatchProcess(s,batchNum) {} 
    virtual ~MaxMatchBatchProcess() {
        std::cout<<"destroy batch process" <<std::endl;
    }
public:
    virtual bool processSim(const PreProcData& record,BatchRecords& records, 
        geo::GridResult& sameIds, bool isBase);    
};


class FirstMatchBatchProcess:public BatchProcess {
public:
    FirstMatchBatchProcess(ProcessStrategy& s,size_t batchNum = 1):
        BatchProcess(s,batchNum) {} 
    virtual ~FirstMatchBatchProcess() {
        std::cout<<"destroy batch process" <<std::endl;
    }
public:
    virtual bool processSim(const PreProcData& record,BatchRecords& records, 
        geo::GridResult& sameIds, bool isBase );  

};

#endif