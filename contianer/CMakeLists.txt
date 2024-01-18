project(contianer)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
include_directories(./include)
aux_source_directory(./include ALL_SRC)
add_library(${PROJECT_NAME} STATIC ${ALL_SRC})
