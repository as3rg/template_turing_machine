cmake_minimum_required(VERSION 3.21)
project(ttm-lib)

set(CMAKE_CXX_STANDARD 23)

add_library(ttm-lib SHARED
        helpers.h
        tape.h
        turing-machine.h
        turing-machine.cpp)