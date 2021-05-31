cmake_minimum_required(VERSION 2.8)

#if(APPLE)
#set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64")
#else(APPLE)
#set(CMAKE_CXX_FLAGS "-std=c++14 -fexceptions -m64 -static")
#endif(APPLE)

if(CPP_STATIC)
set(CMAKE_CXX_FLAGS "-std=c++14 -m64 -Wall -Wextra -static")
else(CPP_STATIC)
set(CMAKE_CXX_FLAGS "-std=c++14 -m64 -Wall -Wextra")
endif(CPP_STATIC)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message("Release Mode")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()


#set(CMAKE_CXX_FLAGS "-std=gnu++14 -fno-ms-compatibility -fexceptions -m64")
set(CMAKE_CXX_FLAGS_DEBUG  "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG")



INCLUDE_DIRECTORIES(./src ./src/esp ./src/common1 ./src/common2 ./module/TST/src/ ./module/src/TST/common1)

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(~/include)

add_subdirectory(./src/common1)
add_subdirectory(./src/common2)
add_subdirectory(./src/esp)

# Main programs
FILE(GLOB TSTSOURCES module/TST/src/*.cpp module/TST/src/common1/*.cpp)
add_library(tst STATIC ${TSTSOURCES})



#add_executable(tst.out module/TST/construct_main.cpp)
#target_link_libraries(tst.out tst)

add_executable(build-index build_main.cpp)
target_link_libraries(build-index common1 common2 esp tst)
add_executable(decompress decompress_main.cpp)
target_link_libraries(decompress common1 common2 esp tst)
add_executable(search search_main.cpp)
target_link_libraries(search common1 common2 esp tst)
add_executable(extract extract_main.cpp)
target_link_libraries(extract common1 common2 esp tst)

