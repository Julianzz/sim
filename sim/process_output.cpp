#include "process_output.h"
#include <sstream>

void MutexStdOutput::output(const StrategyResult& result,bool isBase ) {

    const SimTempCache& simTmpCache = result.simCache; 
    std::string strategy = result.outputRule();
    std::stringstream ss;
    
    std::string value = "base";
    if (!isBase ) {
        value = "increase";
    }
    
	ss << "[rel]:" <<simTmpCache.m_base_id << "\t"
		<<simTmpCache.m_inc_id << "\t" <<simTmpCache.m_score
		<< "\t" <<simTmpCache.m_len_diff
        << "\t" <<simTmpCache.m_addr_sim 
        << "\t" <<simTmpCache.m_distance 
        << "\t" <<simTmpCache.m_same_tel_set.size() 
        << "\t" << value
		<< "\t" <<simTmpCache.m_status 
        << "\t" << "["<<strategy<<"]"
        << "\t" <<simTmpCache.m_name_score
        << "\n";

    //TODO,need verify 
    ScopeLocker locker(locker_) ;
    std::cout<< ss.str();

};