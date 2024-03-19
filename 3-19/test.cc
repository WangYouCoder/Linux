#include<iostream>
#include<unistd.h>
#include<signal.h>
using namespace std;

void PrintPending(sigset_t pending)
{
    for(int i = 31; i > 0; i--)
    {
        if(sigismember(&pending,i))
        {
            cout << "1";
        }
        else
        {
            cout << "0";
        }
    }
    cout << endl;
}

void handler(int signal)
{
    cout << "获得" << signal  <<  "号信号" << endl;
    while(true)
    {
        sigset_t pending;
        sigpending(&pending);
        PrintPending(pending);
        sleep(1);
    }
}

int main()
{
    cout << "gitpid: " << getpid() << endl;
    struct sigaction act, oact;
    act.sa_handler = handler; 

    //将3号信号通过mask，添加到屏蔽信号中
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, 3);

    sigaction(2, &act, &oact);

    while(true) sleep(1);
    return 0;
}