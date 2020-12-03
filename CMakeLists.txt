cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
project (dither VERSION 0.1.1)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(ext/gecode)
add_subdirectory(ext/googletest)

add_subdirectory(src)
add_subdirectory(tests)
