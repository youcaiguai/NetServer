// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//回射服务器

#ifndef _ECHO_SERVER_H_
#define _ECHO_SERVER_H_

#include <string>
#include "TcpServer.h"
#include "EventLoop.h"
#include "TcpConnection.h"

class EchoServer
{
private:
    /* data */
    //业务函数
    void HandleNewConnection(TcpConnection *ptcpconn);
    void HandleMessage(TcpConnection *ptcpconn, std::string &s);
    void HandleSendComplete(TcpConnection *ptcpconn);
    void HandleClose(TcpConnection *ptcpconn);
    void HandleError(TcpConnection *ptcpconn);

    TcpServer tcpserver_;
    long long int cnt;

public:
    EchoServer(EventLoop* loop, int port);
    ~EchoServer();

    void Start();

};

#endif