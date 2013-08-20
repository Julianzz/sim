#include "process_engine.h"
#include "process_main.h"


bool MaxMatchBatchEngine::compareSimScore(StrategyResult& cur, StrategyResult& temp) {
	
    if (cur.simCache.m_is_null) {
        return true;
	}
	if ( cmpRecord(cur.simCache, temp.simCache) ) {
        return true;
	}
    
    return false;
}

bool MaxMatchBatchEngine::processSim(const PreProcData& record,BatchRecords& records, 
    geo::GridResult& sameIds, bool isBase ) {
    
    ProcessStrategy& strategy = process_.getStrategy();
    StrategyResult maxResult ;
    
    geo::GridResult::iterator iter =  sameIds.begin();
    for( ; iter != sameIds.end(); iter++ ) {
        PreProcData* inc = records.fetch(*iter) ;
        if ( inc == NULL ) {
            continue ;
        }
        StrategyResult result;
        result.init( inc, &record , isBase );
        strategy.sim( inc,&record, &result );
        
        if ( compareSimScore( maxResult ,result ) ) {
            maxResult = result;
            maxResult.setNotNull();
        }
    }
    if( !maxResult.isNull() ) { 
        process_.processOutput(maxResult, isBase);
    }
    return true;
}

bool FirstMatchBatchEngine::processSim(const PreProcData& record, BatchRecords& records, 
    geo::GridResult& sameIds, bool isBase ) {
        
    return true;
    
}
bool AllMatchBatchEngine::processSim(const PreProcData& record,BatchRecords& records, 
        geo::GridResult& sameIds, bool isBase ) {
            
    StrategyResult result;
    ProcessStrategy& strategy = process_.getStrategy();
    geo::GridResult::iterator iter =  sameIds.begin(); 
    for( ;iter != sameIds.end(); iter++ ) {
        PreProcData* inc = records.fetch(*iter) ;
        if ( inc == NULL ) {
            continue ;
        }
        result.init( inc, &record , isBase );
        strategy.sim( inc,&record, &result );
        process_.processOutput(result,isBase );
    }
    
    return true;
} 