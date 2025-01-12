//
// Created by jhlod cui on 2024/12/29.
//

#include <unistd.h>
#include "Thread.h"
#include "CurrentThread.h"
#include <semaphore.h>

Thread::Thread(
    ThreadFunc func,
    const std::string &name) : started_(false),
                               joined_(false),
                               tid_(0),
                               func_(std::move(func)),
                               name_(name) {}

Thread::~Thread()
{
    if (started_ && !joined_)
    {
        thread_->detach();
    }
}

void Thread::start()
{
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);

    thread_ = std::shared_ptr<std::thread>(
        new std::thread([&]()
                        {
            tid_ = CurrentThread::tid();
            sem_post(&sem);
         
            func_(); }));
    // 用信号量保证 tid 初始化完成 之后再结束
    sem_wait(&sem);
}

void Thread::join()
{
    joined_ = true;
    thread_->join();
}
