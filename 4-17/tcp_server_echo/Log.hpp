#pragma once
#include <iostream>
#include <cstdarg>

enum
{
    Debug = 0,
    Info,
    Warning,
    Error,
    Fatal
};

std::string LevelToString(int level)
{
    switch (level)
    {
    case Debug:
        return "Debug";
    case Info:
        return "Info";
    case Warning:
        return "Warning";
    case Error:
        return "Error";
    case Fatal:
        return "Fatal";
    default:
        return "Unknown";
    }
}

class Log
{
public:
    Log()
    {}
    void LogMessage(int level, const char* format, ...)
    {
        char buffer[1024];
        va_list args;           // va_list  --> char* 或者 void*
        va_start(args, format); // args 指向了可变参数
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args); 

        uint64_t curtime = time(nullptr);  // 获取当前的时间戳
        printf("[%s][%s]\n", LevelToString(level).c_str(), std::to_string(curtime).c_str(), buffer);
    }   
    ~Log()
    {}
private:
};

Log lg;