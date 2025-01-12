#include "EventLoop.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/eventfd.h>
#include "channel.h"
#include "poller.h"

// 防止一个线程创建多个eventloop

__thread EventLoop *t_loopInThisThread = nullptr;

const int kPollTimeMs = 10000;
// 创建wake up fd 用于唤醒subloop处理新来的channel
int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        printf("failed in eventfd");
        exit(-1);
    }

    return evtfd;
}

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      callingPendingFunctors_(false),
      threadId_(CurrentThread::tid()),
      poller_(Poller::newDefaultPoller(this)),
      wakeupFd_(createEventfd()),
      wakeupChannel_(new Channel(this, wakeupFd_)),
      currentActiveChannel_(nullptr)
{
    printf("eventLoop created %p in thread %d", this, threadId_);
    if (t_loopInThisThread)
    {
        printf("[error],another loop in this thread\n");
        exit(-1);
    }
    else
    {
        t_loopInThisThread = this;
    }

    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    // 每一个eventloop都将监听wakeupchannel的EPOLLIN读事件
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = nullptr;
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_, &one, sizeof one);
    if (n != sizeof one)
    {
        printf("EventLoop::handleRead() reads %lu bytes instead of 8\n", n);
    }
}

void EventLoop::loop()
{
    looping_ = true;
    quit_ = false;
    printf("eventLoop %p start looping\n", this);
    // 轮询
    while (!quit_)
    {
        activeChannels_.clear();
        // 监听两种fd 1. client的fd， 一种wakeup fd
        pollReturnTime_ = poller_->poll(
            kPollTimeMs, &activeChannels_);

        for (Channel *channel : activeChannels_)
        {
            channel->handleEvent(pollReturnTime_);
        }

        doPendingFunctors();
    }
}

void EventLoop::quit()
{

    quit_ = true;
    // 如果在其他线程中调用quit，在一个subloop中，调用了mainLoop的quit
    if (!isInLoopThread())
    {

        wakeup();
    }
}
void EventLoop::runInLoop(Functor cb)
{

    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        // 非当前loop线程中，唤醒loop所在线程，执行cb
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(Functor cb)
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        pendingFunctors_.emplace_back(cb);
    }
    // 唤醒相应的执行相应的回调对应的线程的loop
    // || callingPendingFunctors_的意思是当前loop正在执行回调，但是loop又有了新的回调
    if (!isInLoopThread() || callingPendingFunctors_)
    {
        /* code */
        wakeup();
    }
}
// 唤醒loop所在的线程  向wakeupfd_写一个数据 wakeupchannel就发生读事件，当前loop线程就会被唤醒

void EventLoop::wakeup()
{

    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof one);
    if (n != sizeof one)
    {
        printf("EventLoop::wakeup() writes %lu bytes instead of 8\n", n);
    }
}

void EventLoop::updateChannel(Channel *channel)
{
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel)
{
    return poller_->hasChannel(channel);
}

//  执行回调队列
void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        // 不影响 pendingFunctors_ 注册新的回调
        functors.swap(pendingFunctors_);
    }

    for (const Functor &functor : functors)
    {
        functor();
    }

    callingPendingFunctors_ = false;
}