#include "Protocol.hpp"
#include "TcpServer.hpp"
#include "Socket.hpp"
#include <iostream>
#include <memory>
#include "Calculate.hpp"

// 网络只负责IO发送
// HandlerRequest字节流数据解析和调用业务处理方法
// 处理报文的方法
std::string HandlerRequest(std::string &inbufferstream, bool *error_code)
{
    *error_code = true;
    // 0. 计算器对象
    Calculate calculate;
    // 1. 构建响应对象
    Factory *factory = new Factory();
    auto req = factory->BuildRequest();

    // 2. 分析字节流，看是否有一个合法的报文
    std::string message;
    std::string total_resp_string;
    while (Decode(inbufferstream, &message))
    { 
        // 3. 程序到这里，意味着一定读到了一个合法的报文，并且存放到了message当中
        // 进行反序列化
        if (!req->Deserialize(message))
        {
            *error_code = false;
            return "";
        }

        // 4. 业务处理
        auto resp = calculate.Cal(req);
        // 5. 序列化
        std::string send_string;
        resp->Serialize(&send_string);
        // 6. 构建完整的字符串级别的响应报文
        send_string = Encode(send_string);
        // 7. 发送给客户端
        total_resp_string += send_string;
    }

    return total_resp_string;
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
    std::unique_ptr<TcpServer> svr(new TcpServer(localport, HandlerRequest));
    svr->Loop();
    return 0;
}