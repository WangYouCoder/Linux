
// Connection是将每一个连接对象的各个属性描述组装了起来，TcpServer是将其再进行管理
// 由于可能会有多个连接到来，所以需要将所有的Connection与其相对应的fd管理起来 unorder_map
// 而每一个fd由有其相对于的读写事件，因此需要一个struct epoll_event 数组将所有的事件管理起来

// 解释Connection中的回调函数：当在就绪队列中发现已就绪事件后，会进行识别是哪一种事件(读、写、异常)
// 识别出来后，通过其fd找到所对应的Connection对象，在使用Connection对象通过回调函数，调用其相应事件的函数

#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "Connection.hpp"
#include "Epoller.hpp"
#include "Log.hpp"



// 用来管理Connection
class TcpServer
{
    static const int gmaxevents = 64;

public:
    TcpServer() : _isrunning(false), _epoller(Epoller()), _timeout(-1)
    {
    }
    void Init()
    {
        _epoller.Init();
    }
    void AddConnection(Connection *conn)
    {
        if (conn == nullptr)
            return;
        _connections[conn->Getsockfd()] = conn;
        lg.LogMessage(Info, "add a new connection success, sockfd: %d\n", conn->Getsockfd());
        // 让epoller管理起来
        _epoller.AddEvent(conn->Getsockfd(), conn->Getevent());
    }

    void RemoveConnection(int sockfd)
    {
        if(!IsConnectionExist(sockfd)) return;
        // 1. 清理connection自己关心的事件
        _connections[sockfd]->SetEvent(0);
        // 2. 清理epoller的关心
        _epoller.DelEvent(sockfd);
        // 3. 移除在_connections中的key value
        _connections.erase(sockfd);
    }

    bool IsConnectionExist(int sockfd)
    {
        auto iter = _connections.find(sockfd);
        if(iter == _connections.end())
            return false;
        else 
            return true;
    }

    void EnableReadWrite(int sockfd, bool readable, bool writeable)
    {
        if(!IsConnectionExist(sockfd)) return;
        
        uint32_t event = ((readable ? EPOLLIN : 0) | (writeable ? EPOLLOUT : 0) | EPOLLET);
        // 修改Connection中关心的事件进行修改
        _connections[sockfd]->SetEvent(event);
        // 将关心的事件设置到epoller当中
        _epoller.Modify(sockfd, event);
    }

    void Looponce()
    {
        int n = _epoller.Wait(_revs, gmaxevents, _timeout);
        for (int i = 0; i < n; i++)
        {
            int sockfd = _revs[i].data.fd;
            uint32_t event = _revs->events;

            // 将所有的epoll检测到的fd的问题，统一转化成为读写错误，在后面统一处理
            if (event & EPOLLERR)
                event |= (EPOLLIN | EPOLLOUT);
            if (event & EPOLLHUP)
                event |= (EPOLLIN | EPOLLOUT);

            if (event & EPOLLIN)
            {
                // 读事件
                if (_connections[sockfd]->_recver)
                    _connections[sockfd]->_recver(_connections[sockfd]);
            }
            if (event & EPOLLOUT)
            {
                // 写事件
                if (_connections[sockfd]->_sender)
                    _connections[sockfd]->_sender(_connections[sockfd]);
            }
        }
    }

    void Dispatch()
    {
        _isrunning = true;
        while (_isrunning)
        {
            // std::cout << "---------------" << std::endl;
            Looponce();
        }
        _isrunning = false;
    }
    ~TcpServer() {}

private:
    Epoller _epoller;
    struct epoll_event _revs[gmaxevents];
    int _timeout;

    std::unordered_map<int, Connection *> _connections;
    bool _isrunning;
};