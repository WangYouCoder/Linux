
// 对指定文件描述符进行封装，将其缓冲区、对应的事件、回调函数进行封装
// 一个Connection对象代表着一个连接对象

#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Connection;
class TcpServer;
using func_t = std::function<void(Connection *)>;

class Connection
{
public:
    Connection(int sockfd, unsigned int events, TcpServer *R) : _sockfd(sockfd), _event(events), _R(R)
    {
    }
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
    void SetClient(const struct sockaddr_in c)
    {
        _client = c;
    }

    void AddInbuffer(std::string buffer)
    {
        _inbuffer += buffer;
    }

    std::string& GetInbuffer()
    {
        return _inbuffer;
    }

    void AddOutbuffer(std::string buffer)
    {
        _outbuffer += buffer;
    }

    std::string& GetOutbuffer()
    {
        return _outbuffer;
    }

    void SetEvent(uint32_t event)
    {
        _event = event;
    }

    bool IsNeedSend()
    {
        return !_outbuffer.empty();
    }

    void Close()
    {
        ::close(_sockfd);
    }
    ~Connection()
    {
    }

private:
    int _sockfd;
    // 缓冲区
    std::string _inbuffer;
    std::string _outbuffer;

    // 关系的事件
    uint32_t _event;

    struct sockaddr_in _client;

public:
    // 对特定connection进行处理的回调函数
    func_t _recver;
    func_t _sender;
    func_t _excepter;

    TcpServer *_R;
};


class TcpServer;
class ConnectionFactory
{
public:
    static Connection *BuildListenConnection(int listensock, func_t recver, unsigned int events, TcpServer *R)
    {
        Connection *conn = new Connection(listensock, events, R);
        conn->RegisterFunc(recver, nullptr, nullptr);
        return conn;
    }

    static Connection* BuildNormalConnection(int sockfd, func_t recver, func_t sender, func_t excepter, unsigned int events, TcpServer* R)
    {
        Connection *conn = new Connection(sockfd, events, R);
        conn->RegisterFunc(recver, sender, excepter);
        return conn;
    }
};