// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//服务器socket类，封装socket描述符及相关的初始化操作
#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Socket
{
private:
    /* data */
    int serverfd_;
    
public:
    Socket(/* args */);
    ~Socket();

    int fd() { return serverfd_; }    
    void SetSocketOption();
    void SetReuseAddr();
    void Setnonblocking();
    bool BindAddress(int serverport);
    bool Listen();
    int Accept(struct sockaddr_in &clientaddr);
    bool Close();
};


#endif