//
// Created by cuiyirong on 2024/12/23.
//

#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

class nocopyable {
public:
    nocopyable() = default;

    ~nocopyable() = default;

    nocopyable(const nocopyable &) = delete;

    nocopyable &operator=(const nocopyable &) = delete;
};
#endif //NOCOPYABLE_H
