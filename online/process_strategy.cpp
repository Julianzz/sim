#include "process_strategy.h"
#include "process_main.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

bool ProcessStrategy::initRecord(const Record& record, PreProcData *baseData,bool isBase ) {
    if( isBase ) {
        return initRecord(initFirstRules_, record, baseData );
    } else {
        return initRecord(initSecondRules_, record, baseData );
    }
}

bool ProcessStrategy::initRecord(std::vector<BaseRule*>& rules,const Record& record, 
            PreProcData *baseData ) {
    
    std::vector<BaseRule*>::iterator iter  = rules.begin();
    for( ; iter != rules.end(); iter++ ) {
        BaseRule* rule = *iter;
        bool conn = rule->processRecord(baseData,&record) ;
        //TODO
        //std::cout<<"init:"<<rule->getName()<<","<<conn<<std::endl;
        if (!conn) {
            return true;
        }
    }
    return true;
}

//
void ProcessStrategy::sim( const Record& base, const Record& inc, 
        StrategyResult *results ) {
            
    PreProcData basePre;
    PreProcData incPre;
    
    initRecord( initFirstRules_, base, &basePre );
    initRecord( initSecondRules_, inc, &incPre );
    results->init(&basePre, &incPre,true );
    sim( &basePre, &incPre, results );
    
}

//main compare process function,invoke fule to process
//TODO should seperate process to unit,deal,score,etc.
//     maybe should add some different process
void ProcessStrategy::sim(const PreProcData *baseData, const PreProcData *incData, 
        StrategyResult *results ) {
             
    SimTempCache *tempCache = &(results->simCache);
	std::vector<BaseRule*>::iterator iter = processRules_.begin();
	for (; iter != processRules_.end(); ++iter) {
        bool ret = (*iter)->process( baseData,incData,tempCache);
        //std::cout<<"process:"<<(*iter)->getName()<<","<<ret<<std::endl;
        if( ret ) {
            return ;
        }
	}
}

bool ProcessStrategy::init( const string& fileName,OnlineProcess& process ) {
    
    process_ = &process;
    bool ret = parseJson(fileName, configs_);
    if( !ret ) {
        std::cout<< "error in parse json" <<std::endl;
        return false;
    }

    ret = initSim();
    if( !ret ) {
        std::cout<< "init sim error" <<std::endl;
        return false;
    }
    
    ret = initRules();
    if( !ret ) {
        std::cout<< "init rules error" <<std::endl;
        return false;
    }  
    
    parser_ = new IndexParser();
    parser_->init();
    
    return true; 

}

static const char* params[] =  { 
    "debug",
    "find_in_self",
    "del_same_id",
    "out_base_sgl",
    "out_inc_sgl",
    "name_same_weight",
    "name_sub_same_weight",
    "name_diff_weight",
    "name_sub_diff_weight",
    "poisim_all_output_th",
    "useless_tel_threshold"
};

bool ProcessStrategy::initSim() {
    
    std::cout<<"======= begin to init sim ============ " <<std::endl;
    
    Json::Value& simConfig = configs_;
    
    for( size_t i =0 ; i< sizeof(params)/sizeof(params[0]); i++ ) {
        if ( simConfig[params[i]].isNull() ) {
            std::cout<< "param should provide: "<< params[i] <<std::endl;
            return false;
         }
    }
    
	simConfig_.m_debug = simConfig["debug"].asBool();
	simConfig_.m_find_in_self = simConfig["find_in_self"].asBool();
	simConfig_.m_del_same_id = simConfig["del_same_id"].asBool();

	simConfig_.m_out_base_sgl = simConfig["out_base_sgl"].asBool();
	simConfig_.m_out_inc_sgl = simConfig["out_inc_sgl"].asBool();

	simConfig_.m_name_same_weight = simConfig["name_same_weight"].asDouble();
	simConfig_.m_name_sub_same_weight = simConfig["sub_same_weight"].asDouble();
	simConfig_.m_name_diff_weight = simConfig["name_diff_weight"].asDouble();
	simConfig_.m_name_sub_diff_weight = simConfig["name_sub_diff_weight"].asDouble();
	simConfig_.m_poisim_all_output_th = simConfig["poisim_all_output_th"].asDouble();
	simConfig_.m_useless_tel_threshold = simConfig["useless_tel_threshold"].asDouble();    
    
    std::cout<<simConfig_.m_thread_num<<std::endl;
    std::cout<<simConfig_.m_use_local_idf<<std::endl;
	//simConfig_.m_base_inc_sim_func = poiSimMax;
	//simConfig_.m_inc_inc_sim_func = poiSimAll;
    return true;

}

bool ProcessStrategy::initRule( std::vector<BaseRule*>& rules, Json::Value& conf ) {
    

    for(size_t i = 0 ; i < conf.size(); i++ ) {
        
        Json::Value& ruleConfig = conf[i];
        Json::Value method = ruleConfig["method"] ;
        Json::Value params = ruleConfig["params"] ; 
        
        //std::cout<<" begin to init "<< method.asString() <<std::endl;
        if( method.isNull() ) {
            std::cout<<" config is wrong, method params is not provided " <<std::endl;
            return false;
        }
        
        BaseRule* base = RuleRegister::getRule( method.asString() );
        if ( base == NULL ) {
            std::cout<<"rule:"<< method <<" can not find"<<std::endl;
            return false;
        }
        
        BaseRule* rule = base->clone();
        if ( rule == NULL ) {
            std::cout<<"rule:"<<method.asString()<<" can not be clone "<<std::endl;
            return false;
        }
        bool ret = rule->config(params, &simConfig_); 
        if( !ret ){
            std::cout<<"config rule:"<< method.asString() << " error" <<std::endl;
            return false;
        }
        
        rules.push_back( rule );
    }
    return true;
};

bool ProcessStrategy::initRules() {
    
    Json::Value& processConf = configs_["sim_rules"];
    Json::Value& initBaseConf = configs_["base_rules"];
    Json::Value& initIncConf = configs_["inc_rules"];
    
    if ( processConf.isNull() || initBaseConf.isNull() || initIncConf.isNull() ) {
    //if ( processConf.isNull() ) {
        std::cout<< "please provide sim_rules or base_rules or inc_rules " <<std::endl;
        return false;
    }
    
    bool ret = initRule(processRules_, processConf ); 
    if( !ret ) {
        std::cout<<"init rules failed "<<std::endl;
        return false;
    }
    std::cout<< "success in process rule " <<std::endl;
    
    ret = initRule(initFirstRules_, initBaseConf ); 
    if( !ret ) {
        std::cout<<"init rules failed "<<std::endl;
        return false;
    }
    std::cout<< "success init first rule "<<std::endl;
    
    ret = initRule(initSecondRules_, initIncConf ); 
    if( !ret ) {
        std::cout<<"init rules failed "<<std::endl;
        return false;
    }
    
    std::cout<<"success second rules"<<std::endl;
    return true;
}
