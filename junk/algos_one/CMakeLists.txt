cmake_minimum_required(VERSION 3.16.3)

project(junk_algos_one)

add_executable(
  ${PROJECT_NAME}
  algos_one_dfs.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  util
  graph
  base_algo
  algos
  dot
)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})
