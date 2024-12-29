
#include <iostream>
#include <ctime>
#include "src/timeStamp.h"
#include "src/INetAddress.h"

int main() {
    // 测试默认构造函数
    muduo::TimeStamp defaultTs;
    std::cout << "Default Timestamp valid: " << (defaultTs.valid()? "true" : "false") << std::endl;

    // 测试带参数的构造函数
    muduo::TimeStamp tsWithValue(1612444800000000); // 假设这是一个代表某个时间点的时间戳值示例
    std::cout << "Timestamp with value valid: " << (tsWithValue.valid()? "true" : "false") << std::endl;

    // 测试复制构造函数
    muduo::TimeStamp copiedTs(tsWithValue);
    std::cout << "Copied Timestamp has same value as original: " << (copiedTs.msSinceEpoch() == tsWithValue.msSinceEpoch()? "true" : "false") << std::endl;

    // 测试toString和toFormatString函数
    std::cout << "Timestamp as string: " << tsWithValue.toString() << std::endl;
    std::cout << "Timestamp as formatted string (showMicroseconds=false): " << tsWithValue.toFormatString(false) << std::endl;
    std::cout << "Timestamp as formatted string (showMicroseconds=true): " << tsWithValue.toFormatString(true) << std::endl;

    // 测试now函数（注意实际中这里获取的是当前时间，每次运行结果会不同）
    muduo::TimeStamp currentTs = muduo::TimeStamp::now();
    std::cout << "Current Timestamp valid: " << (currentTs.valid()? "true" : "false") << std::endl;
    std::cout << "Current Timestamp as string: " << currentTs.toString() << std::endl;

    // 测试fromUnixTimestamp函数
    time_t unixTime = time(nullptr);
    muduo::TimeStamp tsFromUnix = muduo::TimeStamp::fromUnixTimestamp(unixTime);
    std::cout << "Timestamp from Unix Timestamp valid: " << (tsFromUnix.valid()? "true" : "false") << std::endl;
    std::cout << "Timestamp from Unix Timestamp as string: " << tsFromUnix.toString() << std::endl;

    // 测试fromUnixTime函数
    muduo::TimeStamp tsFromUnixTime = muduo::TimeStamp::fromUnixTime(unixTime, 500);
    std::cout << "Timestamp from Unix Time (with milliseconds) valid: " << (tsFromUnixTime.valid()? "true" : "false") << std::endl;
    std::cout << "Timestamp from Unix Time (with milliseconds) as string: " << tsFromUnixTime.toString() << std::endl;

    // 测试比较运算符
    std::cout << "Is tsWithValue < currentTs: " << (tsWithValue < currentTs? "true" : "false") << std::endl;
    std::cout << "Is tsWithValue == currentTs: " << (tsWithValue == currentTs? "true" : "false") << std::endl;

    // 测试timeDiff函数
    double diffSeconds = muduo::timeDiff(currentTs, tsWithValue);
    std::cout << "Time difference in seconds between currentTs and tsWithValue: " << diffSeconds << std::endl;

    // 测试addTime函数
    muduo::TimeStamp addedTs = muduo::addTime(currentTs, 10.0); // 给当前时间戳加10秒
    std::cout << "Added Timestamp valid: " << (addedTs.valid()? "true" : "false") << std::endl;
    std::cout << "Added Timestamp as string: " << addedTs.toString() << std::endl;

    // test inetAddress
    std::cout<<"test inetAddress"<<std::endl;
    INetAddress netAddress(8080);
    std::cout<<netAddress.toPort()<<std::endl;
    std::cout<<netAddress.toIpPort()<<std::endl;
    return 0;
}