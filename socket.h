//
// Created by Administrator on 2024/3/20.
//

#ifndef PROJECT1_SOCKET_H
#define PROJECT1_SOCKET_H

#endif //PROJECT1_SOCKET_H

#include <winsock.h>
#include "string"
#include "iostream"
#include "fstream"
using namespace std;

//客户端
class tcpClient
{
private:
    int mConn;   //客户端的socket
    string mIp;   //服务端的ip地址
    int mPort;   // 服务端通讯的端口
public:
    tcpClient()   //构造函数
    :mConn(-1),
    mPort(0)
    {}
    bool connect(const string &ip, const int port);  // 向服务端发起连接请求。
    bool read(string &buffer, const int timeOut=0);
    bool read(void *buffer, const int bufLen, const int timeOut=0);
    bool write(const string &buffer);
    bool write(const void* buffer, const int bufLen);
    void closeConnnect();
    ~tcpClient();
};

//服务端
class tcpServer
{
private:
    int mSocklen;                // 结构体struct sockaddr_in的大小
    struct sockaddr_in mClientAddr;   // 客户端的地址信息。
    struct sockaddr_in mServrtAddr;   // 服务端的地址信息。
    int mListen;                // 服务端用于监听的socket。
    int mConn;                  // 客户端连接上来的socket。
public:
    tcpServer()   //构造函数
    :mListen(-1),
     mConn(-1)
    {}
    bool initServer(const unsigned int port, const int backlog=5);
    bool accept();
    char *getip();
    bool read(string &buffer, const int timeOut=0);
    bool read(void *buffer, const int bufLen, const int timeOut=0);
    bool write(const string &buffer);
    bool write(const void* buffer, const int bufLen);
    void closeListen();
    void closeClient();
    ~tcpServer();
};

bool tcpRead(const int sockfd, string &buffer, const int timeOut=0);
bool tcpRead(const int sockfd, void* buffer, const int bufLen, const int timeOut=0);
bool tcpWrite(const int sockfd, const string &buffer);
bool tcpWrite(const int sockfd, const void *buffer, const int bufLen);

// 从已经准备好的socket中读取数据。
// sockfd：已经准备好的socket连接。
// buffer：存放数据的地址。
// n：本次打算读取数据的字节数。
// 返回值：成功接收到n字节的数据后返回true，socket连接不可用返回false。
bool readn(const int sockfd,char *buffer,const size_t n);

// 向已经准备好的socket中写入数据。
// sockfd：已经准备好的socket连接。
// buffer：待写入数据的地址。
// n：待写入数据的字节数。
// 返回值：成功写入完n字节的数据后返回true，socket连接不可用返回false。
bool writen(const int sockfd,const char *buffer,const size_t n);




















