#include <iostream>
#include <unistd.h>
#include "Thread.hpp"
#include "LockGuard.hpp"

std::string GetThreadname()
{
    char name[64];
    static int cnt = 1;
    snprintf(name, sizeof(name), "Thread-%d", cnt++);
    return name;
}

int ticket = 1000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 创建锁，并初始化

// 加锁：
// 1. 我们要尽可能的给少的代码块加锁
// 2. 一般加锁，都是给临界区加锁

void GetTicket(const std::string &name)
{
    while (true)
    {
        // pthread_mutex_lock(&mutex);
        {
            LockGuard lg(&mutex);
            if (ticket > 0)
            {
                usleep(1000); // 模拟抢票所需要花费的时间
                printf("%s get a ticket: %d\n", name.c_str(), ticket);
                ticket--;

                // 这里也写就是条件满足时,执行完这一部分代码块,就解锁，减少被加锁的代码数量
                // pthread_mutex_unlock(&mutex);
            }
            else
            {
                // 因为不能再下面写,就只能在break之前写了
                // pthread_mutex_unlock(&mutex);
                break;
            }
            // 解锁不能写道这里，因为如果ticket == 0,条件不满足就会直接break,就不会执行解锁功能了
        }
    }
}

    int main()
    {
        std::string name = GetThreadname();
        Thread<std::string> t1(GetTicket, name, name);
        name = GetThreadname();
        Thread<std::string> t2(GetTicket, name, name);
        name = GetThreadname();
        Thread<std::string> t3(GetTicket, name, name);
        name = GetThreadname();
        Thread<std::string> t4(GetTicket, name, name);
        t1.Start();
        t2.Start();
        t3.Start();
        t4.Start();
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        return 0;
    }

    // //使用方视角
    // std::string GetThreadname()
    // {
    //     char name[64];
    //     static int cnt = 1;
    //     snprintf(name, sizeof(name), "Thread-%d", cnt++);
    //     return name;
    // }

    // void Print(int num)
    // {
    //     while(true)
    //     {
    //         std::cout << "hello world" << "num: " << num-- << std::endl;
    //         sleep(1);
    //     }
    // }

    // int main()
    // {
    //     Thread<int> t(Print, GetThreadname(), 10);
    //     std::cout << "is thread runing? " << t.IsRuning() << std::endl;

    //     t.Start();
    //     std::cout << "is thread runing? " << t.IsRuning() << std::endl;

    //     t.join();
    //     std::cout << "is thread runing? " << t.IsRuning() << std::endl;

    //     return 0;
    // }