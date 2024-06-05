#pragma once

#include <iostream>
#include <string>
#include "Socket.hpp"
#include "Log.hpp"
#include "Epoller.hpp"

const static int maxevents = 128;

class EpollServer
{
public:
    EpollServer(uint16_t port) : _port(port), _listensock(new TcpSocket()), _isrunning(false), _epoller(new Epoller())
    {
    }

    void Init()
    {
        _listensock->BuildListenSocketMethod(_port);
        lg.LogMessage(Info, "init socket success, listensock is : %d\n", _listensock->GetSockfd());

        _epoller->Init();
        lg.LogMessage(Info, "init epoller success\n");

        _epoller->AddEvent(_listensock->GetSockfd(), EPOLLIN);
    }

    bool Accepter(std::string *clientip, uint16_t *clientport)
    {
        lg.LogMessage(Info, "get a new link\n");

        Socket *sock = _listensock->AccetpConnecton(clientip, clientport);
        if (sock == nullptr)
        {
            lg.LogMessage(Error, "AccetpConnecton error\n");
            return false;
        }
        lg.LogMessage(Info, "get a client, client info is# %s:%d, fd: %d\n", (*clientip).c_str(), *clientport, sock->GetSockfd());

        _epoller->AddEvent(sock->GetSockfd(), EPOLLIN);
        return true;
    }

    void HandlerEvent(int n)
    {
        lg.LogMessage(Debug, "%d 个 events 已经就绪\n", n);
        for (int i = 0; i < n; i++)
        {
            lg.LogMessage(Debug, "ready fd: %d, Event is: %u\n", _revs[i].data.fd, _revs[i].events);
            int sockfd = _revs[i].data.fd;
            uint32_t events = _revs[i].events;
            if (events & EPOLLIN)
            {
                // 1. listensock就绪  2. 正常sockfd
                if (sockfd == _listensock->GetSockfd())
                {
                    std::string clientip;
                    uint16_t clientport;
                    if(!Accepter(&clientip, &clientport)) continue;
                    lg.LogMessage(Info, "accept client success, client[%s:%d]\n", clientip.c_str(), clientport);
                }
                else
                {
                    char buffer[1024];
                    ssize_t n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
                    if(n > 0)
                    {
                        buffer[n] = 0;
                        lg.LogMessage(Info, "client# %s\n", buffer);
                        std::string message = buffer;
                        std::string echo_message = "echo message: " + message;
                        send(sockfd, echo_message.c_str(), echo_message.size(), 0);
                    }
                    else
                    {
                        lg.LogMessage(Warning, "client quit, maybe close or error, close fd : %d\n", sockfd);
                        _epoller->DelEvent(sockfd);
                        close(sockfd);
                    }
                }
            }
        }
    }

    void Loop()
    {
        _isrunning = true;
        while (_isrunning)
        {
            int timeout = -1;
            int n = _epoller->Wait(_revs, maxevents, timeout);
            switch (n)
            {
            case 0:
                lg.LogMessage(Debug, "timeout...\n");
                break;
            case -1:
                lg.LogMessage(Error, "epoll wait failed\n");
                break;
            default:
                lg.LogMessage(Info, "event happend...\n");
                HandlerEvent(n);
                break;
            }
        }
        _isrunning = false;
    }
    ~EpollServer()
    {
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensock;
    std::unique_ptr<Epoller> _epoller;
    bool _isrunning;
    struct epoll_event _revs[maxevents];
};