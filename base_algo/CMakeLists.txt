cmake_minimum_required(VERSION 3.23)

project(base_algo)

add_library(
  ${PROJECT_NAME}
  dfs.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})
