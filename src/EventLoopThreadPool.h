#pragma once
#include "nocopyable.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>
class EventLoopThread;
class EventLoop;
class EventLoopThreadPool : nocopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;
    EventLoopThreadPool(EventLoop *baseLoop, const std::string &nameArg);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) { numThreads_ = numThreads; }
    void start(const ThreadInitCallback &cb = ThreadInitCallback());

    EventLoop *getNextLoop();

    std::vector<EventLoop *> getAllLoops();

    bool started() { return started_; }

    const std::string name() const { return name_; }

private:
    EventLoop *baseLoop_; // 如果不主动设置线程数量 默认单线程
    std::string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop *> loops_;
};