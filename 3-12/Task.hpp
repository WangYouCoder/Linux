#include<iostream>
#include<functional>
#include<time.h>
#include<vector>
typedef std::function<void()> tack_t;

void Download()
{
    std::cout << "我是一个下载任务" << std::endl;
}

void PrintLog()
{
    std::cout << "我是一个打印日志的任务" << std::endl;
}

void PushVideoStream()
{
    std::cout << "我是一个推送视频的任务" <<std::endl;
}


class Init
{
public:
    std::vector<tack_t> tasks;
    const static int download_code = 0;
    const static int printlog_code = 1;
    const static int pushvideostream = 2;

public:
    Init()
    {
        tasks.push_back(Download);
        tasks.push_back(PrintLog);
        tasks.push_back(PushVideoStream);

       srand(time(nullptr));
    }

    bool CheckSafe(int code)
    {
        if(code >= 0 && code < tasks.size()) return true;
        else return false;
    }

    void Run_Task(int code)
    {
        tasks[code]();
    }

    int Creat_code()
    {
        return rand() % tasks.size();
    }
};

Init init;
