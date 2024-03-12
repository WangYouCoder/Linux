#include<iostream>
#include<vector>
#include<string>
#include<cassert>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include"Task.hpp"
using namespace std;
const int N = 5;
static int num = 1;

class channel
{
public:
    channel(int fd, pid_t id):ctrlfd(fd),workerid(id)
    {
        name = "channel-" + to_string(num++);
    }
    
public:
    int ctrlfd;
    pid_t workerid;
    string name;
};

void Work()
{
    while(true)
    {
        int code = 0;
        ssize_t n = read(0, &code, sizeof(code));
        if(n == sizeof(code))
        {
            if(!init.CheckSafe(code)) 
            {
                continue;
            }
            //cout << "read code: " << code << endl;
            init.Run_Task(code);
        }
        else if(n == 0)
        {
            break;
        }
        else
        {

        }
    }

    cout << "child quit" << endl;
}

void CreatChannels(vector<channel> *c)
{
    for(int i = 0; i < N; i++)
    {
        //1.创建管道
        int pipefd[2];
        int n = pipe(pipefd);
        assert(n == 0);
        (void)n;

        //2.创建子进程
        pid_t id = fork();
        assert(id != -1);

        if(id == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0],0);
            Work();
            exit(0);
        }

        //father
        close(pipefd[0]);
        c -> push_back(channel(pipefd[1],id));
    }
}

void Print(const vector<channel>& c)
{
    for(const auto& channel : c)
    {
        cout << channel.name << ", " << channel.ctrlfd << ", " << channel.workerid << endl;
    }
}

const bool g_always_loop = true;

void Sendcommand(const vector<channel>& channels, bool g_always_loop = true, int count = 10)
{
    int pos = 0;
    while(true)
    {
        //1.选择一个任务
        int code = init.Creat_code();
        //cout << "code: " << code << endl;

        //2.选择信道
        const auto& c = channels[pos++];
        pos %= channels.size();

        //3.发送任务
        write(c.ctrlfd, &code, sizeof(code));

        //4.退出
        if(!g_always_loop)
        {
            count--;
            if(count <= 0)
                break;
        }

        //sleep(1);
    }
    std::cout << "SendCommend done..." << std::endl;
}

void ReleaseChannel(vector<channel>& channels)
{
    for(auto &c : channels)
    {
        close(c.ctrlfd);
    }

    for(auto &c : channels)
    {
        pid_t rid = waitpid(c.workerid, nullptr, 0);
        if(rid = c.workerid)
        {
            cout << "wait success: " << c.workerid << ": success" << endl;
        }

    }
}

int main()
{
    vector<channel> channels;

    //创建信道
    CreatChannels(&channels);

    //发送任务
    Sendcommand(channels, !g_always_loop, 10);

    //回收资源
    ReleaseChannel(channels);

    //Print(channels);
    //sleep(10);
    return 0;
}