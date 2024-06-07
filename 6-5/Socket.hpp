#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Common.hpp"
const static int defaultsockfd = -1;
const static int defaultbacklog = 5;

#define Conv(addr) ((struct sockaddr *)addr)
enum
{
    SocketError = 1,
    BindError,
    ListenError,

};

class Socket
{
public:
    virtual ~Socket() {}
    virtual void CreateSocketOrDie() = 0;                                         // 创建套接字
    virtual void BindSocketOrDie(uint16_t port) = 0;                              // 服务端   将套接字与对应的文件描述符进行绑定
    virtual void ListenSocketOrDie(int backlog = defaultbacklog) = 0;             // 服务端   设置套接字为listen状态
    virtual Socket *AccetpConnecton(std::string *peerip, uint16_t *peerport) = 0; // 服务端  与客户端建立连接，并获得客户端的ip和port
    virtual bool ConnectServer(std::string &serverip, uint16_t serverport) = 0;   // 客户端  与服务端建立连接
    virtual int GetSockfd() = 0;                                                  // 获得文件描述符
    virtual void SetSockfd(int socket) = 0;                                       // 设置文件描述符的状态
    virtual void CloseSockfd() = 0;
    virtual bool Recv(std::string *buffer, int size) = 0;
    virtual void Send(std::string &send_str) = 0;
    virtual Socket *get() = 0;

public:
    // 创建监听套接字
    void BuildListenSocketMethod(uint16_t port, int backlog = defaultbacklog)
    {
        CreateSocketOrDie();
        BindSocketOrDie(port);
        ListenSocketOrDie(backlog);
    }
    // 创建连接套接字
    bool BuildConnectSocketMethod(std::string &serverip, uint16_t serverport)
    {
        CreateSocketOrDie();
        return ConnectServer(serverip, serverport);
    }
    // 创建常规套接字
    void BuildNormalMethod(int socket)
    {
        SetSockfd(socket);
    }
};

class TcpSocket : public Socket
{
public:
    TcpSocket(int sockfd = defaultsockfd) : _sockfd(sockfd)
    {
    }
    virtual ~TcpSocket() {}
    void CreateSocketOrDie() override
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_sockfd < 0)
            exit(SocketError);
        SetNonBlock(_sockfd);
    }
    void BindSocketOrDie(uint16_t port) override
    {
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = htons(port);
        int n = ::bind(_sockfd, Conv(&local), sizeof local);
        if (n < 0)
            exit(BindError);
    }
    void ListenSocketOrDie(int backlog = defaultbacklog) override
    {
        int n = listen(_sockfd, backlog);
        if (n < 0)
            exit(ListenError);
    }
    Socket *AccetpConnecton(std::string *peerip, uint16_t *peerport) override
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int newsockfd = accept(_sockfd, Conv(&peer), &len);
        if (newsockfd < 0)
            return nullptr;
        *peerport = ntohs(peer.sin_port);
        *peerip = inet_ntoa(peer.sin_addr);

        Socket *s = new TcpSocket(newsockfd);
        return s;
    }
    bool ConnectServer(std::string &serverip, uint16_t serverport) override
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof server);
        server.sin_family = AF_INET;
        server.sin_port = htons(serverport);
        server.sin_addr.s_addr = inet_addr(serverip.c_str());
        int n = connect(_sockfd, Conv(&server), sizeof server);
        if (n == 0)
            return true;
        else
            return false;
    }
    int GetSockfd() override
    {
        return _sockfd;
    }
    void SetSockfd(int sockfd) override
    {
        _sockfd = sockfd;
    }

public:
    // 创建监听套接字
    void BuildListenSocket(uint16_t port, int backlog)
    {
        CreateSocketOrDie();
        BindSocketOrDie(port);
        ListenSocketOrDie(backlog);
    }
    // 创建连接套接字
    void BuildConnectSocket(std::string &serverip, uint16_t serverport)
    {
        CreateSocketOrDie();
        ConnectServer(serverip, serverport);
    }
    // 创建常规套接字
    void BuildNormal(int socket)
    {
        SetSockfd(socket);
    }

    void CloseSockfd() override
    {
        if (_sockfd > defaultsockfd)
            ::close(_sockfd);
    }

    Socket *get()
    {
        return this;
    }

    bool Recv(std::string *buffer, int size) override
    {
        char inbuffer[size];
        ssize_t n = recv(_sockfd, inbuffer, size - 1, 0);
        if (n > 0)
        {
            inbuffer[n] = 0;
            *buffer += inbuffer; // 故意拼接的
            return true;
        }
        else if (n == 0)
            return false;
        else
            return false;
    }
    void Send(std::string &send_str) override
    {
        // 多路转接我们在统一说
        send(_sockfd, send_str.c_str(), send_str.size(), 0);
    }

private:
    int _sockfd;
};