cmake_minimum_required(VERSION 3.21)
project(ttm-examples)

set(CMAKE_CXX_STANDARD 23)

add_executable(ttm-examples debug.h macros.h
#        invert.cpp)
        sum.cpp)

target_link_libraries(ttm-examples PUBLIC ttm-lib)
