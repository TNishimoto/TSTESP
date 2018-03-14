cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64")
set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")

INCLUDE_DIRECTORIES(./common1 ./common2)
FILE(GLOB SOURCES *.cpp add/*.cpp)
add_library(esp STATIC ${SOURCES})
target_link_libraries(esp common1 common2)
