#include "Socket.h"
#include "util.hpp"
#include <unistd.h>
#include <fcntl.h>

Socket::Socket() : fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}
Socket::Socket(int _fd) : fd(_fd)
{
    errif(fd == -1, "socket create error");
}

Socket::~Socket()
{
    if(fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress* addr)
{
    errif(-1 == ::bind(fd, (sockaddr*)&addr->addr, addr->addr_len), "socket bind error");
}
void Socket::listen()
{
    errif(-1 == ::listen(fd, SOMAXCONN), "socket listen error");
}
void Socket::setnonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);

}
int Socket::accept(InetAddress* addr)
{
    int client_fd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(-1 == client_fd, "socket accept error");
    return client_fd;
   
}
int Socket::getFd()
{
    return fd;
}