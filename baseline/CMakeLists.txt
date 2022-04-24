cmake_minimum_required(VERSION 3.16.3)

project(baseline)

add_library(
  ${PROJECT_NAME}
  adag_selection.cpp
)

target_link_libraries(${PROJECT_NAME} util)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})
