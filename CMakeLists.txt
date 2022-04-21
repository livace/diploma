cmake_minimum_required(VERSION 3.23)

project(diploma)

get_filename_component(ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDART 11)

message("Build type: ${CMAKE_BUILD_TYPE}")

add_compile_options(-g)

add_subdirectory(base_algo kek/base_algo)
add_subdirectory(generators kek/generators)
add_subdirectory(graph kek/graph)
