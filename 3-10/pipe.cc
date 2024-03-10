#include<iostream>
#include<unistd.h>
#include<cassert>
#include<sys/types.h>
#include<sys/wait.h>
#include<cstring>
using namespace std;

#define MAX 1024
int main()
{
    int pipefd[2] = {0};
    int n  = pipe(pipefd);
    assert(n == 0);

    cout << "pipefd[0]: " << pipefd[0] << ",pipefd[1]: " << pipefd[1] << endl;

    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
    }
    
    //字写，父读
    if(id == 0)
    {
        //子进程
        close(pipefd[0]);

        
        // int cnt = 10;
        // while(cnt)
        // {
        //     char message[MAX];
        //     snprintf(message,sizeof(message),"hello father ! ! !  I am a child process, pid: %d, cnt: %d",getpid(),cnt--);
        //     write(pipefd[1],message,strlen(message));
        //     sleep(1);
        // }
        exit(0);
    }
    //父进程
    close(pipefd[1]);
    char buffer[MAX];

    while(1)
    {
        ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if(n > 0)
        {
            buffer[n] = '\0';
            cout << "child say: " << buffer << " to me !" << endl;
        }
    }

    pid_t rid = waitpid(id,nullptr,0);
    if(rid == id)
    {
        cout << "wait success" << endl;
    }


    return 0; 
}