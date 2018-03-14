cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64 -D_INT128")
#set(CMAKE_CXX_FLAGS "-std=gnu++14 -fno-ms-compatibility -fexceptions -m64")
set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")

FILE(GLOB SOURCES *.cpp)
#add_library(Common1 STATIC io_functions.cpp my_function.cpp string_functions.cpp zstring.h)
add_library(common1 STATIC ${SOURCES})
