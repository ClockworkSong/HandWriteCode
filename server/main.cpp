#include <iostream>

#include <cstdio> // perror()
#include <ctime> // ctime()
#include <unistd.h> // close()

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE 10

static int PORT = 8056;

int main()
{
    //1. 调用socket函数创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);// IPv4 Internet protocols
    if( sockfd == -1) {
        perror("socket error:");
        return -1;
    }

    //2. 调用bind函数给创建的套接字，分配IP地址和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(sockaddr_in));
    if(ret == -1) {
        perror("bind error:");
        return -1;
    }

    //3. 调用listen函数进行监听，等待客户端连接
    ret = listen(sockfd, SIZE);
    if(ret == -1) {
        perror("listen error:");
        return -1;
    }

    // 客户端信息
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(sockaddr_in);

    while (1)
    {
        printf("waiting for connect...\n");

        //4. 等待客户请求到来: 当请求到来后，调用accept函数接受连接请求，返回一个对应于此次连接的新的套接字，做好相互通信准备
        int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (connfd == -1) {
            perror("accept error:");
            break;
        }

        printf("accept ok!\n");

        printf("connect from ip := %s, port := %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        char time_message[100];
        // timer_t now_time = (timer_t)time(NULL);
        // snprintf(time_message, sizeof(time_message), "%.24s\n", ctime((const timer_t)&now_time));
        char* s = "runoobcom";
        snprintf(time_message, sizeof(time_message), "%.24s\n", s);

        //5. 调用write/read或send/recv进行数据的读写，通过accept返回的套接字和客户端进行通信
        int ret = send(connfd, time_message, sizeof(time_message), 0);
        if (ret == -1) {
            perror("send error:");
            close(connfd);
            break;
        }

        close(connfd);
    }

    //6. 关闭服务套接口close
    close(sockfd);

    return 0;
}
