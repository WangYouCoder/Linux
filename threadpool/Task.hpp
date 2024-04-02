#pragma once
#include <iostream>
#include <string>
#include <unistd.h>

const int defaultvalue = 0;

enum
{
    ok = 0,
    div_zero,
    mod_zero,
    unknow
};

const std::string opers = "+-*/%";

class Task
{
public:
    Task()
    {
    }
    Task(int x, int y, char op)
        : data_x(x), data_y(y), oper(op), result(defaultvalue), code(ok)
    {
    }
    void Run()
    {
        switch (oper)
        {
        case '+':
            result = data_x + data_y;
            break;
        case '-':
            result = data_x - data_y;
            break;
        case '*':
            result = data_x * data_y;
            break;
        case '/':
        {
            if (data_y == 0)
                code = div_zero;
            else
                result = data_x / data_y;
        }
        break;
        case '%':
        {
            if (data_y == 0)
                code = mod_zero;
            else
                result = data_x % data_y;
        }

        break;
        default:
            code = unknow;
            break;
        }
    }
    void operator()()
    {
        Run();
    }
    std::string PrintTask()
    {
        std::string s;
        s = std::to_string(data_x);
        s += oper;
        s += std::to_string(data_y);
        s += "=?";

        return s;
    }
    std::string PrintResult()
    {
        std::string s;
        s = std::to_string(data_x);
        s += oper;
        s += std::to_string(data_y);
        s += "=";
        s += std::to_string(result);
        s += " [";
        s += std::to_string(code);
        s += "]";

        return s;
    }
    ~Task()
    {
    }

private:
    int data_x;
    int data_y;
    char oper; // + - * / %

    int result;
    int code; // 结果码，0: 结果可信 !0: 结果不可信，1,2,3,4
};