#ifndef PROCESS_RESOURCE_H_TB1HI9F0
#define PROCESS_RESOURCE_H_TB1HI9F0
#include "process_utils.h"
#include "sim.h"

class Resource;
class Tools;

class ProcessResource {
private:
    Resource resource_;
	Tools tools_ ;
    
public:
    virtual ~ProcessResource () {} ;

private:
    static ProcessResource* instance_;
public:
    static ProcessResource& instance();
    
public:
    Resource& resource() {
        return resource_;
    }
    
    Tools& tools() {
        return tools_;
    }  
    
private:
    ProcessResource () {} ;
    DISALLOW_COPY_AND_ASSIGN(ProcessResource);
    /* data */
};


#endif /* end of include guard: PROCESS_RESOURCE_H_TB1HI9F0 */
