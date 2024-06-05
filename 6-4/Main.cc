#include <iostream>
#include <string>
#include <memory>
#include "EpollServer.hpp"

void Usage(std::string process)
{
    std::cout << "Usage: " << process << " port" << std::endl;
}

// ./epoll_server 8888
int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }

    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<EpollServer> svr = std::make_unique<EpollServer>(port);
    svr->Init();
    svr->Loop();
    return 0;
}