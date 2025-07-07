#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "util.hpp"

#define SERVER_IP ("127.0.0.1")

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(-1 == sockfd, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(-1 == bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)), "socket bind error");

    errif(-1 == listen(sockfd, 128), "socket listen error");

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));

    int client_fd = accept(sockfd, (sockaddr*)&client_addr, &client_len);
    errif(-1 == client_fd, "socket accept error");

    printf("new client fd %d! IP: %s, Port: %d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    while (true)
    {
        char buf[2024];
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(client_fd, buf, sizeof(buf));
        if(read_bytes > 0) {
            printf("message from client fd %d: %s\n", client_fd, buf);
            write(client_fd, buf, sizeof(buf));
        }
        else if(0 == read_bytes) {
            printf("client fd %d disconnected\n", client_fd);
            close(client_fd);
            break;
        }
        else if(-1 == read_bytes) {
            close(client_fd);
            errif(true, "socket read error");
        }
        else {
            printf("others\n");
        }
    }

    close(sockfd);    

    return 0;
}