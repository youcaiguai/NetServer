// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include "Socket.h"

Socket::Socket(/* args */)
{
    serverfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == serverfd_)
    {
        perror("socket create fail!");
        exit(-1);
    }
    std::cout << "server create socket" << serverfd_ << std::endl;
}

Socket::~Socket()
{
    close(serverfd_);
    std::cout << "server close..." << std::endl;
}

void Socket::SetSocketOption()
{
    ;
}

void Socket::SetReuseAddr()
{
    int on = 1;
    setsockopt(serverfd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
}

void Socket::Setnonblocking()
{
    int opts = fcntl(serverfd_, F_GETFL);
    if (opts<0)
    {
        perror("fcntl(serverfd_,GETFL)");
        exit(1);
    }
    if (fcntl(serverfd_, F_SETFL, opts | O_NONBLOCK) < 0)
    {
        perror("fcntl(serverfd_,SETFL,opts)");
        exit(1);
    }
    std::cout << "server setnonblocking..." << std::endl;
}

bool Socket::BindAddress(int serverport)
{
    struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(_ServerIP.c_str());
	serveraddr.sin_port = htons(serverport);
	int resval = bind(serverfd_, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (resval == -1)
	{
		close(serverfd_);
		perror("error bind");
		exit(1);
	}
    std::cout << "server bindaddress..." << std::endl;
    return true;
}

bool Socket::Listen()
{    
	if (listen(serverfd_, 2048) < 0)
	{
		perror("error listen");
		close(serverfd_);
		exit(1);
	}
    std::cout << "server listenning..." << std::endl;
    return true;
}

int Socket::Accept(struct sockaddr_in &clientaddr)
{
    socklen_t lengthofsockaddr = sizeof(clientaddr);
    int clientfd = accept(serverfd_, (struct sockaddr*)&clientaddr, &lengthofsockaddr);
    if (clientfd < 0) 
    {
        //perror("error accept");
        //if(errno == EAGAIN)
            //return 0;
		//std::cout << "error accept:there is no new connection accept..." << std::endl;
        return clientfd;
	}
    //std::cout << "server accept,clientfd: " << clientfd << std::endl;
    return clientfd;
}

bool Socket::Close()
{
    close(serverfd_);
    std::cout << "server close..." << std::endl;
    return true;
}

