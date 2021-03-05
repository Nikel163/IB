cmake_minimum_required(VERSION 3.17)
project(IB)

set(CMAKE_CXX_STANDARD 14)

add_executable(IB main.cpp functions/ofb.cpp functions/ofb.h utils/utils.cpp utils/utils.h utils/md5.hpp utils/md5.cpp functions/rle.cpp functions/rle.h)