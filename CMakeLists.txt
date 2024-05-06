cmake_minimum_required(VERSION 3.10)

project(ds4422-ds4424)
set(CMAKE_C_STANDARD 99)

include_directories(include)
add_library(ds4422-ds4424 INTERFACE)
target_include_directories(ds4422-ds4424 INTERFACE include)

add_executable(test test/test.c)
target_link_libraries(test ds4422-ds4424)