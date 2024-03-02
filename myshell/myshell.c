#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define NoneRedir 0
#define InputRedir 1
#define OutputRedir 2
#define AppendRedir 3
char *filename = NULL;
int Redir = 0;
#define NUM 1024
#define SIZE 100
//#define Debug 1

char cwd[1024];
char env_val[1024];
int lastcode = 0;

const char* getUsername()
{
    const char* name = getenv("USER");
    if(name) return name;
    else return "none";
}

const char* getHostname()
{
    const char* hostname = getenv("HOSTNAME");
    if(hostname) return hostname;
    else return "none";
}

const char* getPwd()
{
    const char* pwd = getenv("PWD");
    if(pwd) return pwd;
    else return "none";
}

int getCommand(char* command,int num)
{
    
    printf("[%s@%s %s]# ",getUsername(),getHostname(),getPwd());

    char* r = fgets(command,num,stdin);
    if(r == NULL) return -1;

    //"abcd\n",将\n改为\0，这样就不会换行了
    command[strlen(command) - 1] = '\0';

    return strlen(command);
}

void commandSplit(char* in,char* out[])
{
    int argc = 0;
    out[argc++] = strtok(in," ");
    while(out[argc++] = strtok(NULL," "));

#ifdef Debug
    printf("\n");
    for(int i = 0; i < argc; i++)
    {
        printf("%d : %s\n",i,out[i]);
    }
#endif
}

int execute(char* argv[])
{
    pid_t id = fork();
    if(id < 0) return -1;
    else if(id == 0)
    {
        //child
        int fd = 0;
        if(Redir == InputRedir)
        {
             fd = open(filename,O_RDONLY);
             dup2(fd,0);
        }
        else if(Redir == OutputRedir)
        {
            fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC ,0666);
            dup2(fd,1);
        }
        else if(Redir == AppendRedir)
        {
            fd = open(filename,O_WRONLY | O_CREAT | O_APPEND ,0666);
            dup2(fd,1);
        }
        else 
        {

        }

        execvp(argv[0],argv);
        exit(1);
    }
    else 
    {
        //father
        int status = 0;
        pid_t rid = waitpid(id,&status,0);
        if(rid > 0)
        {
            lastcode = WEXITSTATUS(status);
        }
    }

    return 0;
}

void cp(const char* path)
{
    //chdir 更改当前路径为path
    chdir(path);
    char tmp[1024];  
    getcwd(tmp,sizeof(tmp));
    sprintf(cwd,"PWD=%s",tmp);
    putenv(cwd);
}

// 1 -> yes, 0 -> no
int doBuildin(char* argv[])
{
    if(strcmp(argv[0],"cd") == 0)
    {
        char* path = NULL;
        if(argv[1] == NULL) path = ".";
        else path = argv[1];
        cp(path);
        return 1;
    }
    else if(strcmp(argv[0],"export") == 0)
    {
        if(argv[1] == NULL) return 1;
        strcpy(env_val,argv[1]);
        putenv(env_val);
        return 1;
    }
    else if(strcmp(argv[0],"echo") == 0)
    {
        char *val = argv[1]+1;
        if(strcmp(val,"?") == 0)
        {
            printf("%d\n",lastcode);
        }
        else 
        {
            printf("%s\n",getenv(val));
        }
        return 1;
    }

    return 0;
}

#define SkipSpace(pos) do{ while(isspace(*pos)) pos++; }while(0)

void checkRedir(char command[],size_t len)
{
    char* end = command + len - 1;
    char* start = command;
    while(end > start)
    {
        if(*end == '<')
        {
            *end = '\0';
            filename = end + 1;
            SkipSpace(filename);
            Redir = InputRedir;
            break;
        }
        else if(*end == '>')
        {
            if(*(end - 1) == '>')
            {
                *(end - 1) = '\0';
                filename = end + 1;
                SkipSpace(filename);
                Redir = AppendRedir;
                break;
            }
            else 
            {
                *end = '\0';
                filename = end + 1;
                SkipSpace(filename);
                Redir = OutputRedir;
                break;
            }
        }
        else 
        {
            end--;
        }
    }
}

int main()
{
    while(1)
    {
        filename = NULL;
        Redir = 0;
        char usercommand[NUM];
        char* argv[SIZE];
        //1.  打印提示符&&获取用户命令字符串
        int n = getCommand(usercommand,sizeof(usercommand));
        if(n <= 0) continue;

        checkRedir(usercommand,strlen(usercommand));
        //printf("%s",usercommand);
        //2.分割字符串
        commandSplit(usercommand,argv);
        //3.判断是否是内建命令
        n = doBuildin(argv);
        if(n) continue;
        //4. 执行相应命令
        execute(argv);
    }
    return 0;
}
