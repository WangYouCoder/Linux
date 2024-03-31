#include<iostream>
#include<pthread.h>
#include<string>
#include<unistd.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int ticket = 1000;

void* RoutineThread(void* argc)
{
    std::string name = static_cast<const char*>(argc);
    while(true)
    {
        //sleep(1);
        pthread_mutex_lock(&mutex);
        if(ticket > 0)
        {
            std::cout << name << " , get a ticket : " << ticket-- <<std::endl;
            usleep(1000);
        }
        else
        {
            std::cout << "没票了" << name << std::endl;
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_t t1, t2, t3;
    pthread_create(&t1, nullptr, RoutineThread, (void*)"thread-1");
    pthread_create(&t2, nullptr, RoutineThread, (void*)"thread-2");
    pthread_create(&t3, nullptr, RoutineThread, (void*)"thread-3");

    sleep(3);
    while(true)
    {
        //pthread_cond_signal(&cond);

        sleep(6);
        pthread_mutex_lock(&mutex);
        ticket += 1000;
        pthread_mutex_unlock(&mutex);

        pthread_cond_signal(&cond);
    }

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    return 0;
}