# spdlog
cmake -S . -B build
cmake --build build

cd build&&ctest

# unit test
make compile_tests