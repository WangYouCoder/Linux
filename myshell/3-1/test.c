#include<stdio.h>

#define Print1 1          // 0001
#define Print2 (1 << 1)   // 0010
#define Print3 (1 << 2)   // 0100
#define Print4 (1 << 3)   // 1000

void Print(int flag)
{
    if(flag & Print1) printf("Print1\n");
    if(flag & Print2) printf("Print2\n");
    if(flag & Print3) printf("Print3\n");
    if(flag & Print4) printf("Print4\n");

    printf("\n");
}

int main()
{
    
    Print(Print1);
    Print(Print1 | Print2);
    Print(Print1 | Print2 | Print3);
    Print(Print1 | Print2 | Print3 | Print4);
    return 0;
}
