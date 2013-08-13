/*
 * test_main.cpp
 *
 *  Created on: 2012-10-22
 *      Author: stefanyang
 */

#include <unistd.h>
#include <getopt.h>
#include <fstream> 
     
#include "settings.h"
#include "json/reader.h"
#include "json/reader.h"
#include "json/writer.h" 
     
#include "online_sim.h"    
#include "sim_service.h"
     
#include "kthttp.h"
#include "services.h"
#include "StringUtility.h"
#include "httpserv.h"
#include "configparser.h"
#include "logger.h"
     
#include "pool.h"
#include "basic_io.h"
#include "level_io.h"
#include "fetch_data.h"

using namespace std;
using namespace kyototycoon;

HTTPServer* g_serv = NULL;

static void stopserver(int signum) {
if (g_serv){
   g_serv->stop();
}
g_serv = NULL;
}


void usage(){
	cout << "<=== usage ===>" << endl;
}

template <class Iter>
void output(const Iter& begin,const Iter& end) {
    Iter iter = begin;
    for( ;iter != end; iter++ ) {
        std::cout<<*iter<<std::endl;
    }
}

int main(int argc, char** argv) {
    
    std::string levelConfig("config/store.conf");

    std::string httpConfig("config/http.conf");
    std::string sourceDir("../source/");
    std::string mode("online");
    
    FetchService::init(levelConfig); 
    
   // prepare the server
    string server_hosts;
    double timeout = -1;
    int workers=10;
    string redishost;
    string slavehost;
    string logfile;
    int loglevel;
    
    int ret = 0 ;
    int opt;
    
    struct option long_options[] = {
       { "sources", required_argument, NULL,'s'},
       { "config", required_argument, NULL,'c'},
       { NULL,0,NULL,0 }
    };

    while((opt = getopt_long(argc, argv, "s", long_options, NULL)) != -1) {
        switch (opt) {
        case 's':
            sourceDir = optarg;
            break;
        case 'c':
            httpConfig = optarg;
        default:
            usage();
            exit(1);          
        }
    }
    
    // set the signal handler to stop the server
    setkillsignalhandler(stopserver);
    
	if(!endswith(sourceDir, "/")){
		sourceDir += "/";
	}
    
    CConfigParser config;
    if(!parse_config(httpConfig,config)){
        usage();
        return -1;
    }
    
    parse_server_config(config,server_hosts,timeout,workers,redishost,slavehost,logfile,loglevel);

    //log module init
    if(logger_init(logfile,loglevel)!=0){
        printf("error in init log services\n");
        exit(-1);
    }
    OnlineSim* sim_ = OnlineSim::instance() ;
    ret = sim_->init(sourceDir,mode);
    if( ret != 0 ){
        std::cout<<" error in init sim " <<std::endl;
        exit(-1);
    }
      
    HTTPServer serv;
    ServiceWorker worker;
    SimService* sim = new SimService();
    
    worker.route("/poisim", sim );
    serv.set_network(server_hosts, timeout);
    serv.set_worker(&worker, workers);
    //serv.set_logger((HTTPServer::Logger*)logger_get(1),logger_level());
    g_serv = &serv;
    
    std::cout<<"hosts:"<<server_hosts<<",workders:"<< workers<<std::endl;
    // start the server and block until its stop
    serv.start();

    // clean up connections and other resources
    serv.finish();
  
    logger_release();
    
	return 0;
}

