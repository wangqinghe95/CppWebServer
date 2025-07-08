#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define SERVER_IP ("127.0.0.1")
#define MAX_EVENTS (1024)
#define READ_BUFFER (1024)

void handleReadEvent(int sockfd);

int main()
{
    Socket* serv_sock = new Socket();
    InetAddress* serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();

    Epoll* ep = new Epoll();
    serv_sock->setnonblocking();
    ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);

    while (true)
    {
        std::vector<epoll_event> events = ep->poll();
        int nfds = events.size();
        for(int i = 0; i < nfds; ++i) {
            if(events[i].data.fd == serv_sock->getFd()) {
                InetAddress* clnt_addr = new InetAddress();
                Socket* clnt_sock = new Socket(serv_sock->accept(clnt_addr));
                printf("new client fd %d! IP: %s, Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
            
                clnt_sock->setnonblocking();
                ep->addFd(clnt_sock->getFd(), EPOLLIN | EPOLLET);
            }
            else if(events[i].events & EPOLLIN) {
                handleReadEvent(events[i].data.fd);
            }
            else {
                printf("epoll something else happened!");
            }
        }
    }
    
/*
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(-1 == sockfd, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(-1 == bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)), "socket bind error");

    errif(-1 == listen(sockfd, 128), "socket listen error");

    int epfd = epoll_create1(0);
    errif(-1 == epfd, "epoll create error");

    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    setnonblocking(sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    while (true)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(-1 == nfds, "epoll wait error");

        for(int i = 0; i < nfds; ++i) {
            if(events[i].data.fd == sockfd) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                bzero(&client_addr, sizeof(client_addr));

                int client_fd = accept(sockfd, (sockaddr*)&client_addr, &client_len);
                errif(-1 == client_fd, "socket accept error");

                printf("new client fd %d! IP: %s, Port: %d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                bzero(&ev, sizeof(ev));
                ev.data.fd = client_fd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(client_fd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
            }
            else if(events[i].events & EPOLLIN) {
                char buf[READ_BUFFER];
                ssize_t read_bytes = read(events[i].data.fd, buf, sizeof(buf));
                if(read_bytes > 0) {
                    printf("message from client fd %d: %s\n", events[i].data.fd, buf);
                    write(events[i].data.fd, buf, sizeof(buf));
                }
                else if(-1 == read_bytes && errno == EINTR) {
                    printf("continue reading\n");
                    continue;
                }
                else if(-1 == read_bytes && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
                    printf("finish reading once, errno: %d\n", errno);
                    break;
                }
                else if(0 == read_bytes) {
                    printf("client fd %d disconnected\n", events[i].data.fd);
                    close(events[i].data.fd);
                    break;
                }
                else {
                    printf("read others\n");
                }
            }
            else {
                printf("something else happened\n");
            }
        }
    }
    
    close(sockfd);    
*/
    return 0;
}

void handleReadEvent(int sockfd){
    char buf[READ_BUFFER];
    while (true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0) {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if(-1 == read_bytes && errno == EINTR) {
            printf("continue reading\n");
            continue;
        }
        else if(-1 == read_bytes && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("finish reading once, errno: %d\n", errno);
            break;
        }
        else if(0 == read_bytes) {
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
        else {
            printf("read something else had happened\n");
        }
    }
}