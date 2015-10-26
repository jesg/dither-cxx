project (dither)# The version number.

include_directories (${PROJECT_SOURCE_DIR}/../include)

add_library(dither SHARED dither.cc ipog.cc)
