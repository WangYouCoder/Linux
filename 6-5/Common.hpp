#pragma once
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cerrno>
#include "Log.hpp"


// 将指定的fd设置为非阻塞
void SetNonBlock(int fd)
{
    int f = fcntl(fd, F_GETFL);
    if(f < 0)
    {
        exit(0);
    }
    fcntl(fd, F_SETFL, f | O_NONBLOCK);
    lg.LogMessage(Info, "set %d been non block\n", fd);
}