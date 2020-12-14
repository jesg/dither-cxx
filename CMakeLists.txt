cmake_minimum_required (VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
project (dither VERSION 0.1.1)

if (GENERATOR_IS_MULTI_CONFIG)
  set(SUBFOLDER $<$CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${SUBDOLDER})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${SUBDOLDER})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${SUBDOLDER})


add_subdirectory(ext)

add_subdirectory(src)
add_subdirectory(tests)

