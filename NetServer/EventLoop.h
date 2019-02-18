// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//

#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <functional>
#include <vector>
#include "Poller.h"
#include "Channel.h"
 
class EventLoop /*nocopyable*/
{
public:
    typedef std::function<void()> Functor;
    typedef std::vector<Channel*> ChannelList;  
    EventLoop(/* args */);
    ~EventLoop();

    void loop();
    void AddChannelToPoller(Channel *pchannel)
    {
        poller_.AddChannel(pchannel);
    }
    void RemoveChannelToPoller(Channel *pchannel)
    {
        poller_.RemoveChannel(pchannel);
    }
    void UpdateChannelToPoller(Channel *pchannel)
    {
        poller_.UpdateChannel(pchannel);
    }

    //Task
    void AddTask(Functor functor)
    {
        functorlist_.push_back(functor);
    }
    void ExecuteTask()
    {
        for(Functor &functor : functorlist_)
        {
            functor();
        }
        functorlist_.clear();
    }

private:
    /* data */    
    std::vector<Functor> functorlist_;
    ChannelList channellist_;
    ChannelList activechannellist_;
    Poller poller_;
    bool quit;
};


#endif