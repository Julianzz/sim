#include "process_main.h"

#include "tools.h"
#include "prepare.h"
#include "rules.h"
#include "init.h"
#include "sim.h"
#include "common.h"

OnlineProcess::OnlineProcess(const std::string& name ):fileName_(name) {
}

OnlineProcess::~OnlineProcess() {
}

bool OnlineProcess::init() {
    
    bool ret = parseJson(fileName_, configs_);
    if( !ret ) {
        std::cout<< "error in parse json" <<std::endl;
        return false;
    }
    sourceDir_ = "../source";
    if ( configs_["source_dir"].isString() ) {  
        sourceDir_ = configs_["source_dir"].asString();
    }
    
    ret = initResource();
    if( !ret ) {
        std::cout<<"init resource error " <<std::endl;
        return false;
    }
    //useLocalIdf_ = configs_["use_local_idf"].asBool();
    
	wordSplitDicVec_.clear();
    size_t size = configs_["wordsplit_dic"].size();
    for(size_t i=0 ; i<size ; i++ ){
        std::string name = sourceDir_ +"/" + configs_["wordsplit_dic"][i].asString();
        wordSplitDicVec_.push_back(name);
    }
    
	Tools& tools_  = ProcessResource::instance().tools() ;
	int result = tools_.m_wordsplit->load(wordSplitDicVec_);
    if ( result != 0 ) {
        std::cout<<"init wordsplit error" <<std::endl;
        return false;
    }
    
    Json::Value& strategy = configs_["strategy"];
    Json::Value::Members members = strategy.getMemberNames();
    Json::Value::Members::iterator iter  = members.begin();
    for( ;iter != members.end(); iter++ ) {
        std::string name  = *iter; 
        std::string ruleFile = strategy[name].asString();
        ProcessStrategy& ps = strategies_[name];
        ret = ps.init(ruleFile, *this);
        if( !ret ) {
            std::cout<<" init failed: "<< name <<std::endl;
            return false;
        }
    }
    return true;
}

void OnlineProcess::dump() {
    std::cout<<"-------dump info--------" <<std::endl;
    std::cout<<"----:"<<sourceDir_<<std::endl;
    std::vector<std::string>::iterator iter = wordSplitDicVec_.begin();
    for( ;iter != wordSplitDicVec_.end(); iter++ ) {
        std::cout<<*iter<<std::endl;
    }
}

bool OnlineProcess::compute( const std::string& strategy,const Record& first, 
                const Record& second, ResultItem*  ) {
                
    std::map<std::string,ProcessStrategy>::iterator iter = strategies_.find( strategy ); 
    if (iter != strategies_.end() ) {
        ProcessStrategy& strategy = iter->second;
        StrategyResult result;
        strategy.sim(first,second,&result);
        std::cout<<"finish compare" << std::endl;
        return true;
    }
    return false;
}

bool OnlineProcess::batchProcess( const std::string& strategy,
         const std::string& baseName, 
         const std::string& incName ) {
    
    std::map<std::string,ProcessStrategy>::iterator iter = strategies_.find( strategy ); 
    if (iter == strategies_.end() ) {
        std::cout<<"strategy:"<<strategy<<" can not find "<<std::endl;
        return false;
    }
    
    BatchProcess batchWorker(iter->second);
    bool ret = batchWorker.init(baseName,incName);
    if ( !ret ) {
        std::cout<<"init batch woker error" <<std::endl;;
        return false;
    }
    ThreadBatchLoad loader(2,&batchWorker );
    ret = loader.start();
    if( !ret ) {
        std::cout<<"compute sim error " <<std::endl;
        return false;
    }
    return true;
}


bool BatchProcess::loadFinish() {
    Resource& resource_ = ProcessResource::instance().resource() ;
    
	if ( !strategy_.userLocalIdf() ) {
		makeIdf(baseRecords_->size() + incRecords_->size(),
                 resource_.idf,
				 resource_.max_idf, 
                 resource_.min_idf );
	}
	makeUselessTel(resource_.uselessTelMap, strategy_.uselessTelThreshold() );
    return true;
}

bool BatchProcess::init(const std::string& base,const std::string inc ) {
    
    BasicInput baseInput;
    BasicInput incInput;
    
    Parser* parser = strategy_.parser();
    
    int ret = baseInput.config(base, parser);
    if( !ret ) {
        std::cout<< "process error " <<std::endl;
        return false;
    }
    ret = incInput.config(inc,parser);
    if( !ret ) {
        std::cout<< "process error " <<std::endl;
        return false;
    }        
    
    baseRecords_ = new BatchRecords( &strategy_,&baseInput,true );
    incRecords_ = new BatchRecords ( &strategy_, &incInput, false );
    
    ThreadBatchLoad baseLoader(batchNum_, baseRecords_);
    baseLoader.start();
    
    ThreadBatchLoad incLoader(batchNum_, incRecords_);
    incLoader.start();
    
    loadFinish();
    
    return true;
}

bool BatchProcess::process() {

    std::cout<<"beigin to process" <<std::endl;
    while( true ) {
        PreProcData* base = baseRecords_->next();
        if( base == NULL ) {
            return true;
        }
        process( *base, *incRecords_,true);
        if( strategy_.findInSelf()) {
            process(*base, *baseRecords_,false );
        }
    }
    return true;
}


bool BatchProcess::process(const PreProcData& base, BatchRecords& records, bool isBase) {
    geo::GridResult sameIds;
    records.search(base,&sameIds);
	if (!isBase) {
		sameIds.erase(base.m_id);
	}
    
    return processSim(base, records, sameIds,isBase );
    
}
bool BatchProcess::processSim(const PreProcData& base, BatchRecords& records,
        geo::GridResult& sameIds, bool isBase) {
            
    geo::GridResult::iterator iter =  sameIds.begin(); 
    for( ;iter != sameIds.end(); iter++ ) {
        PreProcData* inc = records.fetch(*iter) ;
        if ( inc == NULL ) {
            continue ;
        }
        
        StrategyResult result;
        strategy_.sim( &base,inc, &result );
        
        processOutput(result,isBase );
    }
    return true;
}

void BatchProcess::output(StrategyResult& result, bool isBase ) {
    SimTempCache& simTmpCache = result.simCache; 
    
	if (isBase) {
		cout << "[rel]:" <<simTmpCache.m_base_id << "\t"
			<<simTmpCache.m_inc_id << "\t" <<simTmpCache.m_score
			<< "\t" <<simTmpCache.m_len_diff << "\t"
			<<simTmpCache.m_addr_sim << "\t"
			<<simTmpCache.m_distance << "\t"
			<<simTmpCache.m_same_tel_set.size() << "\t" << "base"
			<< "\t" <<simTmpCache.m_status << "\n";
	} else {
		cout << "[rel]:" <<simTmpCache.m_base_id << "\t"
			<<simTmpCache.m_inc_id << "\t" <<simTmpCache.m_score
			<< "\t" <<simTmpCache.m_len_diff << "\t"
			<<simTmpCache.m_addr_sim << "\t"
			<<simTmpCache.m_distance << "\t"
			<<simTmpCache.m_same_tel_set.size() << "\t" << "increase"
			<< "\t" <<simTmpCache.m_status << "\n";
	}
}

//
bool BatchProcess::processOutput(StrategyResult& result,bool isBase) {
    ScopeLocker locker(writeLocker_) ;
    SimTempCache& simTmpCache = result.simCache; 
    if (simTmpCache.m_score > strategy_.getOutputThreshold() ) {
        output(result,isBase);
    }
    return true;
}


//will find the max matches
bool MaxMatchBatchProcess::processSim(const PreProcData& base,BatchRecords& records,
         geo::GridResult& sameIds,bool isBase ) {
             
    StrategyResult maxResult ;
    geo::GridResult::iterator iter =  sameIds.begin();
    for( ;iter != sameIds.end(); iter++ ) {
        PreProcData* inc = records.fetch(*iter) ;
        if ( inc == NULL ) {
            continue ;
        }
        
        StrategyResult result;
        strategy_.sim( &base,inc, &result );
        compareSimScore(&(result.simCache), &(maxResult.simCache));
    }   
    processOutput(maxResult, isBase);
    
    return true;
}
