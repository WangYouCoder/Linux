#include<iostream>
#include<unistd.h>
#include<signal.h>

using namespace std;

void handle(int signal)
{
    cout << "获得" << signal  <<  "号信号" << endl;
}

// int main()
// {
//     cout << "getpid : " << getpid() << endl;
//     signal(2,handle);

//     sigset_t block,oblock;
//     sigemptyset(&block);
//     sigemptyset(&oblock);

//     sigaddset(&block, 2);

//     sigprocmask(SIG_BLOCK, &block, &oblock);

//     while(true)
//     {
//         sleep(1);
//     }

//     return 0;
// }

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
int main()
{
    cout << "getpid: " << getpid() << endl;
    signal(2,handle);
    //1.屏蔽2号信号
    sigset_t block,oblock;
    sigemptyset(&block);
    sigemptyset(&oblock);
    sigaddset(&block, 2);
    sigprocmask(SIG_BLOCK, &block, &oblock);

    sigset_t pending;
    int cnt = 0;
    while(true)
    {
        sigpending(&pending);
        PrintPending(pending);
        sleep(1);

        cnt++;
        if(cnt == 10)
        {
            cout << "解除对2号信号的屏蔽, 2号信号准备抵达 " << endl;
            sigprocmask(SIG_SETMASK, &oblock, NULL);
        }

    }

    return 0;
}