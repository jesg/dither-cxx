

add_library(dither)

target_sources(dither PRIVATE dither.cc ipog.cc simple_constraint_handler.cc)
target_include_directories(dither PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(dither PUBLIC  Gecode::Driver Gecode::Int Gecode::Kernel Gecode::Minimodel Gecode::Search Gecode::Set Gecode::Support)
target_compile_options(dither PRIVATE /wd4267)
set_target_properties(dither PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(dither PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(dither PROPERTIES CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)