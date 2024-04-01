#include "RingQueue.hpp"
#include <unistd.h>
#include <pthread.h>
#include "Task.hpp"
void* consumer(void* args)
{
    //RingQueue<int>* rq = static_cast<RingQueue<int>*>(args);
    RingQueue<Task>* rq = static_cast<RingQueue<Task>*>(args);

    // int cnt = 100;
    // while(cnt)
    // {
    //     rq->Push(cnt);
    //     std::cout << "productor done, data is " << cnt << std::endl;
    //     cnt--;
    //     sleep(1);
    // }
     while(true)
    {
        Task t;
        rq->Pop(&t);
        t.Run();
        std::cout << "comsumer data: " << t.PrintResult() << std::endl;

        sleep(1);
    }   
    return nullptr;
}

void* productor(void* args)
{
    //RingQueue<int>* rq = static_cast<RingQueue<int>*>(args);
    RingQueue<Task>* rq = static_cast<RingQueue<Task>*>(args);
    // while(true)
    // {
    //     int data;
    //     rq->Pop(&data);
    //     std::cout << "consumer done, data is " << data << std::endl;
    // }
    while(true)
    {
        //int data = rand() % 10 + 1;
        int data1 = rand() % 10;
        usleep(rand()%123);
        int data2 = rand() % 10;
        usleep(rand()%123);
        char opera = opers[rand() % (opers.size())];
        Task t(data1, data2, opera);
        rq->Push(t);

        std:: cout << "productor data: " << t.PrintTask() << std::endl;
        
    }
    return nullptr;
}

int main()
{
    //RingQueue<int> *rq = new RingQueue<int>();

    RingQueue<Task> *rq = new RingQueue<Task>();
    pthread_t c,p;
    pthread_create(&c, nullptr, consumer, rq);
    pthread_create(&p, nullptr, productor, rq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    return 0;
}