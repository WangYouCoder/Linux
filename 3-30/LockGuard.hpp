#pragma once
#include <pthread.h>

class Lock
{
public:
    Lock(pthread_mutex_t* lock) :_lock(lock)
    {}
    void lock()
    {
        pthread_mutex_lock(_lock);
    }
    void unlock()
    {
        pthread_mutex_unlock(_lock);
    }
    ~Lock()
    {}
private:
    pthread_mutex_t *_lock;
};

class LockGuard
{
public:
    LockGuard(pthread_mutex_t* lock) :_mutex(lock)
    {
        _mutex.lock();
    }
    ~LockGuard()
    {
        _mutex.unlock();
    }
private:
    Lock _mutex;
};