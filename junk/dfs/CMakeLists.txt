cmake_minimum_required(VERSION 3.16.3)

project(junk_dfs)

add_executable(
  ${PROJECT_NAME}
  dfs.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  util
  graph
  base_algo
)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})
