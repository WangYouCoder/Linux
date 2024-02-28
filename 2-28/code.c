#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

void Worker(int cnt)
{
    printf("I am child,pid: %d,cnt: %d\n",getpid(),cnt);
}

int main()
{
    pid_t id = fork();
    if(id == 0)
    {
        //child
        int cnt = 10;
        while(cnt)
        {
            Worker(cnt);
            sleep(1);
            cnt--;
        }

        exit(0);
    }
    
    //father
    int status = 0;
    pid_t rid = waitpid(id,&status,WNOHANG);
    if(rid > 0)
    {
        //wait success ,child quit now
        printf("child quit success ,exit code: %d,exit singal: %d\n",(status >> 8)&0xFF,status&0x7F);
    }
    else if(rid == 0)
    {
        // wait success,but child not quit
        printf("father do other thing...\n");
    }
    else 
    {
        //wait fait
        printf("wait fail!\n");
    }

    return 0;
}


