#include <iostream>
#include <memory>
#include "Http.hpp"
#include <unistd.h>
#include <string>

std::string HandlerRequest(std::string &request)
{
    std::cout << request << std::endl;

    //std::string content = ""
    return "";
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
    

    std::unique_ptr<HttpServer> svr(new HttpServer(localport, HandlerRequest));
    svr->Loop();
    return 0;
}