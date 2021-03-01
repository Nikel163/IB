cmake_minimum_required(VERSION 3.17)
project(IB)

set(CMAKE_CXX_STANDARD 14)

add_executable(IB main.cpp functions/functions.cpp functions/functions.h utils/utils.cpp utils/utils.h utils/md5.hpp utils/md5.cpp)