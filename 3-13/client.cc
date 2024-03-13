#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<cerrno>
#include<cstring>
#include<fcntl.h>
#include<unistd.h>
#include"comm.h"

using namespace std;

int main()
{
    int wfd = open(FILENAME,O_WRONLY);
    if(wfd < 0)
    {
        cerr << "errno: " << errno << ", errstring: " << strerror(errno) << endl;
        return 1;
    }

    string message;
    while(true)
    {
        cout << "Please enter: ";
        getline(cin, message);

        ssize_t s = write(wfd, message.c_str(), message.size());
        if(s < 0)
        {
            cerr << "errno: " << errno << ", errstring: " << strerror(errno) << endl;
            break;
        }
    }

    close(wfd);
    return 0;
}