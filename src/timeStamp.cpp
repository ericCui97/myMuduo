;
//
// Created by cuiyirong on 2024/12/24.
//

#include "timeStamp.h"
#include <sys/time.h>
#include<cinttypes>

using namespace muduo;

static_assert(sizeof(TimeStamp) == sizeof(int64_t), "timeStamp should be same size as int64_t");

std::string TimeStamp::toString() const {
    char buf[32] = {0};
    const int64_t seconds = msSinceEpoch_ / kMsPerSecond;
    const int64_t microseconds = msSinceEpoch_ % kMsPerSecond;
    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

std::string TimeStamp::toFormatString(bool showMicroseconds) const {
    char buf[64] = {0};
    auto seconds = static_cast<time_t>(msSinceEpoch_ / kMsPerSecond);
    struct tm tm_time{};
    gmtime_r(&seconds, &tm_time);

    if (showMicroseconds) {
        int microseconds = static_cast<int>(msSinceEpoch_ % kMsPerSecond);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                 microseconds);
    } else {
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }
    return buf;
}

TimeStamp TimeStamp::now() {
    struct timeval tv{};
    gettimeofday(&tv,nullptr);
    int64_t seconds = tv.tv_sec;
    return TimeStamp(seconds * kMsPerSecond);
}
