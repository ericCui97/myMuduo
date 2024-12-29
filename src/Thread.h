//
// Created by jhlod cui on 2024/12/29.
//

#ifndef OSMUDUO_THREAD_H
#define OSMUDUO_THREAD_H

#include<functional>
#include<memory>
#include<pthread.h>
#include<string>
#include"nocopyable.h"
#include"atomic.h"

namespace muduo {
    class Thread : nocopyable {

        typedef std::function<void()> ThreadFunc;

        explicit Thread(ThreadFunc, const std::string &name = std::string());

        ~Thread();

        void start();

        void join();

        bool started() const { return started_; }

        pid_t getPid() const { return tid_; }

        const std::string &name() const { return name_; }

        static int numCreated() { return numCreated_.get(); }

    private:
        void setDefaultName();

        bool started_;
        bool joined_;
        pthread_t pthreadId_;
        pid_t tid_;
        ThreadFunc func_;
        std::string name_;
        CountDownLatch latch_;
        static AtomicInt32 numCreated_;

    };
}


#endif //OSMUDUO_THREAD_H
