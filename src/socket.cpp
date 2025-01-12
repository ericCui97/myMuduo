#include "socket.h"
#include "INetAddress.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

Socket::~Socket()
{
    close(sockfd_);
}

void Socket::bindAddress(const INetAddress &localaddr)
{
    if (0 != bind(sockfd_, (sockaddr *)&localaddr.getSockAddr(), sizeof(sockaddr_in)))
    {
        printf("bind fail\n");
        exit(-1);
    }
}

void Socket::listen()
{
    if (0 != ::listen(sockfd_, 1024))
    {
        printf("listen fail\n");
        exit(-1);
    }
}

int Socket::accept(INetAddress *peeraddr)
{
    sockaddr_in addr;
    socklen_t len;
    bzero(&addr, sizeof addr);
    int connfd = ::accept(sockfd_, (sockaddr *)&addr, &len);
    if (connfd >= 0)
    {
        peeraddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::shutdownWrite()
{
    if (0 != ::shutdown(sockfd_, SHUT_WR))
    {
        printf("shutdown fail\n");
        exit(-1);
    }
}

void Socket::SetTcpNoDelay(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof optval);
}
void Socket::setReusePort(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof optval);
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
}

void Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof optval);
}