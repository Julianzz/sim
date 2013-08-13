#ifndef __SIM_SERVICE_H__
#define __SIM_SERVICE_H__

#include <string>
#include <map>

#include "services.h"
#include "online_sim.h"
#include "json/json.h"
#include "fetch_data.h"
#include "StringUtility.h"

using namespace std;

class ServiceResult {
public:
    ServiceResult():errCode_(200),reason_(""),result_("") { };
    
    void setError(int errCode, Json::Value& reason ) {
        errCode_ = errCode;
        reason_ = reason;
    }
    int getErrorCode() {
        return errCode_;
    }
    
    void setResult( Json::Value& result ) {
        result_ = result;
    }
    
    
    string dump(){
        
        Json::Value ret;
        ret["error"] = reason_;
        ret["sim"] = result_;
        ret["status"] = Json::Value(errCode_);
    
        Json::StyledWriter writer;
        string resp  = writer.write(ret);
        return resp;
    }
    
private:
    int errCode_;
    Json::Value reason_;
    Json::Value result_;

};

class SimService: public RichService {
    
public:
    
    virtual int32_t get( const map<string,string>& querys,
            const map<string, string>& reqheads,
            const string& reqbody,
            map<string, string>& resheads,
            string& resbody ) ;
            
    virtual int32_t post(const map<string,string>& querys,
            const map<string, string>& reqheads,
            const string& reqbody,
            map<string, string>& resheads,
            string& resbody ) ;
            
public:
    int fetchPoi(const string& ids, vector<string>& poi1, vector<string>& poi2, int& errCode, Json::Value& reason );
    int parseParams( const map<string,string>& querys, string& id, int& errCode, Json::Value& reason  );
    
};

#endif
