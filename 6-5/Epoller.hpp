
// Epoll功能表

// 1.创建epoll模型            epoll_create
// 2.向epoll中添加事件        AddEvent
// 3.从就绪队列中获取就绪事件  wait
// 4.从epoll中删除事件        DelEvent

#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include "Log.hpp"
#include <sys/epoll.h>
#include <unistd.h>

const static int size = 128;

class Epoller
{
public:
    Epoller() : _epfd(-1)
    {
    }

    void Init()
    {
        _epfd = ::epoll_create(size);
        if (_epfd < 0)
        {
            lg.LogMessage(Fatal, "epoll_create error, %s : %d\n", strerror(errno), errno);
            exit(-1);
        }
        lg.LogMessage(Info, "epoll_create suceess, epfd: %d\n", _epfd);
    }

    void AddEvent(int sockfd, uint32_t events)
    {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = sockfd;

        int n = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, sockfd, &ev);
        if (n < 0)
        {
            lg.LogMessage(Error, "epoll_ctl add fatal, %s : %d\n", strerror(errno), errno);
        }
    }

    void Modify(int sockfd, uint32_t event)
    {
        struct epoll_event ev;
        ev.events = event;
        ev.data.fd = sockfd;

        int n = ::epoll_ctl(_epfd, EPOLL_CTL_MOD, sockfd, &ev);
        if (n < 0)
        {
            lg.LogMessage(Error, "epoll_ctl mod fatal, %s : %d\n", strerror(errno), errno);
        }
    }

    int Wait(struct epoll_event *revs, int maxevents, int timeout)
    {
        struct epoll_event ev;
        int n = ::epoll_wait(_epfd, revs, maxevents, timeout);
        return n;
    }

    void DelEvent(int sockfd)
    {
        int n = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, sockfd, nullptr);
        if (n < 0)
        {
            lg.LogMessage(Error, "epoll_ctl del error, %s : %d\n", strerror(errno), errno);
        }
    }

    ~Epoller()
    {
        if (_epfd >= 0)
        {
            close(_epfd);
        }
        lg.LogMessage(Info, "epoll close success");
    }

private:
    int _epfd;
};