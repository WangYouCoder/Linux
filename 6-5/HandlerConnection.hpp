
// 专门处理IO

#pragma once
#include <iostream>
#include "Connection.hpp"
#include "Protocol.hpp"
#include "Calculate.hpp"
class HandlerConnection
{
public:
    HandlerConnection()
    {
    }

    static void HandlerRequest(Connection *conn)
    {
        std::string &inbuffer = conn->GetInbuffer();
        std::string message; // 符合协议的一个完整报文
        Calculate calulate;  // 负责业务处理
        Factory factory;
        auto req = factory.BuildRequest();
        // 1. 明确报文边界，解决粘包问题
        while (Decode(inbuffer, &message))
        {
            // 2. 反序列化
            if (!req->Deserialize(message))
                continue;
            // 3. 业务处理
            auto res = calulate.Cal(req);
            // 4. 对响应进行序列化
            std::string response;
            res->Serialize(&response);
            // 5. 封装完整报文
            response = Encode(response);
            // 6. 将应答全部追加到outbuffer中
            conn->AddOutbuffer(response);
        }
        // 考虑发送问题
        if (conn->IsNeedSend())
        {
            conn->_sender(conn); // 直接发，不代表着能全部发完  eg:数据有4096字节，输出缓冲区只有1024
        }
    }

    // 在这里读的时候，我们不关心数据的格式是什么，也不关心协议是什么样的
    // 我们只负责把本轮数据完全读取完毕 --- 把读到的字节流数据，交给上层 --- 由上层处理
    static void Recver(Connection *conn)
    {
        char buffer[1024];
        while (true)
        {
            ssize_t n = recv(conn->Getsockfd(), buffer, sizeof(buffer) - 1, 0);
            if (n > 0)
            {
                buffer[0];
                conn->AddInbuffer(buffer);
            }
            else
            {
                if (errno == EAGAIN)
                    break; // 读取完毕
                else if (errno == EINTR)
                    continue; // 信号中断
                else
                {
                    // 真正的读取错误
                    conn->_excepter(conn);
                    return;
                }
            }
        }
        // 解析处理报文
        HandlerRequest(conn);
    }
    static void Sender(Connection *conn)
    {
        std::string &outbuffer = conn->GetOutbuffer();
        while (true)
        {
            ssize_t n = send(conn->Getsockfd(), outbuffer.c_str(), outbuffer.size(), 0);
            if (n >= 0)
            {
                outbuffer.erase(0, n); // 将已经发送的数据从输出缓冲区中移除
                if (outbuffer.empty())
                    break;
            }
            else
            {
                if (errno == EAGAIN)
                    break; // 缓冲区可能已经发满了
                else if (errno == EINTR)
                    continue;
                else
                {
                    conn->_excepter(conn);
                    return;
                }
            }
        }

        // 走到这里，意味着我们本轮发完了，但是数据可能没发完！！！  eg:数据有4096字节，输出缓冲区只有1024
        // (send实际上只是一个拷贝函数，将数据用用户层拷贝给OS，也就是OS的输出缓冲区已经被写满了，但是我们自己维护的输出缓冲区(outbuffer)中还有数据)
        //  我们只调用了一次send噢！！！
        // 开启对EPOLLOUT的关心
        // 当OS的写入缓冲区有空间后，继续调用send

        if (conn->IsNeedSend())
        {
            conn->_R->EnableReadWrite(conn->Getsockfd(), true, true);
        }
        else
        {
            conn->_R->EnableReadWrite(conn->Getsockfd(), true, false);
        }
    }
    static void Excepter(Connection *conn)
    {
        lg.LogMessage(Info, "connection erase done, who: %d\n", conn->Getsockfd());
        // 从移除epoll
        // 从unordered_map中移除conn
        conn->_R->RemoveConnection(conn->Getsockfd());
        // 关闭conn->sockfd
        conn->Close();
        // delete conn
        delete conn;
    }

    ~HandlerConnection()
    {
    }
};