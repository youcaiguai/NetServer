// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//TcpConnection类，表示客户端连接

#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include <functional>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Channel.h"
#include "EventLoop.h"
//#include "Socket.h"

class TcpConnection
{
public:
    typedef std::function<void(TcpConnection*)> Callback;
    typedef std::function<void(TcpConnection*, std::string&)> MessageCallback;
    typedef std::function<void()> TaskCallback;
    TcpConnection(EventLoop *loop, int fd, struct sockaddr_in clientaddr);
    ~TcpConnection();

    int fd()
    { return fd_; }
    //void FillBuffout(std::string &s);
    void Send(std::string &s);
    void HandleRead();
    void HandleWrite();
    void HandleError();
    void HandleClose();

    void SetMessaeCallback(MessageCallback cb)
    {
        messagecallback_ = cb;
    }
    void SetSendCompleteCallback(Callback cb)
    {
        sendcompletecallback_ = cb;
    }
    void SetCloseCallback(Callback cb)
    {
        closecallback_ = cb;
    }
    void SetErrorCallback(Callback cb)
    {
        errorcallback_ = cb;
    }
    //连接清理函数
    void SetConnectionCleanUp(TaskCallback cb)
    {
        connectioncleanup_ = cb;
    }
private:
    /* data */
    EventLoop *loop_;
    Channel *pchannel_;
    int fd_;
    struct sockaddr_in clientaddr_;
    bool halfclose_; //半关闭标志位
    bool disconnected_; //已关闭标志位

    //读写缓冲
    std::string bufferin_;
    std::string bufferout_;


    MessageCallback messagecallback_;
    Callback sendcompletecallback_;
    Callback closecallback_;
    Callback errorcallback_;

    TaskCallback connectioncleanup_;
};


#endif