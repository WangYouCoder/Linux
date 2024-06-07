#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cerrno>
#include "Connection.hpp"
#include "Log.hpp"
#include "HandlerConnection.hpp"
class Accepter
{
public:
    Accepter()
    {
    }
    void AcceptConnection(Connection *conn)
    {
        while (true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sockfd = ::accept(conn->Getsockfd(), (struct sockaddr *)&peer, &len);
            if (sockfd > 0)
            {
                lg.LogMessage(Info, "get a new link, sockfd is: %d\n", sockfd);
                SetNonBlock(sockfd);
                auto recver = std::bind(&HandlerConnection::recver, std::placeholders::_1);
                auto sender = std::bind(&HandlerConnection::sender, std::placeholders::_1);
                auto excepter = std::bind(&HandlerConnection::excepter, std::placeholders::_1);

                Connection *normal_conn = ConnectionFactory::BuildNormalConnection(sockfd, recver, sender, excepter, EPOLLIN | EPOLLET, conn->_R);
                conn->_R->AddConnection(normal_conn);
            }
            else
            {
                if (errno == EAGAIN)
                    break; // 底层数据读完
                else if (errno == EINTR)
                    continue; // 信号中断
                else
                {
                    lg.LogMessage(Warning, "get a new link error\n");
                    break;
                }
            }
        }
    }
    ~Accepter()
    {
    }
};