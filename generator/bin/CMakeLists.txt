cmake_minimum_required(VERSION 3.16.3)

project(generator_bin)

add_executable(
  full_graph_generator
  full_graph_generator_cli.cpp
)

target_include_directories(full_graph_generator PUBLIC ${ROOT})

target_link_libraries(full_graph_generator graph generator util dot)
