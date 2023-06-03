cmake_minimum_required(VERSION 3.24)
project(Cod-SD)

option(SANITIZE "Enable sanitizers" OFF)
if(SANITIZE)
    message(STATUS "Sanitizers enabled")
    if(WIN32)
        message(WARNING "Sanitizers may fail to link on Windows")
    endif()
    set(CMAKE_CXX_FLAGS
            "-Wall -Wextra -Wno-sign-compare -pedantic -fsanitize=address,leak,undefined -fno-omit-frame-pointer -g"
            )
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/Josephus")
add_subdirectory("${PROJECT_SOURCE_DIR}/SkipList-vs-List")
add_subdirectory("${PROJECT_SOURCE_DIR}/CheckLineCol")
add_subdirectory("${PROJECT_SOURCE_DIR}/heaps")
