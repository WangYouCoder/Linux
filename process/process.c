#include"process.h"

const char* str = "/-|\\";


//void process()
//{
//    int rate = 0;
//    char bar[101] = {0};
//    int len = strlen(str);
//
//    while(rate <= 100)
//    {
//        printf("[%-100s][%d%%][%c]\r",bar,rate,str[rate%len]);
//        fflush(stdout);
//        usleep(1000*20);
//        bar[rate++] = '#';
//    }
//    printf("\n");
//}


void process_v2(int rate)
{
    static char bar[101] = {0};
    int len = strlen(str);

    if(rate >= 0 && rate <= 100)
    {
        printf("[%-100s][%d%%][%c]\r",bar,rate,str[rate%len]);
        fflush(stdout);
        usleep(1000*20);
        bar[rate] = '#';
        if(rate == 100) bar[rate] = '\0';
    }
}

void process_v3(double rate)
{
    static char bar1[101] = {0};
    int len = strlen(str);

    static int cnt = 0;
    cnt++;
    cnt = cnt > len ? 0 : cnt;

    if(rate >= 0 && rate <= 100)
    {
        printf("[%-100s][%.1f%%][%c]\r",bar1,rate,str[cnt]);
        fflush(stdout);

        if((int)rate < 99)
        {
            bar1[(int)rate++] = '=';
            bar1[(int)rate] = '>';
        }
        else 
        {
           // bar[(int)rate] = '=';
           bar1[99] = '=';
        }
    }
}
