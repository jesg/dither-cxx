set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(gecode)
add_subdirectory(googletest)

# gtest needs extra treatment....
set_target_properties(gtest PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${SUBDOLDER})
set_target_properties(gtest PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${SUBDOLDER})
set_target_properties(gtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${SUBDOLDER})