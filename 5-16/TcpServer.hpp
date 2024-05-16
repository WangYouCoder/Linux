#include "Socket.hpp"

class TcpServer
{
public:
    TcpServer(uint16_t port) : _port(port), _listensock(new TcpSocket())
    {
        _listensock->BuildListenSocketMethod(port);
    }
    void Loop()
    {
        while(true)
        {
            std::string peerip;
            uint16_t peerport;
            Socket* newsock = _listensock->AccetpConnecton(&peerip, &peerport);
            if(newsock == nullptr) continue;
            std::cout << "获得一个新连接, sockfd: " << newsock->GetSockfd() << " client info : " << peerip << ":" << peerport << std::endl;
            newsock->CloseSockfd();
        }
    }

    ~TcpServer()
    {
        delete _listensock;
    }
private:
    uint16_t _port;
    Socket* _listensock;
};