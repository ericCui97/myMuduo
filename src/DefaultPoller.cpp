#include "poller.h"

#include <stdlib.h>

Poller *Poller::newDefaultPoller(EventLoop *loop)
{
    if (::getenv("MUDUO_USE_POLL"))
    {
        return nullptr; // 生成poll对象
    }

    else
    {
        return nullptr; // 生成epoll对象
    }
}