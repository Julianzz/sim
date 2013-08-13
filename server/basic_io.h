#ifndef __BASIC_IO_H__
#define __BASIC_IO_H__

#include <iostream>

#include "pool.h"

using namespace std;

class BasicIO {
public:
    BasicIO() {};
    
    int print() {
        std::cout<<"hello"<<std::endl;
        return 0;
    }
};

class BasicIOFactory {
public:
    BasicIOFactory() {
    
    };
    
    BasicIO* build() {
        return new BasicIO();
    };
    
    void release( BasicIO* io_){
        delete io_;
    }
};


typedef class IOPool<BasicIOFactory,BasicIO> ResourcePool;

#endif
