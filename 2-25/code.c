#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

void work()
{
    int cnt = 5;
    while(cnt)
    {
        printf("I am a process,pid: %d,ppid: %d, cnt: %d\n",getpid(),getppid(),cnt);
        sleep(1);
        cnt--;
    }

    int a = 10 / 0;
}

int main()
{

    pid_t id = fork();
    if(id == 0)
    {
        work();
        exit(10);
    }
    else 
    {
       // sleep(10);
       int status = 0;
       printf("wait before\n");
       if(id == waitpid(id,&status,0))
       {
           printf("wait success!,pid: %d,rpid: %d,exit sig: %d,exit code: %d\n",getpid(),id,status&0x7f,(status>>8));
       }
       printf("wait after\n");
    }    
    return 0;
}
