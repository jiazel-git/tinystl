cmake_minimum_required(VERSION 3.0.0)
project(STL VERSION 0.1.0)

include(CTest)
enable_testing()


aux_source_directory(./src all_src)
include_directories(./include)
add_executable(main ${all_src})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
