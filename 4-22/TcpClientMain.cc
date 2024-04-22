#include "Protocol.hpp"
#include "TcpClient.hpp"
#include "Socket.hpp"

// ./tcpclient serverip serverport
int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "Usage : " << argv[0] << " serverip serverport " << std::endl;
        return 1;   
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::atoi(argv[2]);

    WY::Socket *s = new WY::TcpSocket();
    if(!s->BuildConnectSocket(serverip, serverport))
    {
        std:: cerr << "connect " << serverip << ":" << serverport << "failed" << std::endl;
    }

    std:: cout << "connect " << serverip << ":" << serverport << " success" << std::endl;
    std::string  message = "hello world";
    //write(s->GetSockfd(), message.c_str(), message.size());
    
    //std::unique_ptr<Factory> factory = std::make_unique<Factory>();
    std::unique_ptr<Factory> factory(new Factory());
    std::shared_ptr<Request> req = factory->BuildRequest(10, 20, '+');

    s->CloseSocket();
    //sleep(10);
    return 0;
}