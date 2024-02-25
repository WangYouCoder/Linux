#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    printf("I am a process");
    sleep(3);
    _exit(0);
}



//int fun()
//{
//    printf("call fun function!\n");
//    
//    //return 11;
//    exit(11);
//}
//
//int main()
//{
//    fun();
//    printf("I am a process!\n");
//
//    return 12;
//}




//int main()
//{
//    int ret = 0;
//    FILE *fp = fopen("./log.txt","r");
//    if(fp == NULL)
//    {
//        printf("errno: %d,errno string: %s\n",errno,strerror(errno));
//        ret = errno;
//    }
//
//    return ret;
//}


//int main()
//{
//    int i = 0;
//    for(i = 0; i < 200; i++)
//    {
//        printf("%d: %s\n",i,strerror(i));
//    }
//
//    return 0;
//}
