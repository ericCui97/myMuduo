cmake_minimum_required(VERSION 3.0)
project(osMuduo)

set(CMAKE_CXX_STANDARD 20)
add_executable(osMuduo main.cpp src/timeStamp.cpp src/nocopyable.h src/INetAddress.cpp src/Thread.cpp src/Thread.h src/atomic.h src/copyable.h)