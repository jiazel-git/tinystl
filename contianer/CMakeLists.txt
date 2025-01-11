project(contianer)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
aux_source_directory(./ ALL_SRC)
add_library(${PROJECT_NAME} STATIC ${ALL_SRC})
