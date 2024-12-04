cmake_minimum_required(VERSION 3.29)
project(aoc2024)
set(CMAKE_CXX_STANDARD 20)

find_package(range-v3 REQUIRED)

add_executable(01 01/main.cpp)
add_executable(02 02/main.cpp)
target_link_libraries(02 range-v3)
