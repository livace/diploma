cmake_minimum_required(VERSION 3.23)

project(generators)

add_library(
  ${PROJECT_NAME}
  full_graph_generator.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})

target_link_libraries(${PROJECT_NAME} graph)
