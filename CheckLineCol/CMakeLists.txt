cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)
project(CheckLineCol)
option(SANITIZE "Enable sanitizers" OFF)
if(SANITIZE)
     message(STATUS "CheckLineCol Sanitizers enabled")
    if(WIN32)
         message(WARNING "Sanitizers may fail to link on Windows")
    endif()
    set(CMAKE_CXX_FLAGS
         "-Wall -Wextra -Wno-sign-compare -pedantic -fsanitize=address,leak,undefined -fno-omit-frame-pointer -g"
    )
endif()

add_executable(CheckLineCol src/main.cpp src/common.h src/common.cpp src/color.hpp src/naive.cpp src/sweep.cpp)

include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG a33701196adfad74917046096bf5a2aa0ab0bb50 # 9.1.0
)
FetchContent_MakeAvailable(fmt)
target_include_directories(CheckLineCol SYSTEM PRIVATE ${fmt_SOURCE_DIR}/include)
target_link_libraries(CheckLineCol PRIVATE fmt::fmt)
