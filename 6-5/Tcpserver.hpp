#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "Connection.hpp"
#include "Epoller.hpp"
#include "Log.hpp"

class TcpServer;
class ConnectionFactory
{
public:
    Connection *BuildListenConnection(int listensock, func_t recver)
    {
        Connection *conn = new Connection(listensock);
        conn->RegisterFunc(recver, nullptr, nullptr);
        return conn;
    }
};

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
        _epoller.AddEvent(conn->Getevent(), conn->Getevent());
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