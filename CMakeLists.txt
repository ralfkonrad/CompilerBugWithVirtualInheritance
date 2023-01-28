cmake_minimum_required(VERSION 3.15.0)
project(CompilerBugWithVirtualInheritance)

option(WARNING_AS_ERROR "Treat warnings as errors" OFF)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS FALSE)

add_executable(compilerbug compilerbug.cpp)

if(MSVC)
    target_compile_options(compilerbug PRIVATE /W4 $<$<BOOL:${WARNING_AS_ERROR}>:/WX>)
else()
    target_compile_options(compilerbug PRIVATE -Wall -Wextra -Wpedantic $<$<BOOL:${WARNING_AS_ERROR}>:-Werror>)
endif()

add_test(NAME testcompilerbug COMMAND compilerbug)

enable_testing()