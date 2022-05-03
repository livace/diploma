cmake_minimum_required(VERSION 3.16.3)

project(dot)

add_library(
  ${PROJECT_NAME}
  dot_serializer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})

target_link_libraries(${PROJECT_NAME} graph CONAN_PKG::boost)
