#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int g_val = 100;

int main()
{
    pid_t id = fork();
    if(id == 0)
    {
        int x = 5;
        while(1)
        {
            printf("child,pid: %d, ppid: %d, g_val: %d, &g_val: %p\n",getpid(),getppid(),g_val,&g_val);
            sleep(1);
            if(x == 0)
            {
                g_val = 200;
                printf("child change g_val: 100->200\n");
            }
            x--;
        }
    }
    else 
    {
        //father
        while(1)
        {
            printf("father,pid: %d, ppid: %d, g_val: %d, &g_val: %p\n",getpid(),getppid(),g_val,&g_val);
            sleep(1);
        }
    }
}



//int un_gval;
//int init_gval = 10;
//
//int main(int argc,char* argv[],char* env[])
//{
//    printf("code addr: %p\n",main);
//    
//    const char* str = "hello Linux";
//    printf("read only char addr: %p\n",str);
//
//    printf("init global value addr: %p\n",&init_gval);
//
//    printf("uninit global value addr: %p\n",&init_gval);
//
//    char* heap = (char*)malloc(100);
//    printf("heap addr: %p\n",heap);
//
//    printf("stack addr: %p\n",&heap);
//
//    int i = 0;
//    for(; argv[i]; i++)
//    {
//        printf("argv[%d]: %p\n",i,&argv[i]);
//    }
//
//    for(i = 0; env[i]; i++)
//    {
//        printf("env[%d]: %p\n",i,env[i]);
//    }
//
//    return 0;
//}
