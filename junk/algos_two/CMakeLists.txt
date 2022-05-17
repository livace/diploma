cmake_minimum_required(VERSION 3.16.3)

project(junk_algos)

add_executable(
  ${PROJECT_NAME}_dfs
  algos_two_dfs.cpp
)

add_executable(
  ${PROJECT_NAME}_gadag
  algos_two_gadag.cpp
)

target_link_libraries(
  ${PROJECT_NAME}_dfs
  util
  graph
  base_algo
  algos
  dot
)

target_link_libraries(
  ${PROJECT_NAME}_gadag
  util
  graph
  base_algo
  algos
  dot
)

target_include_directories(${PROJECT_NAME}_dfs PUBLIC ${ROOT})
target_include_directories(${PROJECT_NAME}_gadag PUBLIC ${ROOT})
