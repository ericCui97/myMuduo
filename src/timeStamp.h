//
// Created by cuiyirong on 2024/12/24.
//

#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#include <cstdint>
#include <string>

namespace muduo
{
    class TimeStamp
    {
    public:
        TimeStamp() : msSinceEpoch_(0)
        {
        }

        explicit TimeStamp(const int64_t msSinceEpoch) : msSinceEpoch_(msSinceEpoch)
        {
        }

        TimeStamp(const TimeStamp &another)
        {
            msSinceEpoch_ = another.msSinceEpoch_;
        }

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] std::string toFormatString(bool showMicroseconds) const;

        [[nodiscard]] bool valid() const { return msSinceEpoch_ > 0; }

        [[nodiscard]] int64_t msSinceEpoch() const { return msSinceEpoch_; }

        static TimeStamp now();

        static TimeStamp invalid()
        {
            return TimeStamp(0);
        }

        static TimeStamp fromUnixTimestamp(int64_t t)
        {
            return fromUnixTime(t, 0);
        }

        static TimeStamp fromUnixTime(const time_t t, const int ms)
        {
            return TimeStamp(t * kMsPerSecond + ms);
        }

        static constexpr int64_t kMsPerSecond = 1000 * 1000;

    private:
        int64_t msSinceEpoch_ = 0;
    };

    inline bool operator<(const TimeStamp &lhs, const TimeStamp &rhs)
    {
        return lhs.msSinceEpoch() < rhs.msSinceEpoch();
    }

    inline bool operator==(const TimeStamp &lhs, const TimeStamp &rhs)
    {
        return lhs.msSinceEpoch() == rhs.msSinceEpoch();
    }

    ///
    /// @param lhs
    /// @param rhs
    /// @return return in seconds
    inline double timeDiff(const TimeStamp &lhs, const TimeStamp &rhs)
    {
        int64_t diff = lhs.msSinceEpoch() - rhs.msSinceEpoch();
        return static_cast<double>(diff) / TimeStamp::kMsPerSecond;
    }

    ///
    /// @param ts1
    /// @param seconds
    /// @return 加 【2】 秒
    inline TimeStamp addTime(const TimeStamp &ts1, const double seconds)
    {
        const auto delta = static_cast<int64_t>(seconds * TimeStamp::kMsPerSecond);
        return TimeStamp(ts1.msSinceEpoch() + delta);
    }
}

#endif // TIMESTAMP_H
