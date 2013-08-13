/*
 * services.h
 *
 *  Created on: 2012-8-17
 *      Author: lzz
 */

#ifndef SERVICES_H_
#define SERVICES_H_

#include <string>
#include <map>  

using namespace std;
 
class RichService {

public:
    RichService()  {} ;
    virtual ~RichService() {
        this->release();
    };
    
    virtual int init() {
        return 0;
    } ;
    virtual void release() {};
    
public:
    
    virtual int32_t get( const map<string,string>& querys,
            const map<string, string>& reqheads,
            const string& reqbody,
            map<string, string>& resheads,
            string& resbody ) {
                return 404;
            }
            
    virtual int32_t put( const map<string,string>& querys,
            const map<string, string>& reqheads,
            const string& reqbody,
            map<string, string>& resheads,
            string& resbody ) {
                return 404;
            }    
    
    virtual int32_t post(const map<string,string>& querys,
            const map<string, string>& reqheads,
            const string& reqbody,
            map<string, string>& resheads,
            string& resbody ) {
                return 404 ;
            }
            

    virtual int32_t del(const map<string,string>& querys,
            const map<string, string>& reqheads,
            const string& reqbody,
            map<string, string>& resheads,
            string& resbody ){
                return 404;
            }         

};

#endif /* SERVICES_H_ */
