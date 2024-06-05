#include <iostream>
#include <memory>
#include <string>
#include "Tcpserver.hpp"
#include "Socket.hpp"
void Usage(std::string process)
{
    std::cout << "Usage: " << process << " port" << std::endl;
}

void recver(Connection* conn)
{
    
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
    uint16_t port = std::stoi(argv[1]);
    listensock->BuildListenSocketMethod(port);
    svr->Init();

    svr->AddConnection(ConnectionFactory().BuildListenConnection(listensock->GetSockfd(), recver));
    svr->Dispatch();
    return 0;
}