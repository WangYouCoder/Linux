#include <iostream>
#include <memory>
#include <string>
#include "Tcpserver.hpp"
#include "Socket.hpp"
#include "Epoller.hpp"
#include "Accepter.hpp"
void Usage(std::string process)
{
    std::cout << "Usage: " << process << " port" << std::endl;
}

void recver(Connection* conn)
{
    std::cout << "get event, sockfd is: " << conn->Getsockfd() << std::endl;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }

    std::unique_ptr<TcpServer> svr = std::make_unique<TcpServer>();
    std::unique_ptr<Socket> listensock = std::make_unique<TcpSocket>();

    std::unique_ptr<Accepter> accepter = std::make_unique<Accepter>();
    auto listen_recver = std::bind(&Accepter::AcceptConnection, accepter.get(), std::placeholders::_1); //?

    uint16_t port = std::stoi(argv[1]);
    listensock->BuildListenSocketMethod(port); // 创建listen套接字
    svr->Init();

    // 将listen添加到epoll模型中
    svr->AddConnection(ConnectionFactory::BuildListenConnection(listensock->GetSockfd(), listen_recver, EPOLLIN | EPOLLET, svr.get()));
    // 启动服务
    svr->Dispatch();
    return 0;
}