cmake_minimum_required(VERSION 3.16.3)

project(junk_algos_more)

add_executable(
  ${PROJECT_NAME}_dfs
  algos_more_dfs.cpp
)

target_link_libraries(
  ${PROJECT_NAME}_dfs
  util
  graph
  base_algo
  algos
  dot
)

target_include_directories(${PROJECT_NAME}_dfs PUBLIC ${ROOT})
