#include "Socket.hpp"
#include "Protocol.hpp"
#include <unistd.h>
//  ./tcpclient serverip serverport
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "Usage : " << argv[0] << " serverip serverport" << std::endl;
        return 0;
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);
    Socket* connectfd = new TcpSocket();

    if(!connectfd->BuildConnectSocketMethod(serverip, serverport))
    {
        std::cerr << "connect " << serverip << ":" << serverport << " failed" << std::endl;
        return 0;
    }    
    std::cerr << "connect " << serverip << ":" << serverport << " success" << std::endl;
    sleep(10);

    return 0;
}