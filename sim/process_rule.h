#ifndef __SIM_RULE_H__
#define __SIM_RULE_H__

#include <map>
#include <string>
#include <assert.h>
#include "json/json.h"
#include "process_resource.h"
#include "process_result.h"

class StrategyResult;
class PreProcData;
class SimTempCache;
class Resource;
class PoiSimCfg;
class Record;

class BaseRule {
    
private:
    std::string name_;
public:
    void setName(const std::string& name ) {
        name_ = name;
    }
    
    const std::string& getName() const {
        return name_;
    }
    
    virtual const string desc() const {
        return "ohh,no info,please guess ";
    }
    
public:
    virtual bool config(Json::Value& items,  PoiSimCfg* simConfig) = 0;
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) = 0;

    virtual bool process( const PreProcData *base_data, 
        const PreProcData *inc_data,
        StrategyResult *results ) = 0;
                  
    virtual bool processRecord(PreProcData *baseData,
            const Record *record ) = 0;
    
    virtual bool initRecord(const std::string& data, Record *record ) = 0;
    
    virtual ~BaseRule() {};
    virtual BaseRule *clone() const = 0;
};


template <typename Derived>
class ResourceRule:public BaseRule {
    
public:
    const Resource* resource_;
    const PoiSimCfg* simConfig_;
    const Tools* tools_;
    
public:
    virtual bool config(Json::Value& items,
                PoiSimCfg* simConfig ) {
        tools_  = &( ProcessResource::instance().tools());
        resource_ = &( ProcessResource::instance().resource()); ;
        simConfig_ = simConfig;
        return this->config(items);
    }
    
    virtual bool config(Json::Value& items) = 0;
    
    virtual bool process(const PreProcData *base_data, 
            const PreProcData *inc_data,
    		SimTempCache *sim_temp_cache) {
                assert( false );
                return false;
            }
            
    virtual bool process( const PreProcData *base_data, 
        const PreProcData *inc_data,
        StrategyResult *results ){
        SimTempCache *tempCache = &(results->simCache);
        return this->process(base_data, inc_data, tempCache );
    }
    
    virtual bool processRecord(PreProcData *baseData,
            const Record *record ) {
        assert( false );
        return false;
    };
    
    virtual bool initRecord(const std::string& data, Record *record ) {
        assert( false );
        return false;
    }
    
public:
    virtual BaseRule *clone() const {
          return new Derived(static_cast<Derived const&>(*this));
    }
};

class RuleRegister {
public:
    static  std::map<std::string,BaseRule*> rules_;
    static  void addRule(const std::string& name, BaseRule* rule );
    static  BaseRule* getRule(const std::string& name );
    
public:
    RuleRegister(const std::string& name,BaseRule* rule ) {
        addRule(name,rule);
    }
};

#define REGINTER_RULE(name)  \
    static name##Rule  name##_rule_; \
    static RuleRegister name##_rule_register_(std::string(#name),(BaseRule*)&name##_rule_ );
     

#endif