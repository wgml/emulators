project(emulators CXX)
cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
add_compile_options(-Wall -Wextra -pedantic -Werror)

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

include_directories(src)

set(PROJECT_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

set(PROJECT_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/emu/chip8/display.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/emu/chip8/emulator.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/emu/chip8/machine.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/util/configuration.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/util/debug.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/util/log.cpp"
)

add_library(emulatorslib STATIC ${PROJECT_SOURCES})

find_package(Threads)

find_package(fmt 5.3.0 REQUIRED)
find_package(Boost 1.70 COMPONENTS program_options REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(emulatorslib PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(emulatorslib PRIVATE fmt::fmt)
target_link_libraries(emulatorslib PRIVATE spdlog::spdlog)
target_link_libraries(emulatorslib PRIVATE ${Boost_LIBRARIES})
# target_link_libraries(emulatorslib PRIVATE stdc++fs)

add_executable(emulators ${PROJECT_MAIN})
target_link_libraries(emulators PRIVATE emulatorslib)
enable_testing()
add_subdirectory(tests)

