cmake_minimum_required(VERSION 3.16.3)

project(algos)

add_library(
  ${PROJECT_NAME}
  adag_selection.cpp
  build_gadag_tree.cpp
  dfs_tree_selector.cpp
  dfs_redundant_trees.cpp
  dfs_gadag_redundant_trees.cpp
  dfs_two_connected.cpp
  gadag_redundant_trees.cpp
)

target_link_libraries(${PROJECT_NAME} util base_algo dot)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})
