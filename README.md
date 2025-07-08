# CppWebServer
A Web Server learn from 30daysCppWebServer

## Introduction

## Compile

## Reference
![CppWebServer](https://github.com/Wlgls/30daysCppWebServer/tree/main)

## Details

### day01

socket 的建立流程，服务端是：socket，bind，listen，accept，recv/send，close。客户端是 socket，connect，send/recv，close

就 server 来分析，socket 返回一个文件描述符，用于监听客户端的连接请求。有连接请求过来后，会再使用一个文件描述符用于存储客户端的地址信息，比如ip，端口等等。然后在使用该文件描述符来 send/recv 数据。等到本次数据传输完毕，那么就会放弃（一般情况下）或者释放该文件描述符。服务器继续使用第一个文件描述符监听客户端的请求。

### day02

添加了错误判断流程和多次信息互相传输的逻辑。

### day03

使用 epoll 完成网络 IO 的复用，即服务器同时保持多个客户端的连接。

epoll 的步骤是：创建 epoll 文件描述符，将一个 epoll 事件加入到该文件描述符中，等待 epoll 事件返回。

epoll 和 socket 的结合步骤是：
1. 创建一个服务器接受客户端请求的 socket，即 server_fd，然后执行绑定，监听。
2. 创建一个 epoll 的文件描述符，和一个 epoll 事件的结构体，将 server_fd 的监听事件加入到 epoll 中
3. 等待 epoll 的事件返回。如果有客户端新连接事件，则 accept，并且同时将请求的客户端 fd 加入到 epoll 的监听事件中，如果是客户端发送消息，则执行 read/write。

## day04

使用类，将管理地址的 InetAddress，Socket，Epoll 模块分开。
想起来一个点：socket 监听的 fd，和 Epoll 的 wait fd 之间的逻辑关系是什么？
socket 的 fd 是监听外部客户端请求事件，而 epoll 的 fd 则是监视 socket
fd 是否有事件发生。