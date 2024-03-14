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
    int shmid = GetShm(key);
    char *s = (char*)shmat(shmid, NULL, 0);
    cout << "attach shm done" << endl;
    //sleep(5);

    int fd = open(FILENAME,O_WRONLY);

    char c = 'a';
    for(; c < 'z'; c++)
    {
        s[c - 'a'] = c;
        cout << "write: " << c << " done" << endl;

        int code = 1;
        write(fd,&code,sizeof(code));
        sleep(3);
    }

    shmdt(s);
    return 0; 
}