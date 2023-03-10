cmake_minimum_required(VERSION 3.24)
project(Josephus)

set(CMAKE_CXX_STANDARD 17)

add_executable(Josephus main.cpp LoopedList.cpp LoopedList.h)
