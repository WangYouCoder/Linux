#include "Socket.hpp"
#include <pthread.h>
#include <functional>

// 不管什么任务肯定都需要从网络中读取数据进行处理，所以参数需要一个Socket
using func_t = std::function<void(Socket* sockp)>;

class TcpServer;

class ThreadData
{
public:
    ThreadData(TcpServer* tcp_this, Socket* sockp) : _this(tcp_this), _sockp(sockp)
    {}
public:
    TcpServer* _this;
    Socket* _sockp;
};

class TcpServer
{
public:
    TcpServer(uint16_t port, func_t handler_request) : _port(port), _listensock(new TcpSocket()), _handler_request(handler_request)
    {
        _listensock->BuildListenSocketMethod(port);
    }

    static void* ThreadRun(void* args)
    {
        pthread_detach(pthread_self()); // 不去进行join阻塞
        ThreadData* td = static_cast<ThreadData*>(args); 
        td->_this->_handler_request(td->_sockp);
        td->_sockp->CloseSockfd();
        delete td->_sockp;
        delete td;
        return nullptr;
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
            
            pthread_t pid;
            ThreadData* td = new ThreadData(this, newsock);
            pthread_create(&pid, nullptr, ThreadRun, td);
            
            
            //newsock->CloseSockfd();
        }
    }

    ~TcpServer()
    {
        delete _listensock;
    }
private:
    uint16_t _port;
    Socket* _listensock;
public:
    func_t _handler_request; 
};