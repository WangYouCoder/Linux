#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    printf("%s\n",getenv("PATH"));
    return 0;
}

//int main(int argc,char* argv[])
//{
//    int i = 0;
//    for(i = 0; i < argc; i++)
//    {
//        printf("%d:%s\n",i,argv[i]);
//    }
//
//    printf("hello Linux\n");
//    return 0;
//}






















//int main()
//{
//    while(1)
//    {
//       printf("hello Linux\n");
//       sleep(1);
//    }
//
//
//
//
//
//
//   // int num = 0;
//   // printf("输入一个数字:");
//   // scanf("%d",&num);
//   // printf("echo:%d\n",num);
//   // 
//    return 0;
//
//}
