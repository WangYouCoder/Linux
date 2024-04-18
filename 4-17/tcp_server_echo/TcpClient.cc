#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include "Comm.hpp"
#include <unistd.h>
using namespace std;

#define Retry_Count 5

void Usage(string proc)
{
    cout << "Usage : \n\t" << proc << " local_ port\n" << endl;
}

bool visitServer(string &serverip, uint16_t &port, int* cnt)
{
    bool ret = true;
    // 1. 创建sockfd
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        cerr << "socket error " << endl;
        ret = false;
        return ret;
    }
    *cnt = 0;
    // 2. connect      填充服务端的信息
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, serverip.c_str(), &server.sin_addr);  // 1. 字符串ip -> 4字节IP  2. 网络序列
    
    int n = connect(sockfd, CONV(&server), sizeof(server));  // 自动进行bind
    if(n < 0)
    {
        cerr << "connect error" << endl;
        ret = false;
    }

    // 并没有像server一样产生新的sockfd，未来我们就用connect成功的sockfd进行通信即可
    while(true)
    {
        string inbuffer;
        cout << "Please Enter# ";
        getline(cin, inbuffer);
        if(inbuffer == "quit")  break;
 
        ssize_t n = write(sockfd, inbuffer.c_str(), sizeof inbuffer);
        if(n > 0)
        {
            char buffer[1024];
            ssize_t m = read(sockfd, buffer, sizeof buffer);
            if(m > 0)
            {
                buffer[m] = 0;
                cout << "get a echo message -> " << buffer << endl;
            }
            else if(m == 0)
            {
                break;
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = false;
        }
    }

    close(sockfd);
    return ret;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }

    // 拿到服务端的信息
    string serverip = argv[1];
    uint16_t port = atoi(argv[2]);

    int cnt = 1;
    while(cnt <= Retry_Count)
    {
        bool result = visitServer(serverip, port, &cnt);
        if(result)
        {
            break;
        }
        else
        {
            sleep(1);
            cout << "server offline, retrying..., count: " << cnt << endl;
            cnt++;
        }
    }

    if(cnt >= Retry_Count)
    {
        cout << "server offline" << endl;
    }

    return 0;
}