#include<iostream>
#include<sys/ipc.h> 
#include<sys/shm.h>
#include<cstring>
#include<cerrno>
#include<unistd.h>
#include"comm.hpp"
using namespace std;

int main()
{
    key_t key = Getkey();
    cout << "key: " << ToHex(key) << endl;

    int shmid = CreatShm(key);
    sleep(5);

    cout << "shmid: " << shmid << endl;
    cout << "开始将shm映射到进程的地址空间中" << endl;
    char *s = (char*)shmat(shmid, NULL, 0);

    sleep(5);
    shmdt(s);
    cout << "将shm从进程的地址空间中退出" << endl;

    sleep(5);
    shmctl(shmid, IPC_RMID, nullptr);
    cout << "在OS中删除共享内存" << endl;

    sleep(10);
    return 0;
}