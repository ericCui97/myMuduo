#pragma once
#include "nocopyable.h"
#include "Thread.h"
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>
class EventLoop;

class EventLoopThread : nocopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;
    EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(),
                    const std::string &name = std::string());

    ~EventLoopThread();

    EventLoop *startLoop();

private:
    EventLoop *loop_;
    bool exiting_;
    std::mutex mutex_;
    Thread thread_;
    std::condition_variable cond_;
    void threadFunc();
    ThreadInitCallback callback_;
};