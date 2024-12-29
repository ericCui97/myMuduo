//
// Created by jhlod cui on 2024/12/29.
//

#include <unistd.h>
#include "Thread.h"

namespace muduo {
    namespace detail {
        pid_t gettid() {
//            return static_cast<pid_t>(::syscall(SYS_gettid));
        }
    }
}