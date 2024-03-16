#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
using namespace std;

void handle(int signal)
{
    cout << "获得2号信号" << endl;
    exit(1);
}

int main()
{
    signal(2,handle);

    while(true)
    {
        cout << "runing ..." << "pid: " << getpid() << endl;
        sleep(1);
    }
    return 0;
}