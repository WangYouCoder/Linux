#include <iostream>
#include <unistd.h>
#include "daemon.hpp"

int main()
{
    // 变成守护进程
    Daemon();

    // 执行的核心代码
    while(true)
    {
        sleep(1);
    }
    return 0;
}