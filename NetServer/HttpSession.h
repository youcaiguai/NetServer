// Copyright 2019, Chen Shuaihao.
//
//Author: Chen Shuaihao
//
// GET /register.do?p={%22username%22:%20%2213917043329%22,%20%22nickname%22:%20%22balloon%22,%20%22password%22:%20%22123%22} HTTP/1.1\r\n
// GET / HTTP/1.1
// Host: bigquant.com
// Connection: keep-alive
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8
// Accept-Encoding: gzip, deflate, br
// Accept-Language: zh-CN,zh;q=0.9,en;q=0.8
// Cookie: _bdid_=059a16ee3bef488b9d5212c81e2b688d; Hm_lvt_c58f67ca105d070ca7563b4b14210980=1550223017; _ga=GA1.2.265126182.1550223018; _gid=GA1.2.1797252688.1550223018; Hm_lpvt_c58f67ca105d070ca7563b4b14210980=1550223213; _gat_gtag_UA_124915922_1=1

// HTTP/1.1 200 OK
// Server: nginx/1.13.12
// Date: Fri, 15 Feb 2019 09:57:21 GMT
// Content-Type: text/html; charset=utf-8
// Transfer-Encoding: chunked
// Connection: keep-alive
// Vary: Accept-Encoding
// Vary: Cookie
// X-Frame-Options: SAMEORIGIN
// Set-Cookie: __bqusername=""; Domain=.bigquant.com; expires=Thu, 01-Jan-1970 00:00:00 GMT; Max-Age=0; Path=/
// Access-Control-Allow-Origin: *
// Content-Encoding: gzip

#ifndef _HTTP_SESSION_H_
#define _HTTP_SESSION_H_

#include <string>
#include <sstream>
#include <map>
//#include "TcpConnection.h"

typedef struct _HttpRequestContext {
	std::string method;
	std::string url;
	std::string version;
	std::map<std::string, std::string> header;
	std::string body;
}HttpRequestContext;

typedef struct _HttpResponseContext {
    std::string version;
    std::string statecode;
    std::string statemsg;
	std::map<std::string, std::string> header;
	std::string body;
}HttpResponseContext;

class HttpSession
{
private:
    /* data */
    HttpRequestContext httprequestcontext_;
    //Http响应报文
    std::string responsecontext_;
    std::string responsebody_;    
    std::string errormsg;
    std::string path_;
    std::string querystring_;
    bool keepalive_;
    std::string body_buff;
public:
    //HttpSession(TcpConnection *ptcpconn);
    HttpSession();
    ~HttpSession();

    void PraseHttpRequest(std::string &s);
    void HttpProcess();
    void AddToBuf(std::string &s);
    void HttpError(int err_num, std::string short_msg);
    bool KeepAlive()
    { return keepalive_;}
};

#endif
