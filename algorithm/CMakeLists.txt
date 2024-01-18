project(algorithm)

include_directories(./)
aux_source_directory(./ ALL_SRC)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
add_library(${PROJECT_NAME} STATIC ${ALL_SRC})
