cmake_minimum_required(VERSION 3.19)
project(图像压缩)

set(CMAKE_CXX_STANDARD 14)

add_executable(encoding compress_encoding.cpp)
add_executable(test test.cpp)
add_executable(decoding compress_decoding.cpp)