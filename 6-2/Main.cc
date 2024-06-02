#include <iostream>
#include <memory>
#include "SelectServer.hpp"

// ./select_server port
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " port\n" << std::endl;
        return 1;
    }
    
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<SelectServer> svr = std::make_unique<SelectServer>(port);

    svr->InitServer();
    svr->Loop();

    return 0;
}