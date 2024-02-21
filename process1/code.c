#include<stdio.h>
#include<unistd.h>
int main()
{
    int i = 1000;
    while(i--)
    {
        pid_t id = getpid();
        printf("我是一个进程...,我的pid是：%d\n",id);
        sleep(1);
    }
    return 0;
}
