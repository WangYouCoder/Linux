#include "Protocol.hpp"
#include "TcpServer.hpp"
#include "Socket.hpp"
#include <iostream>
#include <memory>

void HandlerRequest(WY::Socket *sockp)
{
    std::cout << "in HandlerRequest" << std::endl;
}

// ./server port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage : " << argv[0] << " port" << std::endl;
        return 1;
    }
    uint16_t localport = std::stoi(argv[1]);
    std::unique_ptr<TcpServer> svr(new TcpServer(localport, HandlerRequest));
    svr->Loop();
    return 0;
}