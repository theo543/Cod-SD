cmake_minimum_required(VERSION 3.24)
project(SkipList-vs-List)

set(CMAKE_CXX_STANDARD 20)

add_executable(SkipList-vs-List main.cpp SkipList.hpp SortedList.hpp)

option(SANITIZE "Enable sanitizers" OFF)
if(SANITIZE)
    message(STATUS "SkipList Sanitizers enabled")
    if(WIN32)
        message(WARNING "Sanitizers may fail to link on Windows")
    endif()
    set(CMAKE_CXX_FLAGS
            "-Wall -Wextra -Wno-sign-compare -pedantic -fsanitize=address,leak,undefined -fno-omit-frame-pointer -g"
    )
endif()

