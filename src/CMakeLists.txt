

add_library(dither)

target_sources(dither PRIVATE dither.cc ipog.cc simple_constraint_handler.cc)
target_include_directories(dither PUBLIC ${CMAKE_SOURCE_DIR}/include)

if (MSVC)
  target_compile_options(dither PRIVATE /wd4267)
endif()
set_target_properties(dither PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(dither PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(dither PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)