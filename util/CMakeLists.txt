cmake_minimum_required(VERSION 3.16.3)
project(util)

add_library(${PROJECT_NAME} argparse.cpp signal_handler.cpp stacktrace.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT})

target_link_libraries(
  ${PROJECT_NAME}
  CONAN_PKG::boost
  CONAN_PKG::backward-cpp
  CONAN_PKG::tl-expected
)
