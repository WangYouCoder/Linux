#pragma once
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
enum
{
    Debug = 0,
    Info,
    Warning,
    Error,
    Fatal
};

enum
{
    Screen = 10,
    Onefile,
    Classfile
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

const int default_stype = Screen;
const std::string default_filename = "log.";
const std::string logdir = "logdir";
class Log
{
public:
    Log() : stype(default_stype),filename(default_filename)
    {
        mkdir(logdir.c_str(), 0775);
    }
    void Enable(int sty)
    {
        stype = sty;
    }
    std::string TimestampExLocalTime()
    {
        time_t currtime = time(nullptr); // 获取当前的时间戳
        struct tm* cur = localtime(&currtime);
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "%d-%d-%d %d:%d:%d",
            cur->tm_year+1900, cur->tm_mon+1, cur->tm_mday,
            cur->tm_hour, cur->tm_min, cur->tm_sec);
        return buffer;
    }
    void WriteLogToOnefile(const std::string& logname, const std::string& message)
    {
        umask(0);
        int fd = open(logname.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0666);
        if(fd < 0) return;
        write(fd, message.c_str(), message.size());
        close(fd);
        // std::ofstream out(logname);
        // if(!out.is_open()) return;
        // out.write(message.c_str(), message.size());
        // out.close();
    }
    void WriteLogToClassfile(const std::string& levelstr, const std::string& message)
    {
        std::string logname = logdir;
        logname += "/";
        logname += filename;
        logname += levelstr;
        WriteLogToOnefile(logname, message);
    }

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    void Write(const std::string& levelstr, const std::string& message)
    {
        pthread_mutex_lock(&lock);
        switch(stype)
        {
            case Screen:
                std::cout << message;
                break;
            case Onefile:
                WriteLogToClassfile("all", message);
                break;
            case Classfile:
                WriteLogToClassfile(levelstr, message);
                break;
            default:
                break;
        }
        pthread_mutex_unlock(&lock);
    }
    void LogMessage(int level, const char* format, ...)
    {
        char rightbuffer[1024];
        va_list args;           // va_list  --> char* 或者 void*
        va_start(args, format); // args 指向了可变参数
        vsnprintf(rightbuffer, sizeof(rightbuffer), format, args);
        va_end(args); 

        char leftbuffer[1024];
        std::string levelstr = LevelToString(level);
        std::string curtime = TimestampExLocalTime(); 
        std::string idstr = std::to_string(getpid());
        snprintf(leftbuffer, sizeof(leftbuffer), "[%s][%s][%s] ", levelstr.c_str(), curtime.c_str(), idstr.c_str());

        std::string Info = leftbuffer;
        Info += rightbuffer;
        Write(levelstr, Info);
    }   
    ~Log()
    {}
private:
    int stype;
    std::string filename;
};

Log lg;

class Conf
{
public:
    Conf()
    {
        lg.Enable(Screen);
    }
    ~Conf()
    {}
};

Conf conf;