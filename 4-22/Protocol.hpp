#pragma once
#include <iostream>
#include <memory>

class Request
{
public:
    Request()
    {}
    Request(int x, int y, char op) :_data_x(x), _data_y(y), _oper(op)
    {}
    void Debug()
    {
        std::cout << "_data_x: " << _data_x << std::endl;
        std::cout << "_data_y: " << _data_y << std::endl;
        std::cout << "_oper: " << _oper << std::endl;
    }
    void Inc()
    {
        _data_x++;
        _data_y++;
    }
private:
    int _data_x; // 第一个数据
    int _data_y; // 第二个数据
    char _oper;  // 操作符 + - * / 
};

class Response
{
public:
    Response()
    {}
    Response(int result, int code) :_result(result), _code(code)
    {}
private:
    int _result; // 运算结果
    int _code;   // 运算状态
};

class Factory
{
public:
    std::shared_ptr<Request> BuildRequest()
    {
        std::shared_ptr<Request> req = std::make_shared<Request>();
        return req;
    }

    std::shared_ptr<Request> BuildRequest(int x, int y, char op)
    {
        std::shared_ptr<Request> req = std::make_shared<Request>(x, y, op);
        return req;
    }

    std::shared_ptr<Response> BuildResponse()
    {
        std::shared_ptr<Response> resp = std::make_shared<Response>();
        return resp;
    }

    std::shared_ptr<Response> BuildResponse(int result, int code)
    {
        std::shared_ptr<Response> resp = std::make_shared<Response>(result, code);
        return resp;
    }
};