#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main()
{
    printf("C: hello printf\n");
    fprintf(stdout,"C: hello fprintf\n");
    fputs("C: hello fputs\n",stdout);

    const char* ch = "system: hello write\n";
    write(1,ch,strlen(ch));

    //注意fork()的位置
   // fork();
    return 0;
}
