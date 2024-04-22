#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#define Conv(addptr) ((struct sockaddr *)addptr)

namespace WY
{

    const static int defaultsockfd = -1;
    const int backlog = 5;

    enum
    {
        SocketError = 1,
        BindError,
        ListenError,
    };

    // 封装一个基类，Socket接口类
    // 模板方法设计模式
    class Socket
    {
    public:
        virtual ~Socket() {}
        virtual void CreateSocketOrDie() = 0;
        virtual void BindSocket(uint16_t port) = 0;
        virtual void ListenSocketOrDie(int backlog) = 0;
        virtual Socket *AcceptConnection(std::string *peerip, uint16_t *peerport) = 0;
        virtual bool ConnectServer(std::string &serverip, uint16_t serverport) = 0;
        virtual int GetSockfd() = 0;
        virtual void SetSockfd(int sockfd) = 0;
        virtual void CloseSocket() = 0;

    public:
        // server 服务端
        void BindListenSocket(uint16_t port, int backlog)
        {
            CreateSocketOrDie();        // 创建套接字
            BindSocket(port);           // 绑定
            ListenSocketOrDie(backlog); // 分配一个文件描述符
        }
        // client 客户端
        bool BuildConnectSocket(std::string &serverip, uint16_t serverport)
        {
            CreateSocketOrDie();                        // 创建套接字
            return ConnectServer(serverip, serverport); // 与服务端进行连接
        }

        void BuildNormalSocket(int sockfd)
        {
            SetSockfd(sockfd);
        }
    };

    class TcpSocket : public Socket
    {
    public:
        TcpSocket(int sockfd = defaultsockfd) : _sockfd(sockfd)
        {
        }
        ~TcpSocket()
        {
        }

        void CreateSocketOrDie() override
        {
            _sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
                exit(SocketError);
        }

        void BindSocket(uint16_t port) override
        {
            struct sockaddr_in local;
            memset(&local, 0, sizeof local);
            local.sin_family = AF_INET;
            local.sin_addr.s_addr = INADDR_ANY;
            local.sin_port = htons(port);

            int n = bind(_sockfd, Conv(&local), sizeof(local));
            if (n < 0)
                exit(BindError);
        }

        void ListenSocketOrDie(int backlog) override
        {
            int n = listen(_sockfd, backlog);
            if (n < 0)
                exit(ListenError);
        }

        Socket *AcceptConnection(std::string *peerip, uint16_t *peerport) override
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
            server.sin_addr.s_addr = inet_addr(serverip.c_str());
            server.sin_port = htons(serverport);

            int n = connect(_sockfd, Conv(&server), sizeof(server));
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

        void CloseSocket() override
        {
            if (_sockfd > defaultsockfd)
                close(_sockfd);
        }

    private:
        int _sockfd;
    };

}