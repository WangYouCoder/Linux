#pragma once

#include <iostream>
#include <string>
#include <poll.h>
#include <memory>
#include "Socket.hpp"
#include "Log.hpp"

const int gnum = 1024;

class PollServer
{
private:
    void HandlerEvent()
    {
        // std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < _num; i++)
        {
            if (_rfds[i].fd == -1)
                continue;
            // 读事件分两类，一类是新连接到来，一类是新数据到来
            int fd = _rfds[i].fd;
            short revents = _rfds[i].revents;

            if (revents & POLLIN)
            {
                // std::cout << fd << " " << _listensock->GetSockfd() << std::endl;
                //  新连接到来了
                if (fd == _listensock->GetSockfd())
                {
                    lg.LogMessage(Info, "get a new link\n");
                    // 获取连接
                    std::string clientip;
                    uint16_t clientport;
                    // 不会阻塞！！，因为select已经检测到了listensock已经就绪了
                    Socket *sock = _listensock->AccetpConnecton(&clientip, &clientport);
                    if (sock->GetSockfd() == -1)
                    {
                        lg.LogMessage(Error, "accept error\n");
                        continue;
                    }
                    lg.LogMessage(Info, "get a client, client info is# %s:%d, fd: %d\n", clientip.c_str(), clientport, sock->GetSockfd());

                    // 这里已经获取连接成功了，接下来怎么办？？？
                    // read？write？绝对不能！！！read 底层数据是否就绪时不确定的！谁清楚fd上面是否有读事件呢？poll！
                    // 新链接fd到来的时候，要把新的fd, 想办法交给poll托管 -- 只需要添加到数组_rfds中即可
                    int pos = 0;
                    for (; pos < _num; pos++)
                    {
                        if (_rfds[pos].fd == -1)
                        {
                            _rfds[pos].fd = sock->GetSockfd();
                            _rfds[pos].events = POLLIN;
                            lg.LogMessage(Info, "get a new link, fd is : %d\n", sock->GetSockfd());
                            break;
                        }
                    }
                    if (pos == _num)
                    {
                        // 1. 扩容
                        // 2. 关闭
                        close(sock->GetSockfd());
                        lg.LogMessage(Warning, "server is full...!\n");
                    }
                }
                else
                {
                    // 普通的读事件就绪
                    // 读数据是有问题的
                    // 这一次读取不会被卡住吗？
                    char buffer[1024];
                    ssize_t n = recv(fd, buffer, sizeof(buffer - 1), 0); // 这里读取会阻塞吗？不会！
                    if (n > 0)
                    {
                        buffer[n] = 0;
                        lg.LogMessage(Info, "client say# %s\n", buffer);
                        std::string message = "hello ";
                        message += buffer;
                        send(fd, message.c_str(), message.size(), 0);
                    }
                    else
                    {
                        lg.LogMessage(Warning, "client quit, maybe close or error, close fd : %d\n", fd);
                        close(fd);
                        // 取消poll的关心
                        _rfds[i].fd = -1;
                        _rfds[i].events = 0;
                        _rfds[i].revents = 0;
                    }
                }
            }
        }
    }

public:
    PollServer(uint16_t port) : _port(port), _listensock(new TcpSocket()), _isruning(false), _num(gnum)
    {
    }
    void InitServer()
    {
        _listensock->BuildListenSocketMethod(_port);
        _rfds = new struct pollfd[_num];
        for (int i = 0; i < _num; i++)
        {
            _rfds[i].fd = -1;
            _rfds[i].events = 0;
            _rfds[i].revents = 0;
        }
        // 最开始的只有一个文件描述符
        _rfds[0].fd = _listensock->GetSockfd();
        _rfds[0].events |= POLLIN;
    }
    void Loop()
    {
        _isruning = true;
        while (_isruning)
        {
            // 定义时间
            int timeout = -1;
            int n = poll(_rfds, _num, timeout);
            switch (n)
            {
            case 0:
                lg.LogMessage(Info, "select timeout...\n");
                break;
            case -1:
                lg.LogMessage(Error, "select error...\n");
                break;
            default:
                lg.LogMessage(Info, "select success, begin event handler...\n");
                HandlerEvent();
                break;
            }
        }
        _isruning = false;
    }
    ~PollServer()
    {
        delete[] _rfds;
    }

private:
    std::unique_ptr<Socket> _listensock;
    uint16_t _port;
    bool _isruning;
    struct pollfd *_rfds;
    int _num = gnum;
};
