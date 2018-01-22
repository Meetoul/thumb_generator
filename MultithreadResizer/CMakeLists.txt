cmake_minimum_required(VERSION 3.0)

project( MulthreadResizer )

set (CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set (SOURCE_LIB MResizer.cpp)

add_library(MultithreadResizer STATIC ${SOURCE_LIB})

target_link_libraries( MultithreadResizer ${OpenCV_LIBS} )
target_link_libraries( MultithreadResizer Threads::Threads)