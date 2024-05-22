#pragma once

#include "Socket.hpp"
#include <iostream>
#include <pthread.h>
#include <functional>

// http协议底层采用的是tcp协议
using func_t = std::function<std::string(std::string &request)>; // &必须的

class TcpServer;

class ThreadData
{
public:
    ThreadData(TcpServer *tcp_this, Socket *sockp) : _this(tcp_this), _sockp(sockp)
    {
    }

public:
    TcpServer *_this;
    Socket *_sockp;
};

class TcpServer
{
public:
    TcpServer(uint16_t port, func_t handler_request)
        : _port(port), _listensocket(new TcpSocket()), _handler_request(handler_request)
    {
        _listensocket->BuildListenSocketMethod(_port, backlog);
    }
    static void *ThreadRun(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);

        std::string http_request;

        if (td->_sockp->Recv(&http_request, 4096)) //不一定
        {
            // 2. 报文处理
            std::string http_response = td->_this->_handler_request(http_request); // 回调不仅仅是调出去了，还会回来！！
            if (!http_response.empty())
            {
                td->_sockp->Send(http_response);
            }
        }

        td->_sockp->CloseSocket();
        delete td->_sockp;
        delete td;
        return nullptr;
    }
    void Loop()
    {
        while (true)
        {
            std::string peerip;
            uint16_t peerport;
            Socket *newsock = _listensocket->AcceptConnection(&peerip, &peerport);
            if (newsock == nullptr)
                continue;
            std::cout << "获取一个新连接, sockfd: " << newsock->GetSockFd() << " client info: " << peerip << ":" << peerport << std::endl;

            pthread_t tid;
            ThreadData *td = new ThreadData(this, newsock);
            pthread_create(&tid, nullptr, ThreadRun, td);
        }
    }
    ~TcpServer()
    {
        delete _listensocket;
    }

private:
    int _port;
    Socket *_listensocket;

public:
    func_t _handler_request;
};