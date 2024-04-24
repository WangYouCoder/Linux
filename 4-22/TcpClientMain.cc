#include "Protocol.hpp"
#include "TcpClient.hpp"
#include "Socket.hpp"
#include <ctime>
#include <stdlib.h>
#include <unistd.h>

// ./tcpclient serverip serverport
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage : " << argv[0] << " serverip serverport " << std::endl;
        return 1;
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::atoi(argv[2]);

    WY::Socket *conn = new WY::TcpSocket();
    if (!conn->BuildConnectSocket(serverip, serverport))
    {
        std::cerr << "connect " << serverip << ":" << serverport << "failed" << std::endl;
    }

    std::cout << "connect " << serverip << ":" << serverport << " success" << std::endl;
    std::string message = "hello world";
    // write(s->GetSockfd(), message.c_str(), message.size());

    // 当前centos不支持c++14,所以不支持make_unique...........
    // std::unique_ptr<Factory> factory = std::make_unique<Factory>();
    std::unique_ptr<Factory> factory(new Factory());

    srand((unsigned int)time(nullptr) ^ getpid());
    const std::string opers = "+-*/%^=";
    while (true)
    {
        // send(s->GetSockfd(), &(*req), sizeof(*req), 0);
        // sleep(1);

        // 1. 构建了一个请求
        int x = rand() % 100; // [0,99]
        usleep(rand() % 8888);
        int y = rand() & 100;
        char oper = opers[rand() % opers.size()];
        std::shared_ptr<Request> req = factory->BuildRequest(x, y, oper);

        // 2. 对请求进行序列化
        std::string requeststr;
        req->Serialize(&requeststr);
        // for test
        std::string testreq = requeststr;
        testreq += " ";
        testreq += "=";

        // 3. 添加自描述报头
        requeststr = Encode(requeststr);
        // 4. 发送请求
        conn->Send(requeststr);
        std::string responsestr;
        while (true)
        {
            // 5. 读取响应
            conn->Recv(&responsestr, 1024);

            // 6. 对报文进行解析
            std::string response;
            if(!Decode(responsestr, &response)) continue;

            // 7. response -> "_result _code" 反序列化
            auto resp = factory->BuildResponse();
            resp->Deserialize(response);            

            // 8. 得到计算结果，而且是一个结构化的数据
            std::cout << testreq << resp->GetResult() << " [" << resp->GetCode() << "]" << std::endl;
            break;
        }
        sleep(1);
    }

    conn->CloseSocket();
    // sleep(10);
    return 0;
}