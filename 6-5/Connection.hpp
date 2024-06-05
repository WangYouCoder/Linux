#pragma once
#include <iostream>
#include <string>
#include <functional>

class Connection;
class TcpServer;
using func_t = std::function<void(Connection*)>;

class Connection
{
public:
    Connection(int sockfd) :_sockfd(sockfd)
    {}
    void RegisterFunc(func_t recver, func_t sender, func_t excepter)
    {
        _recver = recver;
        _sender = sender;
        _excepter = excepter;
    }
    int Getsockfd()
    {
        return _sockfd;
    }
    uint32_t Getevent()
    {
        return _event;
    }
    ~Connection()
    {}
private:
    int _sockfd;
    // 缓冲区
    std::string _inbuffer;
    std::string _outbuffer;

    TcpServer *R;

    // 关系的事件
    uint32_t _event;
public:
    // 对特定connection进行处理的回调函数
    func_t _recver;
    func_t _sender;
    func_t _excepter;
};

