cmake_minimum_required(VERSION 3.2)
project(shared_ptr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -v")

set(SOURCE_FILES main.cpp SharedPtr.h test_class.h)
add_executable(shared_ptr ${SOURCE_FILES})
