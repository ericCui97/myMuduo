#include "EpollPoller.h"
#include <unistd.h>
#include "channel.h"
#include <string.h>
const int KNew = -1;
const int KAdded = 1;
const int KDeleted = 2;

/**
 * KNew：表示 Channel 对象是新创建的，尚未添加到 EpollPoller 中。
KAdded：表示 Channel 对象已经添加到 EpollPoller 中。
KDeleted：表示 Channel 对象已经从 EpollPoller 中删除。
 */

EpollPoller::EpollPoller(EventLoop *loop)
    : Poller(loop),
      epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
      events_(KInitEventListSize)
{
    if (epollfd_ < 0)
    {
        printf("epoll_create error:%d \n", errno);
        exit(EXIT_FAILURE);
    }
}

EpollPoller::~EpollPoller()
{
    ::close(epollfd_);
}

/**
 * channel.update  channel.remove => eventLoop.updateChannel removeChannel => poller updateChannel removeChannel
 */

TimeStamp EpollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
    printf("func=%s=>fd total count:%lu\n", __FUNCTION__, channels_.size());
    int numEvents = ::epoll_wait(epollfd_,
                                 &*events_.begin(),
                                 static_cast<int>(events_.size()),
                                 timeoutMs);
    int saveErrno = errno;
    muduo::TimeStamp now(TimeStamp::now());
    if (numEvents > 0)
    {
        printf("%s event happen numEvents=%d\n", __FUNCTION__, numEvents);
        fillActiveChannels(numEvents, activeChannels);
        if (numEvents == events_.size())
        {
            events_.resize(events_.size() * 2);
        }
        return now;
    }
    else if (numEvents == 0)
    {
        printf("%s timeout!\n", __FUNCTION__);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (saveErrno != EINTR)
        {
            errno = saveErrno;
            printf("EpollPoller::poll() err!\n");
        }
        exit(EXIT_FAILURE);
    }
}

void EpollPoller::updateChannel(Channel *channel)
{
    const int index = channel->index();
    printf("fd=%d, events=%d, index=%d\n", channel->fd(), channel->events(), index);
    if (index == KNew || index == KDeleted)
    {
        if (index == KNew)
        {
            /* code */
            int fd = channel->fd();
            channels_[fd] = channel;
        }

        channel->set_index(KAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        printf("epollpoller::updateChannel fd=%d, events=%d, index=%d\n",
               channel->fd(), channel->events(), index);
        if (channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->set_index(KDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void EpollPoller::removeChannel(Channel *channel)
{
    Poller::assertInLoopThread();
    int fd = channel->fd();
    printf("epollpoller::removeChannel fd=%d\n", fd);
    int index = channel->index();
    printf("epollpoller::removeChannel fd=%d, events=%d, index=%d\n",
           channel->fd(), channel->events(), index);
    size_t n = channels_.erase(fd);
    (void)n;
    if (index == KAdded)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->set_index(KNew);
}

void EpollPoller::update(int operation, Channel *channel)
{
    epoll_event event;
    memset(&event, 0, sizeof event);
    int fd = channel->fd();
    event.events = channel->events();
    event.data.ptr = channel;
    event.data.fd = fd;
    if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
    {
        printf("epoll_ctl error:%d\n", errno);
        if (operation == EPOLL_CTL_DEL)
        {
            printf("epoll_ctl del error:%d\n", errno);
        }
        else
        {
            exit(EXIT_FAILURE);
        }
    }
}

void EpollPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
    for (size_t i = 0; i < numEvents; i++)
    {
        Channel *channel = static_cast<Channel *>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel);
    }
}