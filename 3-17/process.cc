#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
using namespace std;

void handle(int signal)
{
    cout << "获得" << signal << "号信号" << endl;
    sleep(1);
}

// //ctrl + z  q  \

// int main()
// {
//     signal(2,handle);  //ctrl + c
//     signal(20,handle); //ctrl + z
//     signal(3,handle);  //ctrl + \

//     while(true)
//     {
//         cout << "runing ..." << "pid: " << getpid() << endl;
//         sleep(1);
//     }
//     return 0;
// }

int main()
{

    signal(8,handle);

    int a = 10;
    a /= 0;
    return 0;
}