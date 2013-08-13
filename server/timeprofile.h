#ifndef __TIME_PROFILE_H__
#define __TIME_PROFILE_H__

#include <sys/time.h>

class TimeProfile{
public:
    TimeProfile(){
        start();
    };
    
    void start() {
        gettimeofday(&start_,NULL);  
    }
    
    long diff() {
        struct timeval end;
        gettimeofday(&end,NULL);
        long timeCost = (end.tv_sec - start_.tv_sec)*1000000 + (end.tv_usec-start_.tv_usec); 
        return timeCost;
    }
    
private:
    //fetch rich data
    struct timeval start_;
    
};

#endif