cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)
include_directories(${CMAKE_SOURCE_DIR}/algorithm)
include_directories(${CMAKE_SOURCE_DIR}/container)
include_directories(${CMAKE_SOURCE_DIR}/memory)

add_executable(algorithm_test algorithm_test.cpp)
target_link_libraries(algorithm_test PRIVATE algorithm)
add_test(NAME algorithm_test COMMAND algorithm_test)

add_executable(iterator_test iterator_test.cpp)
target_link_libraries(iterator_test PRIVATE algorithm container)
add_test(NAME iterator_test COMMAND iterator_test)

add_executable(vector_test vector_test.cpp)
target_link_libraries(vector_test PRIVATE container)
add_test(NAME vector_test COMMAND vector_test)

add_executable(array_test array_test.cpp)
add_test(NAME array_test COMMAND array_test)
