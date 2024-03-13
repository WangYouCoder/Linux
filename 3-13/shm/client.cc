#include<iostream>
#include<sys/ipc.h> 
#include<sys/shm.h>
#include<cstring>
#include<cerrno>
#include"comm.hpp"
using namespace std;

int main()
{
    key_t key = Getkey();
    int chmid = GetShm(key);
    
    return 0;
}