#include"Add.h"
#include"Sub.h"
#include"Mul.h"

int main()
{
    int a = 10, b = 20;
    printf("%d+%d=%d\n",a,b,Add(a,b));
    printf("%d-%d=%d\n",a,b,Sub(a,b));
    printf("%d*%d=%d\n",a,b,Mul(a,b));

    return 0;
}
