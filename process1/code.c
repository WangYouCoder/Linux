#include<stdio.h>
#include<unistd.h>
int main()
{
    printf("我是一个父进程，我的pid:%d\n",getpid());
    pid_t id =  fork();

    if(id < 0) return 1;
    else if(id == 0)
    {
        //child
        while(1)
        {
            printf("我是子进程:pid: %d,ppid: %d,ret: %d,我正在执行下载任务,&idi:%p\n",getpid(),getppid(),id,&id);
            sleep(1);
        }
    }
    else 
    {
        while(1)
        {

            printf("我是父进程:pid: %d,ppid: %d,ret: %d,我正在执行播放任务,&id:%p\n",getpid(),getppid(),id,&id);
            sleep(1);
         }
    }



//    int i = 1000;
//    while(i--)
//    {
//        pid_t id = getpid();
//        pid_t ppid = getppid();
//        printf("I am a process pid:%d,parent id:%d\n",id,ppid);
//        sleep(1);
//    
//    }
//
   // printf("我是一个父进程，我的pid:%d,ppid:%d\n",getpid(),getppid());

   // fork();

   // printf("我是一个进程，我的pid:%d,ppid:%d\n",getpid(),getppid());//注意这里只运行了一次

   // sleep(1);
    return 0;
}
