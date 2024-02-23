#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(int argc,char* argv[])
{
    if(argc != 4)
    {
        printf("使用错误，使用方式: %s op[add][sub][mul][div]\n",argv[0]);
    }

    int x = atoi(argv[2]);
    int y = atoi(argv[3]);

    if(strcmp(argv[1], "add")== 0)
    {
        printf("%d+%d=%d\n",x,y,x+y);
    }
    else if(strcmp(argv[1], "sub") == 0)
    {

        printf("%d-%d=%d\n",x,y,x-y);
    }
    else if(strcmp(argv[1], "mul") == 0)
    {
    
        printf("%d*%d=%d\n",x,y,x*y);
    }
    else if(strcmp(argv[1], "div") == 0)
    {
        if(y == 0) printf("被除数不能为0\n");
        printf("%d/%d=%d\n",x,y,x/y);
    }
    else 
    {
        printf("使用错误，使用方式: %s op[add][sub][mul][div]\n",argv[0]);
    }
    return 0;
}
