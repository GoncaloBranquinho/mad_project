cmake_minimum_required(VERSION 3.20)

project(test)

set(CMAKE_CXX_STANDARD 17)

find_package(ortools REQUIRED)

add_executable(test main.cpp)

target_link_libraries(test PRIVATE ortools::ortools)
