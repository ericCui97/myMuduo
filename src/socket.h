#pragma once
#include "nocopyable.h"
class INetAddress;
class Socket : nocopyable
{
private:
    /* data */
public:
    explicit Socket(/* args */ int sockfd) : sockfd_(sockfd) {};
    void bindAddress(const INetAddress &inetAddress);
    void listen();
    int accept(INetAddress *peerAddress);
    void shutdownWrite();
    void SetTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);
    ~Socket();

    int fd(){return sockfd_;}

private:
    int sockfd_;
};
