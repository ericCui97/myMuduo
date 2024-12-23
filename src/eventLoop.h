//
// Created by cuiyirong on 2024/12/23.
//
#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include "nocopyable.h"


class eventLoop : nocopyable {
public:
    explicit eventLoop();

    ~eventLoop();

    explicit eventLoop(const eventLoop &) = delete;

    void loop();

    void quit();

    [[nodiscard]] int64_t iteration() const { return iteration_; }

    // void runInLoop();


private:
    int64_t iteration_;
};


#endif //EVENTLOOP_H
