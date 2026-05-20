cmake_minimum_required(VERSION 3.10)
project(or_tools_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. FIX THE "STRUCT OR_PROTO_DLL" AND "UINT32" ERRORS
# This tells the compiler OR_PROTO_DLL is nothing (not a struct)
add_definitions(-DOR_PROTO_DLL=)
add_definitions(-D__STDC_CONSTANT_MACROS)

# 2. Force include cstdint for the uint32_t error
add_compile_options(-O3 -include cstdint)

# 3. Setup Paths
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
find_package(ortools QUIET)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

# 4. Build
add_executable(main or-tools-solver.cpp ../utils/utils.cpp)

# 5. Link
target_link_libraries(main 
    PRIVATE 
    ortools 
    Clp 
    Cbc 
    protobuf
)