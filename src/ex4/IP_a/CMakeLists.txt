cmake_minimum_required(VERSION 3.18)
project(MyORToolsCP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ortools REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE ortools::ortools)
