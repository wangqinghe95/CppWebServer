#ifndef SOCKET_H__
#define SOCKET_H__

#include "InetAddress.h"

class InetAddress;

class Socket
{
private:
    int fd;

public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    void setnonblocking();
    int accept(InetAddress*);
    int getFd();
};

#endif