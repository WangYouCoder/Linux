#pragma once
#include "Socket.hpp"
#include <pthread.h>
#include <functional>

// http底层采用的是tcp协议
using func_t = std::function<std::string(std::string &request)>;

class HttpServer;

class ThreadData
{
public:
    ThreadData(HttpServer *tcp_this, WY::Socket *sockp) : _tcp_this(tcp_this), _sockp(sockp)
    {
    }

public:
    HttpServer *_tcp_this;
    WY::Socket *_sockp;
};

class HttpServer
{
public:
    HttpServer(uint16_t port, func_t handler_resquest)
        : _port(port), _listensocket(new WY::TcpSocket()), _handler_request(handler_resquest)
    {
        _listensocket->BindListenSocket(_port, WY::backlog);
    }
    static void *ThreadRoutine(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);

        std::string http_request;

        // 读数据 -- 不关心数据是什么，只读取
        // 1. 读取报文
        if (td->_sockp->Recv(&http_request, 1024))
        {
            // 2. 报文处理
            std::string http_response = td->_tcp_this->_handler_request(http_request);

            // 发送数据 -- 不关心数据是什么，只发送
            // 3. 发送数据
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
            WY::Socket *newsock = _listensocket->AcceptConnection(&peerip, &peerport);
            if (newsock == nullptr)
                continue;
            std::cout << "获取了一个新连接, sockfd: " << newsock->GetSockfd()
                      << " client info " << peerip << ": " << peerport << std::endl;
            // newsock->CloseSocket();

            pthread_t tid;
            ThreadData *td = new ThreadData(this, newsock);
            pthread_create(&tid, nullptr, ThreadRoutine, td);
        }
    }
    ~HttpServer()
    {
        delete _listensocket;
    }

private:
    int _port;
    WY::Socket *_listensocket;

public:
    func_t _handler_request;
};