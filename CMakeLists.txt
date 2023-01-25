cmake_minimum_required(VERSION 3.15.0)
project(CompilerBugWithVirtualInheritance)
set(CMAKE_CXX_STANDARD 14)

add_executable(compilerbug compilerbug.cpp)

if(MSVC)
    target_compile_options(compilerbug PRIVATE /W4 /WX)
else()
    target_compile_options(compilerbug PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()