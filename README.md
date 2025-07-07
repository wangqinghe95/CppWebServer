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