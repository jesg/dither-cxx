
enable_testing()
add_executable(runUnitTests)
target_sources(runUnitTests PRIVATE dither_test.cc  combinations_test.cc)
#target_sources(runUnitTests PRIVATE simple_constraint_handler_test.cc gecode_constraint_handler_test.cc)
target_link_libraries(runUnitTests PRIVATE dither gtest gtest_main)
target_compile_options(runUnitTests PRIVATE /wd4267 /wd4251 /wd4275)

add_test(runUnitTests runUnitTests)