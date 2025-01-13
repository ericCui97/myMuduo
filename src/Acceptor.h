#pragma once
#include <functional>

#include "channel.h"
#include "INetAddress.h"
#include "nocopyable.h"
#include "socket.h"
class EventLoop;

class INetAddress;

class Acceptor:nocopyable {
public:
    using NewConnectionCallback = std::function<void(int sockfd,const INetAddress&)>;
    Acceptor(EventLoop* loop,const INetAddress& listenAddr,bool reuseport);
    ~Acceptor();
    void listen();
    void setNewConnectionCallback(const NewConnectionCallback& cb) {
        newConnectionCallback_ = cb;
    }
    bool listening() const {
        return listening_;
    }
private:
    EventLoop* loop_;
    Socket acceptSocket_;
    NewConnectionCallback newConnectionCallback_;
    Channel acceptChannel_;
    bool listening_;
    void handleRead();
};
