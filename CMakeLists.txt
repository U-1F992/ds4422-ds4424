cmake_minimum_required(VERSION 3.10)

project(ds4422_ds4424 VERSION 0.1.0)
set(CMAKE_C_STANDARD 99)

include_directories(include)
add_library(ds4422_ds4424 STATIC
            src/ds4422_ds4424.c)

add_executable(test test/test.c)
target_link_libraries(test ds4422_ds4424)