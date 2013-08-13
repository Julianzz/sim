#ifndef PROCESS_SYNC_H_Q7ZDWI9I
#define PROCESS_SYNC_H_Q7ZDWI9I

#include <pthread.h>
class Locker {
private:
    pthread_mutex_t mutex;
public:
    Locker() {
        pthread_mutex_init(&mutex,NULL);
    }
    ~Locker() {
        pthread_mutex_destroy(&mutex);
    }
    
    void lock() {
        pthread_mutex_lock(&mutex);
    }
    void unlock() {
        pthread_mutex_unlock(&mutex);
    }    
};

class ScopeLocker {
private:
    Locker& locker_;
public:
    ScopeLocker(Locker& locker):locker_(locker) {
        locker_.lock();
    } 
    ~ScopeLocker() {
        locker_.unlock();
    }
};


#endif /* end of include guard: PROCESS_SYNC_H_Q7ZDWI9I */
