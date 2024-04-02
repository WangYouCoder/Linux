#include <iostream>
#include <ctime>
#include <memory>
#include "threadpool.hpp"
#include "Task.hpp"

int main()
{
    // std::unique_ptr<ThreadPool<Task>> tp(new ThreadPool<Task>());
    // tp->start();

    ThreadPool<Task>::GetInstance()->start();

    srand((uint64_t)time(nullptr));

    while(true)
    {
        int x = rand() % 100 + 1;
        usleep(1000);
        int y = rand() % 100 + 1;
        usleep(1000);
        char op = opers[rand() % opers.size()];

        Task t(x, y, op);
        //tp->Push(t);
        ThreadPool<Task>::GetInstance()->Push(t);

        sleep(1);

        std::cout << "-----------------------------------------------" << std::endl;
    }

    //tp->Wait();
    ThreadPool<Task>::GetInstance()->Wait();
    return 0;
}
