#include "httpserv.h"
#include "StringUtility.h"
#include "services.h"


void ServiceWorker::route( const string& path, RichService* service ) {
    map<const string, RichService* >::iterator iter = routes.find(path);
    if (  iter != routes.end() ) {
        RichService* tmp = iter->second;
        routes.erase(iter);
        delete tmp;
    }
    routes[path] = service;
};

int32_t ServiceWorker::process(HTTPServer* serv, HTTPServer::Session* sess,
                const string& path, HTTPClient::Method method,
                const map<string, string>& reqheads,
                const string& reqbody,
                map<string, string>& resheads,
                string& resbody,
                const map<string, string>& misc) {

    vector<string> querys;
    string query;
    map<string, string>::const_iterator findValue = misc.find("query");
    if (findValue != misc.end()){
        query = findValue->second;
    }

    /*
    * parse querys
    */
    map<string,string> allOptions;
    vector<std::string> options;
    string quyerSpliter("&");
    StringUtility::Split(query,quyerSpliter,options);
    for(size_t i=0;i<options.size();i++){
        vector<string> querys;
        string spliter("=");
        StringUtility::Split(options[i],spliter,querys);
        if(querys.size()>=2){
            allOptions[ querys[0] ] =querys[1];
        }else if (querys.size()>0) {  
             allOptions[ querys[0] ] = "";
        }
    }
    
    if( routes.find(path) == routes.end() ) {
        std::cerr<<"error in routes :"<<path<<std::endl;
        return 404;
    }
    std::cerr<<"begin to process:" << path <<std::endl;
    RichService* service = routes[path];
    
    switch(method){
        case HTTPClient::MGET:
          return service->get(allOptions,reqheads,reqbody,resheads,resbody);
        case HTTPClient::MPUT:
          return service->put(allOptions,reqheads,reqbody,resheads,resbody);
        case HTTPClient::MPOST:
          return service->post(allOptions,reqheads,reqbody,resheads,resbody);
        case HTTPClient::MDELETE:
          return service->post(allOptions,reqheads,reqbody,resheads,resbody);
        default:
          return 404;
    }
    return 404;
};

