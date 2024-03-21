#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
using namespace std;

int gcnt = 100;
void* ThreadRoutine(void* arg)
{
    const char *threadname = (const char*) arg;

    while(true)
    {
        cout << threadname << "getpid: " << getpid() << " gcnt: " << gcnt << " &gcnt: " << &gcnt << endl;
        gcnt--;
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,ThreadRoutine,(void*)"thread 1");

    while(true)
    {
        cout << "i am main phread" << "getpid: " << getpid() << " gcnt: " << gcnt << " &gcnt: " << &gcnt << endl;
        sleep(1);
    }

    return 0;
}