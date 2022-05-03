cmake_minimum_required(VERSION 3.16.3)

project(junk_baseline_three)

add_executable(
  ${PROJECT_NAME}
  baseline_three.cpp
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
