cmake_minimum_required(VERSION 3.5.0)
project(tinystl VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(algorithm)
message("algorithm ok")
add_subdirectory(contianer)
message("contianer ok")
add_subdirectory(src)
message("src ok")

