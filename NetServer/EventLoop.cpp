// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//
#include <iostream>
#include "EventLoop.h" 


EventLoop::EventLoop(/* args */)
    : functorlist_(),
    channellist_(),
    activechannellist_(),
    poller_(),
    quit(true)
{
    
}

EventLoop::~EventLoop()
{
    
}

void EventLoop::loop()
{
    quit = false;
    while(!quit)
    {
        poller_.poll(activechannellist_);
        //std::cout << "server HandleEvent" << std::endl;
        for(Channel *pchannel : activechannellist_)
        {            
            pchannel->HandleEvent();//处理事件
        }
        activechannellist_.clear();
        ExecuteTask();
    }
}