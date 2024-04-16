#include "UdpServer.hpp"
#include "Comm.hpp"
#include<memory>

void Usage(std::string porc)
{
    std::cout << "Usage : \n\t" << porc << " local_ip local_port\n" << std::endl;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        return Usage_Err;
    }

    std::string ip = argv[1];
    uint16_t port = std::stoi(argv[2]);

    //c++14
    //std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(ip, port);
    UdpServer* usvr = new UdpServer(ip, port);
    usvr->Init(); 
    usvr->Start();
    return 0;
}