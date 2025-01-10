project(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)
include_directories(../algorithm)
include_directories(../contianer)
include_directories(../memory)
include_directories(../test)
add_executable(${PROJECT_NAME} ./main.cpp)
target_link_libraries(${PROJECT_NAME} algorithm contianer)