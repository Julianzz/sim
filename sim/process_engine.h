#ifndef PROCESS_ENGINE_H_RJYU6E25
#define PROCESS_ENGINE_H_RJYU6E25

#include <string>
#include <iostream>
#include "process_geo_search.h"

class BatchProcess;
class PreProcData;
class BatchRecords;

class ProcessEngine {
protected:
    BatchProcess& process_;
public:
    ProcessEngine(BatchProcess& process):process_(process) {} 
    virtual ~ProcessEngine() {} ;
    virtual bool processSim(const PreProcData& record,BatchRecords& records, 
        geo::GridResult& sameIds, bool isBase) = 0;
};


class AllMatchBatchEngine: public ProcessEngine {
    
public:
    AllMatchBatchEngine(BatchProcess& process):
        ProcessEngine(process) {} 
    virtual ~AllMatchBatchEngine() {
        //std::cout<<"destroy batch process" <<std::endl;
    }
    virtual bool processSim(const PreProcData& record,BatchRecords& records, 
        geo::GridResult& sameIds, bool isBase);    
};

class StrategyResult;

class MaxMatchBatchEngine: public ProcessEngine {
public:
    MaxMatchBatchEngine(BatchProcess& process):
        ProcessEngine(process) {} 
        
    virtual ~MaxMatchBatchEngine() {
        //std::cout<<"destroy batch process" <<std::endl;
    }
public:
    virtual bool processSim(const PreProcData& record,BatchRecords& records, 
        geo::GridResult& sameIds, bool isBase);   
    bool compareSimScore(StrategyResult& cur, StrategyResult& temp) ;
 
};


class FirstMatchBatchEngine:public ProcessEngine {
public:
    FirstMatchBatchEngine(BatchProcess& process):
        ProcessEngine(process) {} 
    virtual ~FirstMatchBatchEngine() {
        //std::cout<<"destroy batch process" <<std::endl;
    }
public:
    virtual bool processSim(const PreProcData& record,BatchRecords& records, 
        geo::GridResult& sameIds, bool isBase );  

};

#endif /* end of include guard: PROCESS_ENGINE_H_RJYU6E25 */
