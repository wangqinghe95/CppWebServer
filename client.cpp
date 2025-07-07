#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include "util.hpp"

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(-1 == sockfd, "socket create error");


    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    serv_addr.sin_port = htons(8888);

    errif(-1 == connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)), "socket connect error");

    while (true)
    {
        char buf[2024];
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);

        if(strncmp(buf, "exit", 4) == 0) {
            break;
        }

        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if(-1 == write_bytes) {
            printf("socket already disconnected, can't wirte any more");
            break;
        }

        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0) {
            printf("message from server %s\n", buf);
        }
        else if(0 == read_bytes) {
            printf("client fd disconnected\n");
            break;
        }
        else if(-1 == read_bytes) {
            close(sockfd);
            errif(true, "socket read error");
        }
        else {
            printf("others\n");
        }
    }
    
    close(sockfd);
    return 0;
}