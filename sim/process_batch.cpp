#include "process_batch.h"
#include "process_sync.h"

bool ThreadBatchLoad::buildPool() {
	for (size_t i = 0; i < num_ ; ++i) {
		pthread_t tid;
        //std::cout<<"------:"<<i<<std::endl;
		int error = pthread_create(&tid, NULL, ThreadBatchLoad::threadMain, (void *)this );
		if ( error ) {
			cout << "[Err]: phread is not created..." << endl;
			return false;
		}
		threads_.push_back(tid);
	}  
    std::cout <<"finish load thead "<<std::endl;
    return true ;    
}

bool ThreadBatchLoad::waitPool( ) {
	for (size_t i = 0; i < threads_.size() ; ++i) {
		pthread_join(threads_[i], NULL);
	}
    return true;
}

bool ThreadBatchLoad::start( ) {
    bool ret = buildPool();
    if ( !ret ) {
        std::cout<<" error in build thread pool " <<std::endl;
        return false;
    }
    ret = waitPool();
    if ( !ret ) {
        std::cout<<" error in wait pool "<<std::endl;
        return false;
    }
    return true;
}


bool BatchRecords::search( const PreProcData& data, geo::GridResult*result) {
    
	int level = 0;
	if (data.m_type == "271020" || data.m_type == "271016"
			|| data.m_type == "261000"
			|| data.m_type == "261100") {
		level = 2;
	} else {
		level = 1;
	}
    search_->search(data,level,*result);
    return true;
}


PreProcData* BatchRecords::fetch( const geo::GridItem& key ) {
    map<unsigned long long, PreProcData>::iterator iter = records.find(key);
    if( iter != records.end() ) {
        return &(iter->second);
    }
    return NULL;
}

PreProcData* BatchRecords::next( ) {
    
    ScopeLocker locker(readLock_);
    //std::cout<<"first  "<<begin_ <<std::endl;
    if( !begin_ ) {
       // std::cout<< "init" <<std::endl;
        begin_ = true;
        cur_ = records.begin();
    }
    
    //std::cout<<"next  "<<begin_ <<std::endl;
    PreProcData* data = NULL;
    //readLock_.lock();
    if( cur_ == records.end() ) {
        data = NULL;
    }else {
        data = &(cur_->second);
    }
    cur_++;
    //readLock_.unlock();
    return data;
}


bool BatchRecords::process() {
    
    while( true ) {
        Record record;
        InputState stat = mutexInput_->next(&record);
        if( stat != INPUT_SUCCESS ) {
            //std::cout<<"finishi with stat = "<< stat <<std::endl;
            return true;
        }
        bool ret = initRecord( record );
        if( !ret ) {
            std::cout<<" error in process record ";
            return false;
        }
    }
    return true;
}

bool BatchRecords::updateRedource(const Record& record, PreProcData& data) {
    ScopeLocker locker(lock_);
    records[ record.m_id ] = data;
    search_->put(data,*resource_);
    if( !strategy_->userLocalIdf() ) {
        statIdf(data, resource_->idf );
    }
    statTel(data,resource_->uselessTelMap);
    return true;
}

bool BatchRecords::initRecord( const Record& record ) {

    PreProcData data; 
    strategy_->initRecord(record,&data,isBase_ );
    updateRedource(record,data);
    
    return true;
}