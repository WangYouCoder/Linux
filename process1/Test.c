#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>

int main()
{
    while(1)
    {
        printf("I am a process,pid:%d\n",getpid());
        sleep(1);
    }
    return 0;
}


////孤儿进程
//int main()
//{
//    pid_t id = fork();
//    if(id < 0) return 1;
//    else if(id == 0)
//    {
//        //子进程
//        while(1)
//        {
//            printf("I am a child,runing any  time\n");
//            sleep(1);
//        }
//    }
//    else 
//    {
//        //父进程
//        int x = 5;
//        while(x)
//        {
//            printf("I am father,run times:%d\n",x--);
//            sleep(1);
//        }
//        printf("I am father, dead!\n");
//    }
//    return 0;
//}

//僵尸进程
// 
//int main()
//{
//    pid_t id = fork();
//    if(id < 0) return 1;
//    else if(id == 0)
//    {
//        //子进程
//        int x = 5;
//        while(x)
//        {
//            printf("I am a child,run times:%d\n",x--);
//            sleep(1);
//        }
//        exit(2);
//    }
//    else 
//    {
//        //父进程
//        while(1)
//        {
//            printf("I am father,runing any time\n");
//            sleep(1);
//        }
//    }
//    return 0;
//}
