cmake_minimum_required(VERSION 3.22)
project(CLL)

set(CMAKE_CXX_STANDARD 20)

add_executable(CLL
        main.cpp
        FileReader/FileReader.cpp
        FileReader/FileReader.h
        VariableHandler.cpp VariableHandler.hpp CLL.hpp)
