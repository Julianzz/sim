#include "sim_service.h"
#include "timeprofile.h"


int SimService::fetchPoi(const string& id, vector<string>& poi1, 
        vector<string>& poi2, 
        int& errCode, Json::Value& reason  ) {
    
    string spliter(",");
    vector<string> ids;
    StringUtility::Split(id,spliter,ids); 
    if( ids.size()< 2) {
        reason = Json::Value("not enough id to compare,or delimeter is wrong ,please use(,) ");
        errCode = 404;
        return -1;
    } 
    int ret1 = FetchService::fetchPP(ids[0],poi1);
    int ret2 = FetchService::fetchPP(ids[1],poi2);

    if( ret1 !=0  || ret2 != 0) {
        reason = Json::Value("error in fetch poi, please make sure that the givm poiid exists in pp");
        errCode = 404;
        return -1;
    } 
    return 0;
}

int SimService::parseParams( const map<string,string>& querys, string& id ,
            int& errCode, Json::Value& reason ) {
                
    std::string field = "ids";
    map<string,string>::const_iterator iter = querys.find(field);

    if( iter == querys.end()) {
        reason = Json::Value("ids is not in query params");
        errCode = 404;
        return -1;
    };
    id = iter->second;
    return 0;
}


int32_t SimService::get( const map<string,string>& querys,
        const map<string, string>& reqheads,
        const string& reqbody,
        map<string, string>& resheads,
        string& resbody ) {
    
    int errCode = 200;
    int ret = 0;
    Json::Value reason("");
    Json::Value result;
    
    ServiceResult retResult;
    
    TimeProfile profile;
    profile.start();
    
    do { 
        string id;
        vector<string> poi1,poi2;
        
        ret = parseParams(querys,id, errCode,reason);
        if( ret != 0 ){
            break;
        }
        ret = fetchPoi( id,poi1,poi2,errCode,reason );
        if( ret != 0 ){
            break;
        }
        
        OnlineSim* sim_ = OnlineSim::instance() ;
        ret = sim_->run(poi1,poi2,result );
        if( ret !=0  || result.isNull() ) {
            errCode = 404;
            reason = Json::Value("compare error,reason? ");
            break;
        }
    } while (0);
    
    retResult.setError( errCode, reason );
    retResult.setResult( result );
    resbody = retResult.dump();

    return retResult.getErrorCode();
}
    
int32_t SimService::post(const map<string,string>& querys,
        const map<string, string>& reqheads,
        const string& reqbody,
        map<string, string>& resheads,
        string& resbody ) {

    Json::Value value;
    Json::Reader reader;
    int errCode = 200;
    int ret = 0;
    Json::Value reason("");
    Json::Value result;
    
    ServiceResult retResult;
    
    do { 
    
        if( !reader.parse(reqbody, value )) {
            errCode = 404;
            reason = Json::Value("can not parse post body,please check json format");
            break;
        }
        std::string type = "json";
        if( !value.isMember("first") ||  ! value.isMember("second")) {
            errCode = 404;
            reason = Json::Value("first or second field is not in json ");
            break;
        }
        Json::Value first = value["first"];
        Json::Value second = value["second"];
        Json::Value result;

        OnlineSim* sim_ = OnlineSim::instance() ;

        if( first.isString() && second.isString() ) {
            ret = sim_->run( first.asString(), second.asString(),result);
        }else if( first.isObject() && second.isObject() ) {
            ret = sim_->run( first,second, result);
        }
        if( ret !=0  || result.isNull() ) {
            errCode = 404;
            reason = Json::Value("compare error,reason? ");
            break;
        }
        
    }while( 0 );
    
    retResult.setError( errCode, reason );
    retResult.setResult( result );
    resbody = retResult.dump();

    return retResult.getErrorCode();

        
}    
