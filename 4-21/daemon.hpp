#pragma once

#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>

const char* root = "/";
const char *dev_null = "/dev/null";
void Daemon(bool ischdir, bool isclose)
{
    // 1. 忽略可能引起进程异常的信号
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    // 2. 让自己不要成为组长
    if(fork() > 0) exit(0);   //  父进程直接退出，子进程会执行后续的代码

    // 3. 让自己成为一个新的会话
    setsid();

    // 4. 每一个进程都有自己的CWD（当前工作路径）， 是否将当前进程的CWD更改为根目录（这样可以以绝对路径的方式访问所有的配置文件，相对路径比较麻烦）
    if(ischdir) chdir(root);

    // 5. 到这里已经变成守护进程了，不再需要和用户进程输入、输出、错误 进行关联了
    int fd = open(dev_null, O_RDWR);
    if(isclose)
    {
        close(0);
        close(1);
        close(2);
    }
    else
    {
        if(fd > 0)
        {
            dup2(fd, 0);
            dup2(fd, 1);
            dup2(fd, 2);
            close(fd);
        }
    }
    
}