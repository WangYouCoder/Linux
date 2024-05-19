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

    // std::string message = "hello world";
    // write(connectfd->GetSockfd(), message.c_str(), message.size());

    std::unique_ptr<Factory> factory(new Factory());
    std::shared_ptr<Request> req = factory->BuildRequest(10, 20, '+');

    while(true)
    {
        req->Inc();
        send(connectfd->GetSockfd(), &(*req), sizeof(*req), 0);
        sleep(1);
    }

    connectfd->CloseSockfd();
    return 0;
}