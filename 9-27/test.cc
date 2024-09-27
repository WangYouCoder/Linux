#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
using namespace std;

int main()
{
    int flag = false;
    condition_variable cv;
    mutex mtx;
    int x = 1;

    thread t1([&](){
        for(int i = 0; i < 10; i++)
        {
            unique_lock<mutex> lock(mtx);
            if(flag == true)
                cv.wait(lock);

            flag = true;
            cout << this_thread::get_id() << ": "<< x++ << endl;
            cv.notify_one();
        }
    });

    thread t2([&](){
        for(int i = 0; i < 10; i++)
        {
            unique_lock<mutex> lock(mtx);
            if(flag == false)
                cv.wait(lock);

            flag = false;
            cout << this_thread::get_id() << ": "<< x++ << endl;

            cv.notify_one();
        }
    });

    t1.join();
    t2.join();
    return 0;
}