/*
 * httpworke.h
 *
 *  Created on: 2012-8-19
 *      Author: lzz
 */

#ifndef HTTPWORKE_H_
#define HTTPWORKE_H_
     
#include "kthttp.h"

using namespace std;
using namespace kyototycoon;

class RichService;
// prepare the worker
class ServiceWorker : public HTTPServer::Worker {
    
private:
    map<const string, RichService* > routes;
    
public:
    void route( const string& path, RichService* service ) ;
    int32_t process(HTTPServer* serv, HTTPServer::Session* sess,
                    const string& path, HTTPClient::Method method,
                    const map<string, string>& reqheads,
                    const string& reqbody,
                    map<string, string>& resheads,
                    string& resbody,
                    const map<string, string>& misc) ;
};

#endif /* HTTPWORKE_H_ */
