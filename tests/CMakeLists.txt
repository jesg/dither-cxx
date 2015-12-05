project (dither)

enable_testing()
include_directories(${PROJECT_SOURCE_DIR}/../include)

find_package(Threads)

add_executable(runUnitTests dither_test.cc simple_constraint_handler_test.cc)
target_link_libraries(runUnitTests dither gtest gtest_main
	${CMAKE_THREAD_LIBS_INIT})
add_test(runUnitTests runUnitTests)
