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

    // 1.创建服务器
    std::unique_ptr<TcpServer> svr = std::make_unique<TcpServer>();
    // 2.创建套接字
    std::unique_ptr<Socket> listensock = std::make_unique<TcpSocket>();
    // 3. 创建连接
    std::unique_ptr<Accepter> accepter = std::make_unique<Accepter>();
    auto listen_recver = std::bind(&Accepter::AcceptConnection, accepter.get(), std::placeholders::_1); //?

    uint16_t port = std::stoi(argv[1]);
    // 创建listen套接字
    listensock->BuildListenSocketMethod(port); 
    svr->Init();

    // 将 listen文件描述符 添加到epoll模型中
    svr->AddConnection(ConnectionFactory::BuildListenConnection(listensock->GetSockfd(), listen_recver, EPOLLIN | EPOLLET, svr.get()));
    // 启动服务
    svr->Dispatch();
    return 0;
}

// 1. 先创建listen文件描述符和其对应的Connection对象，同时将listen套接字的accept连接函数进行bind与注册
//   (注册的recver的位置，为了就是后续与EPOLLIN对应，也可以说是在创建Connection时设置的关心事件就是EPOLLIN)
//   (对象中包括：自身的文件描述符、关心的事件、读写异常的回调函数、输入输出缓冲区、服务器的回调指针 -> 后面说这个回调指针的作用)

// 2. 因为accept函数也可能阻塞，所有需要将 listen文件描述符 添加到epoll模型中：

// 3. 启动服务器

// 4. 由epoll_wait系统调用将就绪的连接进行获取(此时只有listen在epoll里面)

// 5. (当创建listen对应的Connection对象时，将其关心的事件设置为了EPOLLIN) 
//    获取之后会判断该struct epoll_event(包括fd和event)关心的是什么事件(此时只有listen)，其实就是EPOLLIN

// 6. 然后会回调相应的recver函数，其实就是一开始注册的AcceptConnection函数

// 7. 在AcceptConnection函数中会进行accept连接，它会返回一个普通的sockfd，使用sockfd来进行数据通信

// 8. 自然sockfd也是需要添加到epoll当中，后续过程与listen一致
// 注意：epoll是不断读取数据的，所以需要将对应的文件描述符设置为非阻塞状态，listen是在创建的时候就设置了，sockfd需要自己手动设置
// 回调指针就是用在：listen对应的Connection对象需要将自己建立连接之后产生的sockfd添加到epoll当中，所以需要在Connection中添加一个回调指针，方便进行添加

// 9. 服务器开始循环
