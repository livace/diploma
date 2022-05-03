cmake_minimum_required(VERSION 3.16.3)

project(junk_dot)

add_executable(
  ${PROJECT_NAME}
  dot.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  util
  graph
  base_algo
  baseline
  dot
)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})
