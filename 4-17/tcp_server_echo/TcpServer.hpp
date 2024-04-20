#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include "Log.hpp"
#include "nocopy.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "Comm.hpp"
#include <unistd.h>
#include <sys/wait.h>

const static int default_backlog = 5;

class TcpServer : public nocopy
{
public:
    TcpServer(uint16_t port) : _port(port), _isruning(false)
    {
    }
    void Init()
    {
        Log lg;
        // 1. 创建套接字， socket, file fd, 本质是文件
        _listensock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listensock < 0)
        {
            lg.LogMessage(Fatal, "create socket errno, errno code: %d, errno string: %s\n", errno, strerror(errno));
            exit(Fatal);
        }
        lg.LogMessage(Debug, "create socket success, socket: %d, errno code: %d, errno string: %s\n", _listensock, errno, strerror(errno));
        
        // 解决一些少量的bind失败的问题
        int opt = 1;
        setsockopt(_listensock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));


        // 2. 填充本地网络信息并bind
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;    // 当前还没有设置到内核中，目前还在栈上

        // 2.1 bind
        if(bind(_listensock, CONV(&local), sizeof(local)) != 0) 
        {
            lg.LogMessage(Fatal, "bind socket errno, errno code: %d, errno string: %s\n", errno, strerror(errno));
            //exit(Bind_Err);
            exit(3);
        } 
        lg.LogMessage(Debug, "bind socket success, socket: %d\n", _listensock);


        // 3. 设置socket为监听状态，tcp特有的
        if(listen(_listensock, default_backlog) != 0)
        {
            lg.LogMessage(Fatal, "listen socket errno, errno code: %d, errno string: %s\n", errno, strerror(errno));
            //exit(Listen_Err)
            exit(4);
        }
        lg.LogMessage(Debug, "listen socket success, socket: %d\n", _listensock);
    }
    void Service(int sockfd)
    {
        char buffer[1024];
        while(true)
        {
            ssize_t n = read(sockfd, buffer, sizeof(buffer) - 1);
            if(n > 0)
            {
                buffer[n] = 0;
                std::cout << "client say# " << buffer << std::endl;

                std::string echo_string = "server echo# ";
                echo_string += buffer;
                write(sockfd, echo_string.c_str(), echo_string.size());
            }
            else if(n == 0) // n == 0,表示读到文件结尾，读端关闭连接
            {
                lg.LogMessage(Info, "client quit...\n");
                break;
            }
            else
            {
                lg.LogMessage(Error, "read socket error, errno code: %d, error string: %s\n", errno, strerror(errno));
            }
        }
    }
    void Start()
    {
        _isruning = true;
        while(_isruning)
        {
            // 4. 获取链接   int accept(int socket, struct sockaddr* addr, socklen_t* addrlen);  返回值会返回一个新的文件描述符
            //                                             输入输出型参数，主要是体现在输出，addr和addrlen会将与谁链接的数据带回来
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sockfd = accept(_listensock, CONV(&peer), &len);
            if(sockfd < 0)
            {
                lg.LogMessage(Warning, "accept socket error, errno code: %d, error string: %s\n", errno, strerror(errno));
                continue;
            }
            // 获取链接成功
            lg.LogMessage(Debug, "accept socket success, get a new sockfd: %d\n", sockfd);

            // // 5. 提供服务  v1
            // Service(sockfd);
            // close(sockfd);

            // v2 多进程
            // pid_t id = fork();
            // if(id < 0)
            // {
            //     close(sockfd);
            //     continue;
            // }
            // else if(id == 0)
            // {
            //     // child
            //     close(_listensock);
            //     if(fork() > 0)
            //         exit(0);
                
            //     // 孤儿进程
            //     Service(sockfd);
            //     close(sockfd);
            //     exit(0);
            // }
            // else
            // {
            //     close(sockfd);
            //     pid_t rid = waitpid(id, nullptr, 0);
            //     if(rid == id)
            //     {

            //     }
            //     else
            // }

            // v3 基于信号量
            // signal(SIGCHLD, SIG_IGN); // 在Linux环境中，如果对SIG_IGN进行忽略，子进程退出的时候，会自动的释放自己的资源，不会产生僵尸进程
            // pid_t id = fork();
            // if(id < 0)
            // {
            //     close(sockfd);
            //     continue;
            // }
            // else if(id == 0)
            // {
            //     // child
            //     close(_listensock);
            //     Service(sockfd);
            //     close(sockfd);
            //     exit(0);
            // }
            // else
            // {
            //     close(sockfd);
            //     pid_t rid = waitpid(id, nullptr, 0);
            //     if(rid == id)
            //     {

            //     }
            //     else
            // }
            
        }
    }
    ~TcpServer() {}

private:
    uint16_t _port;
    int _listensock;
    bool _isruning;
};
