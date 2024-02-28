#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
    char *const argv[] = {"ls","-a","-l",NULL};
    pid_t id = fork();
    if(id == 0)
    {
        execl("./myprocess","myprocess",NULL);

       // printf("pid: %d,exec command begin\n",getpid());
       // sleep(3);
       // //execl("/usr/bin/ls","ls","-l","-a",NULL);
       // //execlp("ls","ls","-l","-a",NULL);
       //// execv("/usr/bin/ls",argv);
       // execvp("ls",argv);
       // printf("pid: %d,exec command end\n",getpid());
    }
    else 
    {
        //father
        pid_t rid = waitpid(-1,NULL,0);
        if(rid > 0)
        {
            printf("wait success,rid: %d\n",rid);
        }
    }
    return 0;
}
