#pragma once
#include <iostream>
#include <memory>
#include <jsoncpp/json/json.h>

#define SelfDefine 1

const std::string ProtSep = " ";
const std::string LineBreakSep = "\n";


// 1. 解决结构化数据的序列和反序列化的问题
// 2. 还要解决用户区分报文边界 --- 数据包粘报问题

std::string Encode(std::string &message)
{
    std::string len = std::to_string(message.size());
    std::string package = len + LineBreakSep + message + LineBreakSep;
    return package;
}

bool Decode(std::string &package, std::string *message)
{
    // std::cout << "----------------" << std::endl;

    // pos 是"\n"的位置
    auto pos = package.find(LineBreakSep);
    if(pos == std::string::npos) return false;
    std::string lens = package.substr(0, pos);
    // std::cout << "lens: " << lens << std::endl;
    int messagelen = std::stoi(lens);
    // std::cout << "messagelen: " << messagelen << std::endl;


    //             "len"         "x op y"          两个"\n"
    int total = lens.size() + messagelen + 2 * LineBreakSep.size();
    if(package.size() < total) return false;
    // package中至少有一个合法的报文
    *message = package.substr(pos + LineBreakSep.size(), messagelen);

    // 将我们处理过的报文去除掉
    package.erase(0, total);
    return true;

    // 可以在外部使用while循环不断取出合法的报文，一直到返回错误为止
}

class Request
{
public:
    Request() :_data_x(0), _data_y(0),_oper(0)
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

    // 序列化：结构化数据 -> 字符串
    bool Serialize(std::string *out)
    {
#ifdef SelfDefine
        *out = std::to_string(_data_x) + ProtSep + _oper + ProtSep + std::to_string(_data_y);
        return true;
#else
        Json::Value root;
        root["datax"] = _data_x;
        root["datay"] = _data_y;
        root["oper"] = _oper;
        Json::FastWriter writer;
        *out = writer.write(root);
        return true;
#endif
    }

    // 反序列化：字符串 -> 结构化数据
    bool Deserialize(std::string& in)
    {
#ifdef SelfDefine
        auto left = in.find(ProtSep);
        if(left == std::string::npos) 
            return false;
        auto right = in.rfind(ProtSep);
        if(right == std::string::npos) 
            return false; 

        _data_x = std::stoi(in.substr(0, left));
        _data_y = std::stoi(in.substr(right + ProtSep.size()));
        std::string oper = in.substr(left + ProtSep.size(), right - (left + ProtSep.size()));
        if(oper.size() != 1) 
            return false;
        _oper = oper[0];
        return true;
#else
    Json::Value root;
    Json::Reader reader;
    bool res = reader.parse(in, root);
    if(res) 
    {
        _data_x = root["datax"].asInt();
        _data_y = root["datay"].asInt();
        _oper = root["oper"].asInt();
    }
    return res;
#endif
    }

    // char GetOper
    // {
    //     return _oper;
    // }
    int GetY()  { return _data_y; }
    int GetX()  { return _data_x; }
    char GetOper()   {return _oper; }

private:
    int _data_x; // 第一个数据
    int _data_y; // 第二个数据
    char _oper;  // 操作符 + - * / 
};

class Response
{
public:
    Response() :_result(0), _code(0)
    {}
    Response(int result, int code) :_result(result), _code(code)
    {}

    // 序列化：结构化数据 -> 字符串
    bool Serialize(std::string *out)
    {
#ifdef SelfDefine
        *out = std::to_string(_result) + ProtSep + std::to_string(_code);
        return true;
#else
        Json::Value root;
        root["result"] = _result;
        root["code"] = _code;
        Json::FastWriter writer;
        *out = writer.write(root);
        return true;
#endif
    }

    // 反序列化：字符串 -> 结构化数据
    bool Deserialize(std::string& in)  // "_result _code"
    {
#ifdef SelfDefine
        auto pos = in.find(ProtSep);
        if(pos == std::string::npos) return false;
        _result = std::stoi(in.substr(0, pos));
        _code = std::stoi(in.substr(pos + ProtSep.size()));
        return true;
#else
        Json::Value root;
        Json::Reader reader;
        bool res = reader.parse(in, root);
        if(res) 
        {
           _result = root["result"].asInt();
           _code = root["code"].asInt();
        }
      return res;
#endif
    }

    void SetResult(int res) {_result = res;}
    void SetCode(int code) {_code = code;}
    int GetResult() { return _result; }
    int GetCode() { return _code; }

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