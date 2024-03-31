#include "BlockQueue.hpp"
#include <ctime>
#include <unistd.h>
#include "Task.cpp"

void* consumer(void* args)
{
    BlockQueue<Task>* bq = static_cast<BlockQueue<Task>*>(args);
    while(true)
    {
        Task t;
        bq->Pop(&t);
        t.Run();
        std::cout << "comsumer data: " << t.PrintResult() << std::endl;

        sleep(1);
    }   
    return nullptr;
}

void* productor(void* args)
{
    BlockQueue<Task>* bq = static_cast<BlockQueue<Task>*>(args);
    while(true)
    {
        //int data = rand() % 10 + 1;
        int data1 = rand() % 10;
        usleep(rand()%123);
        int data2 = rand() % 10;
        usleep(rand()%123);
        char opera = opers[rand() % (opers.size())];
        Task t(data1, data2, opera);
        bq->Push(t);

        std:: cout << "productor data: " << t.PrintTask() << std::endl;
        
    }
    return nullptr;
}


int main()
{
    srand((uint16_t)time(nullptr));
    BlockQueue<Task> *bq = new BlockQueue<Task>();
    pthread_t c,p;
    pthread_create(&c, nullptr, consumer, bq);
    pthread_create(&c, nullptr, productor, bq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    return 0;
}