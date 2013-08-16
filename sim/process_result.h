#ifndef PROCESS_RESULT_H_5IOPNSN4
#define PROCESS_RESULT_H_5IOPNSN4
#include "sim.h"
#include "json/json.h"

struct StrategyResult {
    
    SimTempCache simCache;
    //Json::Value results;
    //Record base;
    //Record inc;
    std::map<std::string, bool> results;
    
    //StrategyResult(): results(Json::objectValue) { } 
    StrategyResult() { }         
    void init( const PreProcData *base_data, const PreProcData *inc_data ,bool isBase);
    void dump();
    void setRuleInfo(const std::string& name, bool value );
    void setNotNull() {
        simCache.m_is_null = false;
    }
    
    std::string outputRule(bool isFull = false) const ;
};

#endif /* end of include guard: PROCESS_RESULT_H_5IOPNSN4 */
