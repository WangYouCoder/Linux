#pragma once
#include <iostream>



class Request
{
private:
    // 序列化格式： _data_x _oper _data_y
    int _data_X;
    int _data_y;
    char _oper;
};

class Response
{
private:
    int _result;
    int _code;  // 运行状态
};


