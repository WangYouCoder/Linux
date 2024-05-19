#include "Socket.hpp"
#include "TcpServer.hpp"
#include <iostream>
#include <memory>
#include "Protocol.hpp"
void HandlerRequest(Socket *sockp)
{
    while (true)
    {
        Request req;
        recv(sockp->GetSockfd(), &req, sizeof req, 0);
        req.Debug();
    }
}

//  ./tcpserver port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage : " << argv[0] << " port" << std::endl;
        return 0;
    }
    uint16_t port = std::stoi(argv[1]);

    std::unique_ptr<TcpServer> svr(new TcpServer(port, HandlerRequest));
    // TcpServer* svr = new TcpServer(port, HandlerRequest);
    svr->Loop();
    return 0;
}