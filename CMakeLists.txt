cmake_minimum_required(VERSION 3.16.3)

project(diploma)

get_filename_component(ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDART 11)

message("Build type: ${CMAKE_BUILD_TYPE}")

add_compile_options(-g)

include(cmake/conan.cmake)

add_subdirectory(baseline kek/baseline)
add_subdirectory(base_algo kek/base_algo)
add_subdirectory(generator kek/generator)
add_subdirectory(graph kek/graph)
add_subdirectory(junk kek/junk)
add_subdirectory(util kek/util)
