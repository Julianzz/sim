#ifndef __PROCESS_STREGEGY_H__
#define __PROCESS_STREGEGY_H__

#include <string>
#include <vector>
#include <map>
#include "process_utils.h"
#include "process_input.h"
#include "sim.h"
#include "process_rule.h"
#include "json/json.h"

class PoiSimCfg;
class Tools;
class OnlineProcess;
class BaseRule;
class StrategyResult;    

class ProcessStrategy {
    
public:
    ProcessStrategy():
        process_(NULL),
        useLocalIdf_(true),
        parser_(NULL) { } 
    
    //clear up strategy
    virtual ~ProcessStrategy(){
        delRules(processRules_);
        delRules(initFirstRules_);
        delRules(initSecondRules_);
        if( parser_ != NULL ) {
            delete parser_;
        }
    }
    
    void delRules( std::vector<BaseRule*>& rules_ ) {
        std::vector<BaseRule*>::iterator iter  = rules_.begin();
        for( ;iter != rules_.end(); iter++ ) {
            BaseRule*rule  =  *iter;
            delete rule;
        }
        rules_.clear();
    }
    
    bool init( const string& fileName,OnlineProcess& process );
    
public:
    
    bool initRecord(std::vector<BaseRule*>& rules,const Record& record, PreProcData *baseData );
    bool initRecord(const Record& record, PreProcData *baseData,bool isBase );
    
    void sim(const PreProcData *base_data, const PreProcData *inc_data, StrategyResult *results );
    void sim( const Record& base, const Record& inc, StrategyResult *results ) ;
    
public:
    std::vector<BaseRule*>& getProcessRule() {
        return processRules_;
    }
    
    PoiSimCfg& getSimConfig() {
        return simConfig_;
    }
    
public:
    bool userLocalIdf() {
        return useLocalIdf_;
    }
    
    size_t uselessTelThreshold() {
        return simConfig_.m_useless_tel_threshold;
    }
    double getOutputThreshold( ) {
        return simConfig_.m_poisim_all_output_th;
    }
    bool findInSelf() {
        return simConfig_.m_find_in_self;
    }		
    
    Parser* parser() {
        return parser_;
    }
    
    void dump();
    
private:
    bool initSim();
    bool initRules();
    
    bool initRule( std::vector<BaseRule*>& rules, Json::Value& conf );
    
private:
    
    PoiSimCfg simConfig_;
    std::vector<BaseRule*> processRules_;
    std::vector<BaseRule*> initFirstRules_;
    std::vector<BaseRule*> initSecondRules_;
    
    Json::Value configs_;
    OnlineProcess* process_;
    bool useLocalIdf_; 
    Parser* parser_;
};


#endif