cmake_minimum_required(VERSION 3.16.3)

project(baseline)

add_library(
  ${PROJECT_NAME}
  adag_selection.cpp
  dfs_redundant_trees.cpp
  dfs_gadag_redundant_trees.cpp
  gadag_redundant_trees.cpp
)

target_link_libraries(${PROJECT_NAME} util base_algo)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})
