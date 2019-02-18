// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//Poller类，对epoll的封装

#ifndef _POLLER_H_
#define _POLLER_H_

#include <vector>
#include <map>
#include <sys/epoll.h>
#include "Channel.h"

class Poller
{
public:
    typedef std::vector<Channel*> ChannelList;
    
    int pollfd_;
    std::vector<struct epoll_event> eventlist_;
    std::map<int, Channel*> channelmap_;

    Poller(/* args */);
    ~Poller();

    void poll(ChannelList &activechannellist);
    void AddChannel(Channel *pchannel);
    void RemoveChannel(Channel *pchannel);
    void UpdateChannel(Channel *pchannel);
private:
    /* data */
    //int pollfd_;    
    
};


#endif