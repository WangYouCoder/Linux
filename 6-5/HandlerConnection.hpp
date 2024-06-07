
// 专门处理IO

#pragma once
#include <iostream>
#include "Connection.hpp"

class HandlerConnection
{
public:
    HandlerConnection()
    {
    }

    static void recver(Connection *conn)
    {
        std::cout << "recver" << std::endl;
    }
    static void sender(Connection *conn)
    {
        std::cout << "sender" << std::endl;
    }
    static void excepter(Connection *conn)
    {
        std::cout << "excepter" << std::endl;
    }

    ~HandlerConnection()
    {
    }
};