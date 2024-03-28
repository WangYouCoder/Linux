#pragma once
#include<iostream>
#include<string>
#include<functional>
#include<pthread.h>

template<class T>
using func_t = std::function<void(T)>;

//应用方视角
template<class T>
class Thread
{
public:
    Thread(func_t<T> func,const std::string& threadname, T data)
    :_tid(0),_threadname(threadname),_isruning(false),_func(func),_data(data)
    {}
 
    static void* ThreadRoutine(void* args)
    {
        Thread* ts = static_cast<Thread*>(args); 
        ts->_func(ts->_data);
        return nullptr;
    } 

    bool Start()
    {
        int n = pthread_create(&_tid, nullptr, ThreadRoutine, this); // *******
        if(n == 0)
        {
            _isruning = true;
            return true;
        }
        return false;
    }
    bool join()
    {
        if(!_isruning) return true;
        int n = pthread_join(_tid, nullptr);
        if(n == 0)
        {
            _isruning = true;
            return true;
        }
        return false;
    }
    bool IsRuning()
    {
        return _isruning;
    }

    ~Thread()
    {}
private:
    pthread_t _tid;
    std::string _threadname;
    bool _isruning;
    func_t<T> _func;
    T _data;
};