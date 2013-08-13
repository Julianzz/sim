#ifndef __POOL_H__
#define __POOL_H__

#include <string>
#include <iostream>
#include "cpper/common.h"

using namespace std;
using namespace cpper::core;
using namespace cpper::collection;

template < class Factory,class BasicIO >
class IOPool {
    
private:
    CBlockingList<BasicIO*> pools_;
    Factory& factory_;
    
public:
    
    class Connection {
    private:
        IOPool* pool_;
        BasicIO* io_;
        
    public:
        Connection(IOPool* pool,BasicIO*io):pool_(pool),io_(io) { };
        ~Connection() {
        };
        
        void release() {
            if( io_ != NULL ) {
                pool_->release(io_);
                io_ = NULL;
            };
        };

        BasicIO* operator ->() {
            return io_;
        }
    };
    
    
public:
    
    IOPool(Factory& f ):factory_(f){ };
    
    ~IOPool() {
        BasicIO* basicIO = pools_.tryPoll();
        while(basicIO != NULL){
            factory_.release(basicIO);
            basicIO = pools_.tryPoll();
        }    
    }
    
    int init(const int poolSize = 10 ) {
        int i =0 ;
        for( i=0; i< poolSize; i++ ) {
            BasicIO* io = factory_.build();
            if (io == NULL ) {
                return -1;
            }
            pools_.addLast(io);
        }
        return 0;
    }
    
    Connection get() {
        BasicIO* io_ = pools_.poll();
        return Connection(this,io_);
    }
    
    void release( BasicIO* io_) {
        if( io_ != NULL ) {
            pools_.addLast(io_);
        }
    }
    
    std::string dump() {
        std::cout<<pools_.size()<<std::endl;
        return std::string("test");
    }
    
};


#endif