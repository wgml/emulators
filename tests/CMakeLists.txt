cmake_minimum_required(VERSION 3.0)

set(TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)
find_path(CATCH_INCLUDE_DIR NAMES catch.hpp PATH_SUFFIXES catch2)
include_directories(${CATCH_INCLUDE_DIR})

add_executable(emulators_tests ${TEST_SOURCES})
target_link_libraries(emulators_tests emulatorslib)
add_test(NAME emulators_tests
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         COMMAND ${CMAKE_BINARY_DIR}/tests/emulators_tests --force-colour)
