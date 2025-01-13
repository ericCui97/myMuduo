#include "Acceptor.h"
#include "EventLoop.h"
#include "Channel.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

static int createNonBlocking() {
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (sockfd < 0) {
        printf("%s:%s:%d:listen socket create err:%d\n",__FILE__, __FUNCTION__,__LINE__,errno);
        exit(-1);
    }

    return sockfd;
}


Acceptor::Acceptor(EventLoop *loop, const INetAddress &listenAddr, bool reuseport)
    : loop_(loop)
      , acceptSocket_(createNonBlocking()) // socket
      , acceptChannel_(loop, acceptSocket_.fd())
      , listening_(false) {
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(true);
    acceptSocket_.bindAddress(listenAddr); // bind
    // TcpServer::start() Acceptor.listen  有新用户的连接，要执行一个回调（connfd=》channel=》subloop）
    // baseLoop => acceptChannel_(listenfd) =>
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::handleRead() {
    INetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if (connfd < 0) {
        printf("%s:%s:%d:accept err:%d\n",__FILE__, __FUNCTION__,__LINE__,errno);
    }else {
        if (newConnectionCallback_) {
            newConnectionCallback_(connfd,peerAddr);
        }else {
            ::close(connfd);
        }
    }
}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
    acceptChannel_.remove();
}

void Acceptor::listen() {
    listening_ = true;
    acceptSocket_.listen();
    // 设置为可读
    acceptChannel_.enableReading();
}
