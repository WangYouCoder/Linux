#pragma once

#include <iostream>
#include <string>
#include <sys/select.h>
#include <memory>
#include "Socket.hpp"
#include "Log.hpp"

const static int num = sizeof(fd_set) * 8;

class SelectServer
{
private:
    void HandlerEvent(fd_set &rfds)
    {
        if(FD_ISSET(_listensock->GetSockfd(), &rfds))
        {
            lg.LogMessage(Info, "get a new link\n");
            // 获得连接
            std::string clientip;
            uint16_t clientport;
            Socket *sock = _listensock->AccetpConnecton(&clientip, &clientport);
            if(sock == nullptr)
            {
                lg.LogMessage(Error, "accept error\n");
            }

            lg.LogMessage(Info, "get a client, client info is: %s:%d, fd: %d\n", clientip.c_str(), clientport, sock->GetSockfd());
        }
    }
public:
    SelectServer(uint16_t port) :_port(port), _listensock(new TcpSocket()), _isruning(false)
    {}
    void InitServer()
    {
        _listensock->BuildListenSocketMethod(_port);
        for(int i = 0; i < num; i++)
        {
            _rfds_array[i] = nullptr;
        }
    }
    void Loop()
    {
        _isruning = true;
        while(_isruning)
        {
            // 我们不能直接进行accept,因为它在底层也是一个IO,如果没有设备与它建立连接，那么它也会进行阻塞
            // 所以所有的fd,都需要交给select,listensock上面有新连接,相当于读事件,有新连接,就等价于有新数据的到来
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(_listensock->GetSockfd(), &rfds);

            // 遍历数组，1. 找到最大的fd，2. 将合法的fd添加到rfds集合中

            // 定义时间
            struct timeval timeout = {5, 0};
            int n = select(_listensock->GetSockfd() + 1, &rfds, nullptr, nullptr, &timeout);
            switch(n)
            {
                case 0:
                    lg.LogMessage(Info, "select timeout..., last time: %u.%u\n",timeout.tv_sec, timeout.tv_usec);
                    break;
                case -1:
                    lg.LogMessage(Error, "select error...\n");
                    break;
                default:
                    lg.LogMessage(Info, "select success, begin event handler..., last time: %u.%u\n",timeout.tv_sec, timeout.tv_usec);
                    HandlerEvent(rfds);
                    break;
            }

        }
        _isruning = false;
    }
    ~SelectServer()
    {}
private:
    std::unique_ptr<Socket> _listensock;
    uint16_t _port;
    bool _isruning;

    Socket *_rfds_array[num];
};