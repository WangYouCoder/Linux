#include "Socket.hpp"
#include "TcpServer.hpp"
#include <iostream>
#include <memory>
//  ./tcpserver port
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage : " << argv[0] << " port" << std::endl;
        return 0;
    }
    uint16_t port = std::stoi(argv[1]);
    Socket* listensock = new TcpSocket();
    listensock->BuildListenSocketMethod(port);

    std::unique_ptr<TcpServer> svr(new TcpServer(port));
    svr->Loop();

    return 0;
}