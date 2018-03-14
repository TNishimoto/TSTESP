cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64")
set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")

FILE(GLOB SOURCES *.cpp)

add_library(common2 STATIC ${SOURCES})
target_link_libraries(common2)
#LINK_DIRECTORIES(~/lib)
include_directories(~/include)

