project (dither)# The version number.

include_directories (${PROJECT_SOURCE_DIR}/../include)
include_directories(${GECODE_INCLUDE_DIRS})

add_library(dither SHARED dither.cc ipog.cc)
target_link_libraries(dither ${GECODE_LIBRARIES})
