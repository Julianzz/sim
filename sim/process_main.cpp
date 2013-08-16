#include "process_main.h"
#include "process_engine.h"

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
    ThreadBatchLoad loader(DEFAULT_WORKERS,&batchWorker );
    ret = loader.start();
    if( !ret ) {
        std::cout<<"compute sim error " <<std::endl;
        return false;
    }
    return true;
}


BatchProcess::BatchProcess(ProcessStrategy& s,size_t batchNum ):
    baseRecords_(NULL),
    incRecords_(NULL),
    strategy_(s),
    batchNum_(batchNum) { 
    output_ =  new MutexStdOutput();
    incEngine_  = new AllMatchBatchEngine(*this);
    baseEngine_ = new MaxMatchBatchEngine(*this);
}

BatchProcess::~BatchProcess() {
    if ( output_ ) {
        delete output_;
    }
    if ( baseEngine_ ) {
        delete baseEngine_;
    }
    if ( incEngine_ ) {
        delete incEngine_;
    }
    //std::cout<<"destroy batch process" <<std::endl;
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
    
    std::cout <<"-----output " <<std::endl;
	std::cout << "max idf:" << resource_.max_idf << "\t" << resource_.min_idf << std::endl;
	std::cout << "idf init:" << resource_.idf.size() << std::endl;
    std::cout << "idf size:" <<baseRecords_->size() <<"," <<incRecords_->size()<<std::endl;
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
    baseRecords_->info();
    
    ThreadBatchLoad incLoader(batchNum_, incRecords_);
    incLoader.start();
    incRecords_->info();
    loadFinish();
    
    return true;
}

bool BatchProcess::process() {

    std::cout<<"beigin to process" <<std::endl;
    while( true ) {
        PreProcData* base = incRecords_->next();
        if( base == NULL ) {
            return true;
        }
        process( *base, *baseRecords_ ,true);
        if( strategy_.findInSelf()) {
            process(*base, *incRecords_,false );
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
    if ( isBase ) { 
        return baseEngine_->processSim(base, records, sameIds,isBase );
    } else {
        return incEngine_->processSim(base, records, sameIds, isBase );
    }
    return true;
}
//
bool BatchProcess::processOutput(StrategyResult& result,bool isBase) {
    SimTempCache& simTmpCache = result.simCache; 
    //if (simTmpCache.m_score > strategy_.getOutputThreshold() ) {
        output_->output(result,isBase);
        //}
    return true;
}
