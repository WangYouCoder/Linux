#pragma once
#include<iostream>
#include<sys/ipc.h> 
#include<sys/shm.h>
#include<cstring>
#include<cerrno>
using namespace std;

const int size = 4096;

const std::string pathname = "/home/040312-lsy/lesson/3-13/shm";
const int proj_id = 0x11223344;

key_t Getkey()
{
    key_t key = ftok(pathname.c_str(),proj_id);
    if(key < 0)
    {
        cerr << "errno: " << errno << ", errstring: " << strerror(errno) << endl;
    }

}

string ToHex(int id)
{
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "0x%x", id);
    return buffer;
}

int CreatShmHlper(key_t key, int flag)
{
    int shmid = shmget(key, size, flag);
    if(shmid < 0)
    {
        cerr << "errno: " << errno << ", errstring: " << strerror(errno) << endl;
        exit(2);
    }
    return shmid;
}

int CreatShm(key_t key)
{
    return CreatShmHlper(key, IPC_CREAT | IPC_EXCL | 0666);
}

int GetShm(key_t key)
{
    return CreatShmHlper(key, IPC_CREAT);
}