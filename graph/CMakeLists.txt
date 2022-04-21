cmake_minimum_required(VERSION 3.23)

project(graph)

add_library(
  ${PROJECT_NAME}
  edge.cpp
  graph.cpp
  graph_serializer.cpp
  vertex.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})
