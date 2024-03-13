#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<cerrno>
#include<cstring>
#include<fcntl.h>
#include<unistd.h>
#include"comm.h"
using namespace std;

bool Makefifo()
{
     //1.创建一个管道文件
    int n = mkfifo(FILENAME,0666);
    if(n < 0)
    {
        cerr << "errno: " << errno << ", errstring: " << strerror(errno) << endl;
        return false;
    }
    cout << "mkfifo success..." << endl;
    return true;
}

int main()
{
Start:
    //2.以读的方式打开管道文件
    int rfd = open(FILENAME, O_RDONLY);
    if(rfd < 0)
    {
        cerr << "errno: " << errno << ", errstring: " << strerror(errno) << endl;
        if(Makefifo()) goto Start;
        else return 1;
    }
    cout << "open fifo success..." << endl;

    //3.读取管道文件中的数据
    char buffer[1024];
    while(true)
    {
        ssize_t s = read(rfd, buffer, sizeof(buffer) - 1);
        if(s > 0)
        {
            buffer[s] = 0;
            cout << "Client say: " << buffer << endl;
        }
        else if(s == 0)
        {
            cout << "client quit, server quit too!" << endl;
            break;
        }
    }
    cout << "read fifo success..." << endl;

    //4.关闭管道文件
    close(rfd);
    cout << "close success...";
    return 0;
}