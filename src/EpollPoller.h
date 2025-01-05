#pragma once
#include "poller.h"

#include <sys/epoll.h>
using muduo::TimeStamp;
/**
 * epoll
 * epoll_create
 * epoll_ctl
 * epoll_wait
 */
class EpollPoller : public Poller
{
private:
    /* data */
    static const int KInitEventListSize = 16;
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
    void update(int operation, Channel *channel);
    using EventList = std::vector<epoll_event>;

    int epollfd_;
    EventList events_;

public:
    EpollPoller(EventLoop *loop);
    ~EpollPoller() override;

    TimeStamp poll(int timeoutMs, ChannelList *activeChannels) override;
    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;
};