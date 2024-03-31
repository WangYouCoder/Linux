#pragma once
#include<iostream>
#include<queue>
#include<pthread.h>
const int capacity = 5;

template<class T>
class BlockQueue
{
public:
    BlockQueue(int cap = capacity):_capacity(cap)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_c_cond, nullptr);
        pthread_cond_init(&_p_cond, nullptr);
    }

    bool Full()
    {
        return _capacity == _q.size();
    }

    bool Empty()
    {
        return _q.size() == 0;
    }

    void Push(const T& in) //生产者生产数据
    {
        pthread_mutex_lock(&_mutex);
        if(Full())
        {
            // 队列已经满了，阻塞等待，等待消费线程进行消费
            pthread_cond_wait(&_p_cond, &_mutex);
        }
        _q.push(in);
        pthread_cond_signal(&_c_cond); //生产者一生产，就可以唤醒消费者线程了
        pthread_mutex_unlock(&_mutex);
    }

    void Pop(T* out) // 消费者消费数据
    {
        pthread_mutex_lock(&_mutex);
        if(Empty())
        {
            // 队列已经为空，阻塞等待，等待生产线程生产数据
            pthread_cond_wait(&_c_cond, &_mutex);
        }
        *out = _q.front();
        _q.pop();
        pthread_cond_signal(&_p_cond);//消费者一消费，就可以唤醒生产者线程进行生产了
        pthread_mutex_unlock(&_mutex);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_c_cond);
        pthread_cond_destroy(&_p_cond);
    }
private:
    std::queue<T> _q;
    int _capacity;
    pthread_mutex_t _mutex;
    pthread_cond_t _c_cond;
    pthread_cond_t _p_cond;
};