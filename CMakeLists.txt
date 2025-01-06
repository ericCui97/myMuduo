cmake_minimum_required(VERSION 3.0)
project(osMuduo)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++20 -fPIC")
add_subdirectory(libs/spdlog)
find_package(spdlog)
aux_source_directory(./src SRC_LIST)
add_library(osMuduo SHARED ${SRC_LIST})
add_executable(main main.cpp src/timeStamp.cpp src/INetAddress.cpp)
target_link_libraries(main
  spdlog
)