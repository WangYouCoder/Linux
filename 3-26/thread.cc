#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <functional>
#include <time.h>
using namespace std;

//typedef function<void()> func_t;
using func_t = function<void()>;

class ThreadData
{
public:
    ThreadData(const string &name, const uint64_t &time, func_t f)
        : threadname(name), createtime(time), func(f)
    {}

public:
    string threadname;
    uint64_t createtime;
    func_t func;
};

void Print()
{
    cout << "I am new thread" << endl;
}

void *ThreadRoutine(void *args)
{
    ThreadData *td = static_cast<ThreadData *>(args);
    while (true)
    {
        cout << "threadname" << td->threadname << ", createtime" << td->createtime << endl;
        td->func();
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    ThreadData *td = new ThreadData("thread 1", (uint64_t)time(nullptr), Print);
    pthread_create(&tid, NULL, ThreadRoutine, td);

    while (true)
    {
        sleep(1);
    }

    return 0;
}