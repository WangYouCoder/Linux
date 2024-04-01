#pragma once
#include <iostream>
#include <pthread.h>
#include <vector>
#include <semaphore.h>
#include "LockGuard.hpp"

const int defaultsize = 5;
template <class T>
class RingQueue
{
private:
    void P(sem_t &sem)
    {
        // 对信号量++
        sem_wait(&sem);
    }
    void V(sem_t &sem)
    {
        // 对信号量--
        sem_post(&sem);
    }

public:
    RingQueue(int size = defaultsize) : _size(size), _p_step(0), _c_step(0), _ringqueue(size)
    {
        sem_init(&_space_sem, 0, size);
        sem_init(&_data_sem, 0, 0);

        pthread_mutex_init(&_c_mutex, nullptr);
        pthread_mutex_init(&_p_mutex, nullptr);
    }
    void Push(const T &in)
    {
        P(_space_sem);
        LockGuard lg(&_p_mutex);
        {
            // 生产
            _ringqueue[_p_step] = in;
            _p_step++;
            _p_step %= _size;
        }
        V(_data_sem);
    }
    void Pop(T *out)
    {
        P(_data_sem);
        LockGuard lg(&_c_mutex);
        {
            // 消费
            *out = _ringqueue[_c_step];
            _c_step++;
            _c_step %= _size;
        }
        V(_space_sem);
    }
    ~RingQueue()
    {
        sem_destroy(&_space_sem);
        sem_destroy(&_data_sem);

        pthread_mutex_destroy(&_c_mutex);
        pthread_mutex_destroy(&_p_mutex);
    }

private:
    std::vector<T> _ringqueue;
    int _size;

    int _p_step; // 生产者的位置
    int _c_step; // 消费者的位置

    sem_t _space_sem;
    sem_t _data_sem;
    
    pthread_mutex_t _c_mutex;
    pthread_mutex_t _p_mutex;
};