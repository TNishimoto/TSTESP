cmake_minimum_required(VERSION 2.8)

if(APPLE)
set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64")
else(APPLE)
set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64 -static")
endif(APPLE)

#set(CMAKE_CXX_FLAGS "-std=gnu++14 -fno-ms-compatibility -fexceptions -m64")
set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")

INCLUDE_DIRECTORIES(./src ./src/tst ./src/esp ./src/common1 ./src/common2)

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(~/include)

add_subdirectory(./src/common1)
add_subdirectory(./src/common2)
add_subdirectory(./src/esp)

add_executable(build-index build_main.cpp)
target_link_libraries(build-index common1 common2 esp tst)
add_executable(decompress decompress_main.cpp)
target_link_libraries(decompress common1 common2 esp tst)
add_executable(search search_main.cpp)
target_link_libraries(search common1 common2 esp tst)
add_executable(extract extract_main.cpp)
target_link_libraries(extract common1 common2 esp tst)

