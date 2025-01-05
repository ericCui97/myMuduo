#include "channel.h"
#include <sys/epoll.h>
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI; // 可读事件
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false) {};

Channel::~Channel() {};

void Channel::tie(const std::shared_ptr<void> &obj)
{
    tie_ = obj;
    tied_ = true;
}

// 当fd上的事件发生时，调用handleEvent update在poller里面更改对应的事件的epoll_ctl
void Channel::update()
{

    // TODO
    // loop_->updateChannel(this);
}

void Channel::handleEvent(TimeStamp receiveTime)
{
    // 如果绑定了对象，确保channel绑定的对象还存在，
    std::shared_ptr<void> guard;
    if (tied_)
    {
        guard = tie_.lock();
        if (guard)
        {
            handleEventWithGuard(receiveTime);
        }
        else
        {
            printf("Channel::handleEvent,guard is null\n");
        }
    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::handleEventWithGuard(TimeStamp receiveTime)
{
    printf("Channel::handleEventWithGuard,revents:%d\n", revents_);
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if (closeCallback_)
        {
            closeCallback_();
        }
    }

    if (revents_ & EPOLLERR)
    {
        if (errorCallback_)
        {
            /* code */
            errorCallback_();
        }

        /* code */
    }
    if (revents_ & (EPOLLIN | EPOLLPRI))
    {
        if (readCallback_)
        {
            readCallback_(receiveTime);
        }
    }
}