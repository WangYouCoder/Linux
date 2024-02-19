#include"process.h"
#define TARGET 1024*1024
#define DNUM 1024*10

typedef void (*callback_t)(int);

void Download(callback_t cb)
{
    int target = TARGET;
    int total = 0;
    
    while(total < target)
    {
        usleep(1000*40);
        total += DNUM;
       double rate = total*100 / target; 
        cb(rate);
    }
    printf("\n");
}

void Down()
{
    int target = TARGET;
    int total = 0;
    
    while(total < target)
    {
        total += DNUM;
        process_v2(total*100 / target);
        usleep(1000*20);
    }

    printf("\n");
}

int main()
{
     Down();
    Download(process_v3);
    return 0;
}
