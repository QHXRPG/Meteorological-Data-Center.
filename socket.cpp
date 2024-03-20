//
// Created by Administrator on 2024/3/20.
//

#include <winsock2.h>
#include "socket.h"
#include "iostream"
#include "unistd.h"
#include "signal.h"
#include "pthread_signal.h"
#include <Ws2tcpip.h>
#include <Winsock2.h>

using namespace std;
#define	SIGPIPE		13	/* Broken pipe.  */
bool tcpClient::connect(const string &ip, const int port) {
    if(mConn != -1)   //判断是否已经连接了服务端
    {
        ::close(mConn);   //关闭已连接的这个socket
        mConn = -1;          //把这个socket置为-1，表示现在这个socket没连接
    }

    // 如果send到一个disconnected socket上，内核就会发出SIGPIPE信号,终止进程,直接屏蔽它。
    signal(SIGPIPE, SIG_IGN);

    mIp = ip;  //获取用户输入的服务端ip
    mPort = port;  //获取用户输入的服务端端口
    struct hostent *h;
    struct sockaddr_in serverAddr;
    if((mConn = socket(AF_INET,SOCK_STREAM,0)) < 0)
        return false;

    if(!(h = gethostbyname(mIp.c_str())))
    {
        ::close(mConn);
        mConn = -1;
        return false;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(mPort);   // 指定服务端的通讯端口
    memcpy(&serverAddr.sin_addr, h->h_addr_list[0], h->h_length);

    if(::connect(mConn, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) !=0)
    {
        ::close(mConn);
        mConn = -1;
        return false;
    }
    return true;
}

bool tcpClient::read(string &buffer, const int timeOut)
{
    if(mConn == -1)
        return false;
    return ((tcpRead(mConn, buffer, timeOut)));
}

bool tcpClient::read(void *buffer, const int bufLen, const int timeOut)
{
    if(mConn == -1)
        return false;
    return (tcpRead(mConn, buffer, bufLen, timeOut));
}

bool tcpClient::write(const string &buffer)
{
    if(mConn == -1)
        return false;
    return (tcpWrite(mConn, buffer));
}

bool tcpClient::write(const void *buffer, const int bufLen) {
    if (mConn==-1) return false;

    return(tcpWrite(mConn, buffer, bufLen));
}

void tcpClient::closeConnnect() {
    if (mConn >= 0)
        ::close(mConn);

    mConn=-1;
    mPort=0;
}

tcpClient::~tcpClient() {
    closeConnnect();
}

bool tcpServer::initServer(const unsigned int port, const int backlog) {
    // 如果服务端的socket>0，关掉它
    if(mListen > 0)
    {
        ::close(mListen);
        mListen = -1;
    }
    if((mListen = socket(AF_INET, SOCK_STREAM, 0)))
        return false;
    // 忽略SIGPIPE信号，防止程序异常退出。
    signal(SIGPIPE, SIG_IGN);

    // 打开SO_REUSEADDR选项，当服务端连接处于TIME_WAIT状态时可以再次启动服务器
    // 否则bind()可能会不成功，报：Address already in use。
    int opt = 1;
    setsockopt(mListen, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char *>(&opt), sizeof(opt));

    memset(&mServrtAddr, 0, sizeof(mServrtAddr));
    mServrtAddr.sin_family = AF_INET;
    mServrtAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 任意ip地址。
    mServrtAddr.sin_port = htons(port);
    if(bind(mListen, (struct sockaddr *) &mServrtAddr, sizeof(mServrtAddr)) != 0)
    {
        closeListen();
        return false;
    }
    if(listen(mListen, backlog) != 0)
    {
        closeListen();
        return false;
    }
    return true;
}

bool tcpServer::accept()
{
    if(mListen == -1)
        return false;
    int mSockLen = sizeof(struct sockaddr_in);
    if((mConn = ::accept(mListen, (struct sockaddr*) &mClientAddr, (socklen_t*) &mSockLen)) < 0)
        return false;
    return true;
}

char *tcpServer::getip()
{
    return(inet_ntoa(mClientAddr.sin_addr));
}

bool tcpServer::read(string &buffer, const int timeOut)
{
    if(mConn == -1)
        return false;
    return (tcpRead(mConn, buffer, timeOut));
}

bool tcpServer::read(void *buffer, const int bufLen, const int timeOut)
{
    if(mConn == -1)
        return false;
    return ((tcpRead(mConn, buffer, bufLen, timeOut)));
}

bool tcpServer::write(const string &buffer) {
    if(mConn == -1)
        return false;
    return ((tcpWrite(mConn,buffer)));
}

bool tcpServer::write(const void *buffer, const int bufLen) {
    if(mConn == -1)
        return false;
    return ((tcpWrite(mConn, buffer, bufLen)));
}

void tcpServer::closeListen()
{
    if(mListen >= 0)
    {
        ::close(mListen);
        mListen = -1;
    }
}

void tcpServer::closeClient()
{
    if(mConn >= 0)
    {
        ::close(mConn);
        mConn = -1;
    }
}

tcpServer::~tcpServer()
{
    closeListen();
    closeClient();
}

bool tcpRead(const int sockfd, string &buffer, const int timeOut)
{
    if(sockfd == -1)
        return false;
    // 如果itimeout>0，表示等待itimeout秒，如果itimeout秒后接收缓冲区中还没有数据，返回false。
    if(timeOut > 0)
    {
        struct pollfd fds;
        fds.fd=sockfd;
        fds.events=POLLIN;
        if ( WSAPoll(&fds,1,timeOut*1000) <= 0 ) return false;
    }

    // 如果itimeout==-1，表示不等待，立即判断socket的接收缓冲区中是否有数据，如果没有，返回false。
    if (timeOut==-1)
    {
        struct pollfd fds;
        fds.fd=sockfd;
        fds.events=POLLIN;
        if ( WSAPoll(&fds,1,0) <= 0 ) return false;
    }

    int bufLen = 0;
    // 先读取报文长度，4个字节。
    if(! readn(sockfd, (char *) &bufLen, 4))
        return false;
    buffer.resize(bufLen); // 把读取到的报头的报文长度传给resize来调整buffer的大小

    //再读取报文内容
    if(! readn(sockfd, &buffer[0], bufLen))
        return false;
    return true;
}

bool tcpRead(const int sockfd, void* buffer, const int bufLen, const int timeOut)
{
    if (sockfd==-1)
        return false;

    // 如果itimeout>0，表示需要等待itimeout秒，如果itimeout秒后还没有数据到达，返回false。
    if (timeOut>0)
    {
        struct pollfd fds;
        fds.fd=sockfd;
        fds.events=POLLIN;
        if ( WSAPoll(&fds,1,timeOut*1000) <= 0 ) return false;
    }

    // 如果itimeout==-1，表示不等待，立即判断socket的缓冲区中是否有数据，如果没有，返回false。
    if (timeOut==-1)
    {
        struct pollfd fds;
        fds.fd=sockfd;
        fds.events=POLLIN;
        if ( WSAPoll(&fds,1,0) <= 0 ) return false;
    }

    //读取报文内容
    if (! readn(sockfd, (char *) buffer, bufLen))
        return false;
    return true;
}

bool tcpWrite(const int sockfd, const string &buffer)
{
    if(sockfd == -1)
        return false;
    int bufLen = buffer.size();

    //先发送报头。
    if(!writen(sockfd, (char *) &bufLen, 4))
        return false;

    // 再发送报文体。
    if(!writen(sockfd, buffer.c_str(), bufLen))
        return false;
    return true;
}

bool tcpWrite(const int sockfd, const void *buffer, const int bufLen)
{
    if (sockfd==-1) return false;

    if (!writen(sockfd, (char *) buffer, bufLen)) return false;

    return true;
}

bool readn(const int sockfd,char *buffer,const size_t n)
{
    int left = n; // 剩余需要读取的字节数。
    int idx = 0;  // 已成功读取的字节数。
    int nread;    // 每次调用recv()函数读到的字节数。

    while(left > 0)
    {
        if((nread = recv(sockfd, buffer + idx, left, 0)) <=0)
            return false;
        idx += nread;
        left -= nread;
    }
    return true;
}

bool writen(const int sockfd,const char *buffer,const size_t n)
{
    int nleft = n;       // 剩余需要写入的字节数。
    int idx = 0;          // 已成功写入的字节数。
    int nwritten;      // 每次调用send()函数写入的字节数。

    while(nleft > 0 )
    {
        if ( (nwritten=send(sockfd,buffer + idx,nleft,0)) <= 0)
            return false;

        nleft = nleft - nwritten;
        idx = idx + nwritten;
    }

    return true;
}






























