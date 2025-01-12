
#pragma once

#include "nocopyable.h"
#include <functional>
#include <vector>
#include <atomic>
#include <unistd.h>
#include "timeStamp.h"
#include <memory>
#include "CurrentThread.h"
#include <mutex>
class Channel;
class Poller;

class EventLoop : nocopyable
{

public:
    using Functor = std::function<void()>;
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    muduo::TimeStamp pollReturnTime() const { return pollReturnTime_; }

    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);

    void wakeup();
    // eventloop —》 poller -> channel
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);

    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

private:
    using ChannelList = std::vector<Channel *>;
    std::atomic_bool looping_;                // 是否在循环
    std::atomic_bool quit_;                   // 是否退出循环
    std::atomic_bool callingPendingFunctors_; // 是否正在执行回调函数
    const pid_t threadId_;                    // loop 所在线程的id

    muduo::TimeStamp pollReturnTime_; // poller返回发生事件的channels的时间点
                                      // 当mainLoop获取一个新用户的channel，
                                      // 通过轮询算法选择一个subLoop，通过该成员唤醒subLoop处理channel
    int wakeupFd_;
    std::unique_ptr<Poller> poller_;

    std::unique_ptr<Channel> wakeupChannel_;

    ChannelList activeChannels_;
    Channel *currentActiveChannel_;
    std::vector<Functor> pendingFunctors_; // 存储loop所需要的所有回调操作
    void handleRead();                     // wake up
    void doPendingFunctors();
    std::mutex mutex_;
};
