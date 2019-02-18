// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
//main

#include <signal.h>
#include "EventLoop.h"
//#include "TcpServer.h"
#include "HttpServer.h"
#include "EchoServer.h"

//gprof
static void sighandler( int sig_no )   
{   
      exit(0);   
}   

int main()
{
    signal( SIGUSR1, sighandler );
    signal(SIGPIPE, SIG_IGN);  //SIG_IGN,系统函数，忽略信号的处理程序,客户端发送RST包后，服务器还调用write会触发

    // EventLoop loop;
    // EchoServer echoserver(&loop, 8088);
    // echoserver.Start();
    // loop.loop();

    EventLoop loop;
    HttpServer httpserver(&loop, 8088);
    httpserver.Start();
    loop.loop();
    return 0;
}