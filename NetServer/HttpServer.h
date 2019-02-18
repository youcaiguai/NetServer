// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//

#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include <string>
#include <map>
#include "EventLoop.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "HttpSession.h"


class HttpServer
{
private:
    /* data */
    
    void HandleNewConnection(TcpConnection *ptcpconn);
    void HandleMessage(TcpConnection *ptcpconn, std::string &s);
    void HandleSendComplete(TcpConnection *ptcpconn);
    void HandleClose(TcpConnection *ptcpconn);
    void HandleError(TcpConnection *ptcpconn);

    TcpServer tcpserver_;
    std::map<TcpConnection *, HttpSession*> httpsessionnlist_;//管理Http会话
    long long int cnt;

public:
    HttpServer(EventLoop *loop, int port);
    ~HttpServer();

    void Start();

};


#endif
