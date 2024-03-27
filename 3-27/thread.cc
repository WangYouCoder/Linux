#include <iostream>
#include<unistd.h>
#include <string>
#include <thread>
using namespace std;

__thread int g_val = 100;

void* ThreadRoutine(void* argv)
{
    string name = (const char*)argv;
    while(true)
    {
        sleep(1);
        cout << "name: " << argv << " g_val: " << g_val << " , &g_val: " << &g_val << endl;
        g_val++;
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, ThreadRoutine, (void*)"thread-1");
    while(true)
    {
        sleep(1);
        cout << "name: main pthread, " << " g_val: " << g_val << " , &g_val: " << &g_val << endl;
    }

    pthread_join(tid,nullptr);
    return 0;
}






// void run()
// {
//     while(true)
//     {
//         cout << "I am thread" << endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     thread t(run);

//     t.join();

//     cout << "I am main thread" << endl;
//     return 0;
// }