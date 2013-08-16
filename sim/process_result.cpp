#include "process_result.h"

void StrategyResult::init( const PreProcData *base_data, const PreProcData *inc_data ,bool isBase) {
	simCache.clear();
    results.clear();
    simCache.m_base_id = base_data->m_id;
	simCache.m_inc_id = inc_data->m_id;
	simCache.m_base_name = base_data->m_name;
	simCache.m_inc_name = inc_data->m_name;
	simCache.m_status = base_data->m_status;
	simCache.m_is_base = isBase;
}

void StrategyResult::dump() {
    std::cout<<" ++++++++ output info is ++++++ " <<std::endl;
    print(&simCache);
}

void StrategyResult::setRuleInfo(const std::string& name, bool value ) {
    results[ name ] = value;
    //results["rules"][name] = value;
    //results["names"][name] = simCache.m_score;
}

std::string StrategyResult::outputRule(bool isFull ) const {
    std::string ret;
    std::string tmp;
    if(results.size() <= 0 ) {
        return ret;
    }
    
    std::map<std::string,bool>::const_iterator iter = results.begin();
    for ( ; iter != results.end(); iter++ ) {
        if ( iter->second ) {
            ret += iter->first;
        }
    }
    /*
    Json::Value::Members member =results["rules"].getMemberNames();
    Json::Value::Members::iterator iter = member.begin();
    
    for ( ;iter != member.end(); iter++ ) {
        tmp += *iter+":";
        const Json::Value& value = results["rules"][*iter];
        bool v = value.asBool();
        if( v ) {
            ret += *iter;
        }
    }*/
    return ret ;
}