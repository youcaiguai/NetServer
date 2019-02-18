// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//

#include <iostream>
#include <functional>
#include "HttpServer.h"


HttpServer::HttpServer(EventLoop *loop, int port)
    : tcpserver_(loop, port),
    cnt(0)
{
    tcpserver_.SetNewConnCallback(std::bind(&HttpServer::HandleNewConnection, this, std::placeholders::_1));
    tcpserver_.SetMessageCallback(std::bind(&HttpServer::HandleMessage, this, std::placeholders::_1, std::placeholders::_2));
    tcpserver_.SetSendCompleteCallback(std::bind(&HttpServer::HandleSendComplete, this, std::placeholders::_1));
    tcpserver_.SetCloseCallback(std::bind(&HttpServer::HandleClose, this, std::placeholders::_1));
    tcpserver_.SetErrorCallback(std::bind(&HttpServer::HandleError, this, std::placeholders::_1));
}

HttpServer::~HttpServer()
{

}

void HttpServer::HandleNewConnection(TcpConnection *ptcpconn)
{
    //std::string msg(s);
    HttpSession *phttpsession = new HttpSession();
    httpsessionnlist_[ptcpconn] = phttpsession;
}

void HttpServer::HandleMessage(TcpConnection *ptcpconn, std::string &s)
{
    //std::cout << "http num is:" << ++cnt << std::endl;   
    HttpSession *phttpsession =  httpsessionnlist_[ptcpconn];
    phttpsession->PraseHttpRequest(s);
    phttpsession->HttpProcess();
    std::string msg;
    phttpsession->AddToBuf(msg);
    ptcpconn->Send(msg);
    if(!phttpsession->KeepAlive())
    {
        //短连接，可以告诉框架层数据发完就可以关掉TCP连接，不过这里注释掉，还是交给客户端主动关闭吧
        //ptcpconn->HandleClose();
    }
}

void HttpServer::HandleSendComplete(TcpConnection *ptcpconn)
{

}

void HttpServer::HandleClose(TcpConnection *ptcpconn)
{
    HttpSession *phttpsession = httpsessionnlist_[ptcpconn];
    httpsessionnlist_.erase(ptcpconn);
    delete phttpsession;
}

void HttpServer::HandleError(TcpConnection *ptcpconn)
{
    HttpSession *phttpsession = httpsessionnlist_[ptcpconn];
    httpsessionnlist_.erase(ptcpconn);
    delete phttpsession;
}

void HttpServer::Start()
{
    tcpserver_.Start();
}
