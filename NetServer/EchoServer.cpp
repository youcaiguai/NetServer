// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//

#include <iostream>
#include <functional>
#include "EchoServer.h"

EchoServer::EchoServer(EventLoop* loop, int port)
    : tcpserver_(loop, port),
    cnt(0)
{
    tcpserver_.SetNewConnCallback(std::bind(&EchoServer::HandleNewConnection, this, std::placeholders::_1));
    tcpserver_.SetMessageCallback(std::bind(&EchoServer::HandleMessage, this, std::placeholders::_1, std::placeholders::_2));
    tcpserver_.SetSendCompleteCallback(std::bind(&EchoServer::HandleSendComplete, this, std::placeholders::_1));
    tcpserver_.SetCloseCallback(std::bind(&EchoServer::HandleClose, this, std::placeholders::_1));
    tcpserver_.SetErrorCallback(std::bind(&EchoServer::HandleError, this, std::placeholders::_1));
}

EchoServer::~EchoServer()
{

}
void EchoServer::Start()
{
    tcpserver_.Start();
}

void EchoServer::HandleNewConnection(TcpConnection *ptcpconn)
{
    std::cout << "New Connection Come in" << std::endl;
}

void EchoServer::HandleMessage(TcpConnection *ptcpconn, std::string &s)
{
    //std::cout << "HandleMessage: " << ++cnt << std::endl;
    //std::cout << "client msg: " << s << std::endl;

    //std::string msg("reply msg:");
    //msg += s;
    //s.clear();
    //swap优化
    std::string msg;
    msg.swap(s);
    msg.insert(0, "reply msg:");
    ptcpconn->Send(msg);
}

void EchoServer::HandleSendComplete(TcpConnection *ptcpconn)
{
    //std::cout << "Message send complete" << std::endl;
}

void EchoServer::HandleClose(TcpConnection *ptcpconn)
{
    std::cout << "EchoServer conn close" << std::endl;
}

void EchoServer::HandleError(TcpConnection *ptcpconn)
{
    std::cout << "EchoServer error" << std::endl;
}
