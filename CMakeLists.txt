cmake_minimum_required(VERSION 3.0)

project( ThumbGen )

set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.58.0 REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_EXE main.cpp)

include_directories(ThreadPool)
include_directories(MultithreadResizer)

add_executable( ThumbGen ${SOURCE_EXE} )

add_subdirectory(ThreadPool)
add_subdirectory(MultithreadResizer)

target_link_libraries( ThumbGen Boost::filesystem )
target_link_libraries( ThumbGen ThreadPool )
target_link_libraries( ThumbGen MultithreadResizer )