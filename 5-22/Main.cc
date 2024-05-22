#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include "TcpServer.hpp"
#include "HttpProtocol.hpp"

// 网络负责IO发送
// HandlerRequest字节流数据解析和调用业务处理方法的
// 处理报文的方法
// request: 我们认为我们已经读到了一个完整的请求了
std::string HandlerHttpRequest(std::string &request)
{
    HttpRequest req;
    // 反序列化
    req.Deserialize(request);
    req.DebugHttp();

    std::string content = "<html><h1>hello bite</h1></html>";
    std::string httpstatusline = "Http/1.0 200 OK\r\n";
    std::string httpheader = "Content-Length: " + std::to_string(content.size()) + "\r\n";
    httpheader += "\r\n";

    std::string httpresponse = httpstatusline + httpheader + content;
    return httpresponse;
}

// ./server port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage : " << argv[0] << " port" << std::endl;
        return 0;
    }
    uint16_t localport = std::stoi(argv[1]);

    std::unique_ptr<TcpServer> svr(new TcpServer(localport, HandlerHttpRequest));

    svr->Loop();

    return 0;
}