
#pragma once
#include "nocopyable.h"
#include "timeStamp.h"
#include <vector>
#include <unordered_map>
#include <memory>
// base class for io multiplexing
// this class doesnt own the channel
class Channel;
class EventLoop;

class Poller : nocopyable
{
private:
    /* data */
public:
    using ChannelList = std::vector<Channel *>;
    // Poller(/* args */);
    Poller(EventLoop *loop);

    // ，当你有一个基类指针指向一个派生类对象时，如果基类的析构函数不是虚函数，那么在删除这个指针时，只会调用基类的析构函数，而不会调用派生类的析构函数。这可能会导致派生类中分配的资源（如动态内存）没有被正确释放，从而造成内存泄漏。
    virtual ~Poller();
    // 纯虚函数
    virtual muduo::TimeStamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
    virtual void updateChannel(Channel *channel) = 0;
    virtual void removeChannel(Channel *channel) = 0;
    // channel在poller中是否存在
    bool hasChannel(Channel *channel) const;
    static Poller *newDefaultPoller(EventLoop *loop);
    void assertInLoopThread() const {
        // ownerLoop_->assertInLoopThread();
    };

protected:
    using ChannelMap = std::unordered_map<int, Channel *>;
    ChannelMap channels_;

private:
    EventLoop *ownerLoop_;
};
