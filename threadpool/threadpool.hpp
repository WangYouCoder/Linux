#pragma once
#include <iostream>
#include "Log.hpp"
#include "Thread.hpp"
#include "LockGuard.hpp"
#include "Task.hpp"
#include <queue>
#include <functional>
#include <vector>
#include <unistd.h>

const int defaultnum = 5;

class ThreadData
{
public:
    ThreadData(const std::string &name) : threadname(name)
    {}
    ~ThreadData()
    {}
public:
    std::string threadname;
};

template<class T>
class ThreadPool
{
private:
    ThreadPool(int thread_num = defaultnum):_thread_num(thread_num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        //创建线程
        for(int i = 0; i < _thread_num; i++)
        {
            std::string threadname = "thread-";
            threadname += std::to_string(i+1);

            ThreadData td(threadname);

            // Thread<ThreadData> t(threadname, std::bind(&ThreadPool<T>::ThreadRun, this, std::placeholders::_1), td);
            // _threads.push_back(t);
            _threads.emplace_back(threadname, std::bind(&ThreadPool<T>::ThreadRun, this, std::placeholders::_1), td);
            lg.LogMessage(Info, "%s is created...\n", threadname.c_str());
        }
    }
    ThreadPool(const ThreadPool<T>& tp) = delete;
    const ThreadPool<T>& operator=(const ThreadPool<T>) = delete;
public:
    static ThreadPool<T> *GetInstance()
    {
        if(instance == nullptr)
        {
            instance = new ThreadPool<T>();
        }
        return instance;
    }
    bool start()
    {
        for(auto& thread : _threads)
        {
            thread.Start();
            lg.LogMessage(Info, "%s is runing...\n", thread.ThreadName().c_str());
        }
        return true;
    }
    void ThreadWait(const ThreadData& td)
    {
        lg.LogMessage(Debug, "no task, %s is sleeping...\n", td.threadname.c_str());
        pthread_cond_wait(&_cond, &_mutex);
    }
    void ThreadWackUp()
    {
        pthread_cond_signal(&_cond);
    }
    void ThreadRun(ThreadData& td)
    {
        while(true)
        {
            T t;
            {
                LockGuard lockguard(&_mutex);
                while(_q.empty())
                {
                    ThreadWait(td);
                    lg.LogMessage(Debug, "%s is wake up...\n", td.threadname.c_str());
                }
                t = _q.front();
                _q.pop();
            }
            //处理任务
            t();
            lg.LogMessage(Debug, "%s handler task %s done, result is : %s\n",
                td.threadname.c_str(), t.PrintTask().c_str(), t.PrintResult().c_str());
        }
    }
    void Push(T& in)
    {
        lg.LogMessage(Debug, "other thread push a task, task is %s\n", in.PrintTask().c_str());
        LockGuard lockguard(&_mutex);
        _q.push(in);
        ThreadWackUp();
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

    bool Wait()
    {
        for(auto& thread : _threads)
        {
            thread.join();
        }
        return true;
    }
private:
    std::queue<T> _q;
    std::vector<Thread<ThreadData>> _threads;
    int _thread_num;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;

    static ThreadPool<T> *instance;
    static pthread_mutex_t sig_lock;
};

template<class T> 
ThreadPool<T> *ThreadPool<T>::instance = nullptr;

template<class T> 
pthread_mutex_t ThreadPool<T>::sig_lock = PTHREAD_MUTEX_INITIALIZER;
