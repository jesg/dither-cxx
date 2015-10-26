project (dither)

enable_testing()
include_directories(${PROJECT_SOURCE_DIR}/../include)

add_executable(runUnitTests dither_test.cc)
target_link_libraries(runUnitTests dither gtest gtest_main)
add_test(runUnitTests runUnitTests)
