cmake_minimum_required(VERSION 3.28)
project(TuringMachine)

set(CMAKE_CXX_STANDARD 23)

add_executable(TuringMachine main.cpp
        tape.h
        helpers.h
        turing-machine.h)
