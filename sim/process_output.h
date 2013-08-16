#ifndef PROCESS_OUTPUT_H_EDVV1WXW
#define PROCESS_OUTPUT_H_EDVV1WXW

#include "process_result.h"
#include "process_sync.h"

class BasicOutput {
public:
    virtual ~BasicOutput() {} ;
    virtual void output(const StrategyResult& result, bool isBase ) = 0;
};

class MutexStdOutput: public BasicOutput {
private:
    Locker locker_;
public:
    MutexStdOutput() {} ;
    virtual ~MutexStdOutput() {} 
    void output(const StrategyResult& result,bool isBase );
};

#endif /* end of include guard: PROCESS_OUTPUT_H_EDVV1WXW */
