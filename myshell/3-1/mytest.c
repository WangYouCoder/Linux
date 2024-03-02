#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>


#define FILE_NAME "log.txt"

int main()
{
    fprintf(stdout,"hello stdout\n");
    fprintf(stderr,"hello stderr\n");









    //int fd = open(FILE_NAME,O_WRONLY | O_CREAT | O_TRUNC ,0666);
    //if(fd < 0)
    //{
    //    perror("open");
    //    return 1;
    //}

    //dup2(fd,1);

    //printf("hello stdout\n");
    //fprintf(stderr,"hello stderr\n");

    //close(fd);


   // char ch[1024];
   // ssize_t s = read(0,ch,1024);
   // if(s < 0)
   // {
   //     return 1;
   // }
   // else 
   // {   
   //     //ch[s-1] = 0;
   //     write(1,ch,strlen(ch));
   // }

   // return 0;

    //close(0);
    ////int fd = open(FILE_NAME,O_WRONLY | O_CREAT | O_APPEND,0666);
    //int fd = open(FILE_NAME,O_RDONLY);
    //if(fd < 0)
    //{
    //    perror("open");
    //    return 1;
    //}
    //
    //char buffer[1024];
    //fread(buffer,1,sizeof(buffer),stdin);
    //printf("%s\n",buffer);

    //fprintf(stdout,"fd: %d\n",fd);
    //fprintf(stdout,"stdout ->fd: %d\n",stdout->_fileno);
    //fflush(stdout);

    //close(fd);
    return 0;
}


//int main()
//{
//    printf("stdin->fd: %d\n",stdin->_fileno);
//    printf("stdout->fd: %d\n",stdout->_fileno);
//    printf("stderr->fd: %d\n",stderr->_fileno);
//
//    return 0;
//}

//int main()
//{
//    umask(0);
//    int fd1 = open("log1.txt",O_WRONLY | O_CREAT, 0666);
//    int fd2 = open("log2.txt",O_WRONLY | O_CREAT, 0666);
//    int fd3 = open("log3.txt",O_WRONLY | O_CREAT, 0666);
//    int fd4 = open("log4.txt",O_WRONLY | O_CREAT, 0666);
//    if(fd1 < 0)
//    {
//        perror("open");
//    }
//
//    const char* ch = "hello Linux\n";
//    write(fd1,ch,strlen(ch));
//    
//    printf("fd1: %d\n",fd1);
//    printf("fd2: %d\n",fd2);
//    printf("fd3: %d\n",fd3);
//    printf("fd4: %d\n",fd4);
//
//    close(fd1);
//    close(fd2);
//    close(fd3);
//    close(fd4);
//
//    return 0;
//}
//
//







//int main()
//{
//    FILE* fp = fopen("log.txt","w");
//    if(fp == NULL)
//    {
//        perror("fopen");
//        return 1;
//    }
//
//    fclose(fp);
//    return 0;
//}
