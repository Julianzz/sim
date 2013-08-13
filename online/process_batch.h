#ifndef PROCESS_BATCH_H_1645RIHZ
#define PROCESS_BATCH_H_1645RIHZ

#include <pthread.h>
#include <vector>
#include <string>

#include "process_strategy.h"
#include "process_input.h"
#include "process_geo_search.h"
#include "process_sync.h"
#include "sim.h"


class BatchWorker {
public:
    virtual bool process( ) = 0;
};

class ThreadBatchLoad {
private:
    BatchWorker* worker_;
    std::vector<pthread_t> threads_;
    size_t num_;
    
public:
    ThreadBatchLoad(size_t num ,BatchWorker* worker ):worker_(worker),
        num_(num) {
    }
    static void * threadMain(void *arg) {
        ThreadBatchLoad*loader = (ThreadBatchLoad*)arg;
        loader->process(); 
        return NULL;
    }
    
    void process() {
        worker_->process();
    }
    
    bool buildPool() ;
    bool waitPool( );
    bool start( ) ;
    
};

class SingleBatchLoader {
private:
    BatchWorker* worker_;
    size_t num_;
public:
    SingleBatchLoader(size_t num ,BatchWorker* worker ):
        worker_(worker),
        num_(num) {
    }
    bool start( ){
        worker_->process();
        return true;
    }

};

class BatchRecords: public BatchWorker {
    
public:
    typedef map<unsigned long long, PreProcData> DataRecord;
    
public:
    
    class Iterator : public std::iterator<std::input_iterator_tag, BatchRecords > {
        DataRecord::iterator p_ ;
        public:
          Iterator(DataRecord::iterator x ) :p_(x) {
          }
          Iterator(const Iterator& mit) : p_(mit.p_) {}
          
          Iterator& operator++() {
              ++p_;
              return *this;
          }
          Iterator operator++(int) {
              Iterator tmp(*this); 
              operator++(); 
              return tmp;
          }
          bool operator==(const Iterator& rhs) {
              return p_==rhs.p_;
          }
          bool operator!=(const Iterator& rhs) {
              return p_!=rhs.p_;
          }
          PreProcData& operator*() {
              return p_->second;
          }
    };

public:
    Iterator begin() {
        return Iterator(records.begin());
    }
    Iterator end() {
        return Iterator(records.end());
    }
    
public:
    BatchRecords(ProcessStrategy* strategy,BasicInput* input,bool isBase ):
        strategy_(strategy),
        isBase_( isBase ) {
            
        mutexInput_ = new MutexInput( input );
        //pthread_mutex_init( &lock_, NULL );
        //pthread_mutex_init( &readLock_, NULL);
        search_ = new geo::GridSearch();
        resource_ = &( ProcessResource::instance().resource() );
        begin_ = false;
        std::cout<<"init begin_" <<begin_<<std::endl;
    }
    ~BatchRecords() {
        delete mutexInput_;
        mutexInput_ = NULL;
       // pthread_mutex_destroy(&lock_);
        delete search_;
    }
    
public:

    // compute idfs and others
    bool initRecord( const Record& record );
    bool updateRedource(const Record& record,PreProcData& data) ;
    
    virtual bool process();
    
    size_t size() {
        return records.size();
    }  
    
    void reset() {
        begin_ = false;
    }
    PreProcData* next( );
    
    //PreProcData* fetch( GridItem& key ) {
    PreProcData* fetch( const geo::GridItem& key );
    bool search( const PreProcData& data, geo::GridResult*result);

public:
    
    DataRecord records;
    ProcessStrategy* strategy_;
    Locker lock_;
    bool isBase_;
    
    MutexInput* mutexInput_ ;
    geo::GridSearch* search_;
    Resource* resource_;
    map<unsigned long long, PreProcData>::iterator cur_;
    bool begin_;
    
    Locker readLock_;
    
};

class MutexBatchRecords {
public:
    MutexBatchRecords(BatchRecords& records ):records_(records) {} 
    ~MutexBatchRecords() {
    
    }; 
    
private:
    Locker lock_;
    BatchRecords& records_;
};




#endif /* end of include guard: PROCESS_BATCH_H_1645RIHZ */
