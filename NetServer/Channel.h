// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//Channel类，fd和事件的封装

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <functional>

class Channel
{

public:
    typedef std::function<void()> Callback;

    Channel();
    ~Channel();

    void SetFd(int fd) 
    {
        fd_ = fd; 
    }
    int GetFd() 
    { 
        return fd_; 
    }
    
    void SetEvents(uint32_t events)
    { 
        events_ = events; 
    }
    uint32_t GetEvents()
    { 
        return events_; 
    }

    void HandleEvent();
    void SetReadHandle(Callback cb)
    {
        readhandler_ = cb; //提高效率，可以使用move语义
    }
    void SetWriteHandle(Callback cb)
    {
        writehandler_ = cb; 
    }    
    void SetErrorHandle(Callback cb)
    { 
        errorhandler_ = cb;
    }
    void SetCloseHandle(Callback cb)
    {
        closehandler_ = cb;
    }

private:
    /* data */
    int fd_;
    uint32_t events_;//epoll events 

    //事件触发时执行的函数，在tcpconn中注册
    Callback readhandler_;
    Callback writehandler_;
    Callback errorhandler_;
    Callback closehandler_;
};



#endif