#pragma once
#include "Socket.hpp"
#include <pthread.h>
#include <functional>
using func_t = std::function<std::string(std::string &, bool *error_code)>;

class TcpServer;

class ThreadData
{
public:
    ThreadData(TcpServer *tcp_this, WY::Socket *sockp) : _tcp_this(tcp_this), _sockp(sockp)
    {
    }

public:
    TcpServer *_tcp_this;
    WY::Socket *_sockp;
};

class TcpServer
{
public:
    TcpServer(uint16_t port, func_t handler_resquest)
        : _port(port), _listensocket(new WY::TcpSocket()), _handler_request(handler_resquest)
    {
        _listensocket->BindListenSocket(_port, WY::backlog);
    }
    static void *ThreadRoutine(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);

        std::string inbufferstream;
        while (true)
        {
            bool ok = true;
            // 读数据 -- 不关心数据是什么，只读取
            // 1. 读取报文
            if (!td->_sockp->Recv(&inbufferstream, 1024))
                break;
            // 2. 报文处理
            std::string send_string = td->_tcp_this->_handler_request(inbufferstream, &ok);
            if (ok)
            {
                // 发送数据 -- 不关心数据是什么，只发送
                // 3. 发送数据
                if (!send_string.empty())
                {
                    td->_sockp->Send(send_string);
                }
            }
            else
            {
                break;
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
    ~TcpServer()
    {
        delete _listensocket;
    }

private:
    int _port;
    WY::Socket *_listensocket;

public:
    func_t _handler_request;
};