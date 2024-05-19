#pragma once
#include <iostream>
#include <memory>


class Request
{
public:
    Request(int x, int y, char oper) :_data_x(x), _data_y (y), _oper(oper)
    {}
    Request()
    {}

    void Inc()
    {
        _data_x++;
        _data_y++;
    }

    void Debug()
    {
        std::cout << "_data_x: " << _data_x << std::endl;
        std::cout << "_data_y: " << _data_y << std::endl;
        std::cout << "_oper: " << _oper << std::endl;
    }


private:
    // 序列化格式： _data_x _oper _data_y
    int _data_x;
    int _data_y;
    char _oper;
};

class Response
{
public:
    Response()
    {}
    Response(int result, int code) : _result(result), _code(code)
    {}
private:
    int _result;
    int _code;  // 运行状态
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


