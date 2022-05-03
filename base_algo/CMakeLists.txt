cmake_minimum_required(VERSION 3.16.3)

project(base_algo)

add_library(
  ${PROJECT_NAME}
  dfs.cpp
  intersect.cpp
  is_nested.cpp
  is_rooted_tree.cpp
  is_dag.cpp
  make_undirected.cpp
  is_undirected.cpp
  subtract.cpp
  without.cpp
)

target_link_libraries(${PROJECT_NAME} util)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})
