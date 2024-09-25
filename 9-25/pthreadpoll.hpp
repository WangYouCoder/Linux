#pragma onec 
#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

template<class T>
class ThreadPoll
{
private:
    bool Empty()
    {
        return _task_queue.size() == 0;
    }

    void Lock()
    {
        pthread_mutex_lock(&_mutex);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }

    void Wait()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    void WakeUp()
    {
        pthread_cond_signal(&_cond);
    }
public:
    ThreadPoll(int num = 5)
    : _thread_num(num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }
    ~ThreadPoll()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

    void ThreadPollInit()
    {
        pthread_t tid;
        for(int i = 0; i < _thread_num; i++)
        {
            pthread_create(&tid, nullptr, Routine, this);
        }
    }

    static void* Routine(void* arg)
    {
        ThreadPoll* tp = (ThreadPoll*)arg;

        while(true)
        {
            // 取任务存在竞争，所以需要加锁
            tp->Lock();

            // 有可能会有多个线程同时被唤醒，如果使用if进行判断的话，当只有一个任务到来时，多个线程被唤醒，只会有一个线程拿到任务，
            // 这时任务队列为空，而有多个线程被唤醒，其他线程应再次判断是否满足条件
            while(tp->Empty())
                tp->Wait();
            
            T task;
            tp->Get(task);
            tp->Unlock();

            task.Run();
        }
    }

    void Push(const T& task)
    {
        Lock();
        _task_queue.push(task);
        Unlock();
        WakeUp();
    }

    void Get(T &task)
    {
        task = _task_queue.front();
        _task_queue.pop();
    }
private:
    std::queue<T> _task_queue;
    int _thread_num;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};