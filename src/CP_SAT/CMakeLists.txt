cmake_minimum_required(VERSION 3.18)
project(MyORToolsCP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ORTOOLS_DIR "/home/goncalo/Transferências/or-tools_x86_64_Fedora-41_cpp_v9.12.4544")

add_definitions(-DOR_PROTO_DLL=)


include_directories(
    ${ORTOOLS_DIR}/include
    ${ORTOOLS_DIR}/include/ortools
    ${ORTOOLS_DIR}/include/absl
)


add_executable(main main.cpp)

target_link_libraries(main
    ${ORTOOLS_DIR}/lib64/libortools.so
    pthread
)

