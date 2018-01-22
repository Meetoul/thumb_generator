cmake_minimum_required(VERSION 3.0)

project( ThreadPool )

set (CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set (SOURCE_LIB ThreadPool.cpp Worker.cpp)

add_library(ThreadPool STATIC ${SOURCE_LIB})

target_link_libraries( ThreadPool Threads::Threads)