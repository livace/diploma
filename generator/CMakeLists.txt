cmake_minimum_required(VERSION 3.16.3)

project(generator)

add_library(
  ${PROJECT_NAME}
  full_graph_generator.cpp
  random_graph_generator.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})

target_link_libraries(${PROJECT_NAME} graph base_algo)

add_subdirectory(bin)
