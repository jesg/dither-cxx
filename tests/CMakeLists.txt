
enable_testing()
add_executable(dither_test)
target_sources(dither_test PRIVATE dither_test.cc  combinations_test.cc)
#target_sources(runUnitTests PRIVATE simple_constraint_handler_test.cc gecode_constraint_handler_test.cc)
target_link_libraries(dither_test PRIVATE dither gtest gtest_main)
if (MSVC)
  target_compile_options(dither_test PRIVATE /wd4267 /wd4251 /wd4275)
endif()

add_custom_target(run_unit_tests COMMAND "$<TARGET_FILE:dither_test>" DEPENDS dither_test)