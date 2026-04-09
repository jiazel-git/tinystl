cmake_minimum_required(VERSION 3.5.0)
project(tinystl VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(algorithm)
message("==========configure algorithm success==========")

add_subdirectory(container)
message("==========configure container success==========")

add_subdirectory(test)
message("==========configure test success==========")
