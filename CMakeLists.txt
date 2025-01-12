cmake_minimum_required(VERSION 3.2)
project(osMuduo)
enable_testing()
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++20 -fPIC")
# add_subdirectory(libs/spdlog)

add_subdirectory(test)
# find_package(spdlog)
aux_source_directory(./src SRC_LIST)
add_library(osMuduo SHARED ${SRC_LIST})
add_executable(main main.cpp 
src/timeStamp.cpp 
src/INetAddress.cpp 
src/CurrentThread.cpp
src/EventLoop.cpp
src/channel.cpp
)
# target_link_libraries(main
#   spdlog
# )