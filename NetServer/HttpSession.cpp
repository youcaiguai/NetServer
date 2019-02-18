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

// 200：请求被正常处理 
// 204：请求被受理但没有资源可以返回 
// 206：客户端只是请求资源的一部分，服务器只对请求的部分资源执行GET方法，相应报文中通过Content-Range指定范围的资源。

// 301：永久性重定向 
// 302：临时重定向 
// 303：与302状态码有相似功能，只是它希望客户端在请求一个URI的时候，能通过GET方法重定向到另一个URI上 
// 304：发送附带条件的请求时，条件不满足时返回，与重定向无关 
// 307：临时重定向，与302类似，只是强制要求使用POST方法
 
// 400：请求报文语法有误，服务器无法识别
// 401：请求需要认证 
// 403：请求的对应资源禁止被访问 
// 404：服务器无法找到对应资源 

// 500：服务器内部错误 
// 503：服务器正忙

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "HttpSession.h"

HttpSession::HttpSession()
{

}

HttpSession::~HttpSession()
{

}

void HttpSession::PraseHttpRequest(std::string &s)
{
    //std::cout << "HttpServer::HttpParser" << std::endl;
    std::string msg;
    msg.swap(s);
	std::string crlf("\r\n"), crlfcrlf("\r\n\r\n");
	size_t prev = 0, next = 0, pos_colon;
	std::string key, value;

    //以下解析可以改成状态机，解决一次收Http报文不完整问题
	//prase http request line
	if ((next = msg.find(crlf, prev)) != std::string::npos)
	{
		std::string first_line(msg.substr(prev, next - prev));
		prev = next;
		std::stringstream sstream(first_line);
		sstream >> (httprequestcontext_.method);
		sstream >> (httprequestcontext_.url);
		sstream >> (httprequestcontext_.version);
	}
	else
	{
		std::cout << "Error in httpPraser: http_request_line isn't complete!" << std::endl;
        //可以临时存起来，凑齐了再解析
	}
    //prase http request header
	size_t pos_crlfcrlf = 0;
	if (( pos_crlfcrlf = msg.find(crlfcrlf, prev)) != std::string::npos)
	{
		while (prev != pos_crlfcrlf)
        {
            next = msg.find(crlf, prev + 2);
            pos_colon = msg.find(":", prev + 2);
            key = msg.substr(prev + 2, pos_colon - prev-2);
            value = msg.substr(pos_colon + 2, next-pos_colon-2);
            prev = next;
            httprequestcontext_.header.insert(std::pair<std::string, std::string>(key, value));
        }
	}
    else
    {
        std::cout << "Error in httpPraser: http_request_header isn't complete!" << std::endl;
    }	
    //prase http request body
	httprequestcontext_.body = msg.substr(pos_crlfcrlf + 4);
}

void HttpSession::HttpProcess()
{
    if("GET" == httprequestcontext_.method)
    {
        ;
    }
    else if("POST" == httprequestcontext_.method)
    {
        ;
    }
    else
    {
        std::cout << "HttpServer::HttpParser" << std::endl;
        errormsg = "Method Not Implemented";
        HttpError(501, "Method Not Implemented");
    }

    size_t pos = httprequestcontext_.url.find("?");
    if(pos != std::string::npos)
    {
        path_ = httprequestcontext_.url.substr(0, pos);
        querystring_ = httprequestcontext_.url.substr(pos+1);
    }
    else
    {
        path_ = httprequestcontext_.url;
    }
    
    //keepalive判断处理
    std::map<std::string, std::string>::const_iterator iter = httprequestcontext_.header.find("Connection");
    if(iter != httprequestcontext_.header.end())
    {
        keepalive_ = (iter->second == "Keep-Alive");
    }
    else
    {
        if(httprequestcontext_.version == "HTTP/1.1")
        {
            keepalive_ = true;//HTTP/1.1默认长连接
        }
        else
        {
            keepalive_ = false;//HTTP/1.0默认短连接
        }            
    }

    responsebody_.clear();
    if("/" == path_)
    {        
        path_ = "/index.html";
    }
    else if("/hello" == path_)
    {
        //Wenbbench 测试用
        std::string filetype("text/html");
        responsebody_ = ("hello world");
        responsecontext_ += httprequestcontext_.version + " 200 OK\r\n";
        responsecontext_ += "Server: Chen Shuaihao's NetServer/0.1\r\n";
        responsecontext_ += "Content-Type: " + filetype + "; charset=utf-8\r\n";
        if(iter != httprequestcontext_.header.end())
        {
            responsecontext_ += "Connection: " + iter->second + "\r\n";
        }
        responsecontext_ += "Content-Length: " + std::to_string(responsebody_.size()) + "\r\n";
        responsecontext_ += "\r\n";
        responsecontext_ += responsebody_;
        return;
    }
    else
    {
        ;
    }    

    //std::string responsebody;    
    path_.insert(0,".");
    FILE* fp = NULL;
    if((fp = fopen(path_.c_str(), "rb")) == NULL)
    {
        //perror("error fopen");
        //404 NOT FOUND
        HttpError(404, "Not Found");
        return;
    }
    else
    {
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));
        while(fread(buffer, sizeof(buffer), 1, fp) == 1)
        {
            responsebody_.append(buffer);
            memset(buffer, 0, sizeof(buffer));
        }
        if(feof(fp))
        {
            responsebody_.append(buffer);
        }        
        else
        {
            std::cout << "error fread" << std::endl;
        }        	
        fclose(fp);
    }

    std::string filetype("text/html"); //暂时固定为html
    responsecontext_ += httprequestcontext_.version + " 200 OK\r\n";
    responsecontext_ += "Server: Chen Shuaihao's NetServer/0.1\r\n";
    responsecontext_ += "Content-Type: " + filetype + "; charset=utf-8\r\n";
    if(iter != httprequestcontext_.header.end())
    {
        responsecontext_ += "Connection: " + iter->second + "\r\n";
    }
    responsecontext_ += "Content-Length: " + std::to_string(responsebody_.size()) + "\r\n";
    responsecontext_ += "\r\n";
    responsecontext_ += responsebody_;    
}

void HttpSession::AddToBuf(std::string &s)
{
    responsecontext_.swap(s);
}

void HttpSession::HttpError(int err_num, std::string short_msg)
{
    //这里string创建销毁应该会耗时间
    //std::string body_buff;
    responsebody_ += "<html><title>出错了</title>";
    responsebody_ += "<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>";
    responsebody_ += "<style>body{background-color:#f;font-size:14px;}h1{font-size:60px;color:#eeetext-align:center;padding-top:30px;font-weight:normal;}</style>";
    responsebody_ += "<body bgcolor=\"ffffff\"><h1>";
    responsebody_ += std::to_string(err_num) + " " + short_msg;
    responsebody_ += "</h1><hr><em> Chen Shuaihao's NetServer</em>\n</body></html>";

    responsecontext_ += httprequestcontext_.version + " " + std::to_string(err_num) + " " + short_msg + "\r\n";
    responsecontext_ += "Server: Chen Shuaihao's NetServer/0.1\r\n";
    responsecontext_ += "Content-Type: text/html\r\n";
    responsecontext_ += "Connection: Keep-Alive\r\n";
    responsecontext_ += "Content-Length: " + std::to_string(responsebody_.size()) + "\r\n";
    responsecontext_ += "\r\n";
    responsecontext_ += responsebody_;
}

