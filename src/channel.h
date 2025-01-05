
#pragma once
#include "nocopyable.h"
#include "eventLoop.h"
#include <functional>
#include <memory>
#include "timeStamp.h"
using muduo::TimeStamp;
/**
 * channel 封装了sockfd和它感兴趣的event，如EPOLLIN、EPOLLOUT等 它也知道poller返回的具体发生的事件
 */
class Channel : nocopyable
{
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(TimeStamp)>;

public:
    Channel(EventLoop *loop, int fd);
    ~Channel();
    void handleEventWithGuard(TimeStamp receiveTime);
    void handleEvent(TimeStamp receiveTime);

    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }

    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }
    void tie(const std::shared_ptr<void> &obj);
    int fd() const { return fd_; }
    int events() const { return events_; }

    int set_revents(int revt) { revents_ = revt; }
    void enableReading()
    {
        events_ |= kReadEvent;
        update();
    }

    void disableReading()
    {
        events_ &= ~kReadEvent;
        update();
    }

    void enableWriting()
    {
        events_ |= kWriteEvent;
        update();
    }

    void disableWriting()
    {
        events_ &= ~kWriteEvent;
        update();
    }

    void disableAll()
    {
        events_ = kNoneEvent;
        update();
    }

    void update();

    bool isNoneEvent() const { return events_ == kNoneEvent; }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }
    int index() { return index_; }
    void set_index(int idx) { index_ = idx; }
    EventLoop *ownerLoop() { return loop_; }
    void remove();

private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop *loop_; // 事件循环
    const int fd_;    // fd，poller监听的对象
    int events_;      // 注册fd感兴趣的事件
    int revents_;     // poller返回的具体发生的事件
    int index_;
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

    std::weak_ptr<void> tie_;
    bool tied_;
};