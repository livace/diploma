include_guard(GLOBAL)

message("running conan install")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  message("compiler: clang")
  set(CONAN_PROFILE "${CMAKE_CURRENT_LIST_DIR}/clang.profile")
else()
  message(FATAL_ERROR "unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

find_program(CONAN conan)
if(NOT CONAN)
  message(FATAL_ERROR "conan not found")
endif()

set(ENV{CONAN_REVISIONS_ENABLED} 1)

set(CONANFILE "${ROOT}/cmake/conanfile.txt")
execute_process(
  RESULT_VARIABLE CONAN_RETURN_CODE
  OUTPUT_FILE "${CMAKE_BINARY_DIR}/conan.log"
  ERROR_FILE "${CMAKE_BINARY_DIR}/conan.log"
  COMMAND "${CONAN}" "install"
      "${CONANFILE}"
      "--install-folder" "${CMAKE_BINARY_DIR}"
      "--profile" "${CONAN_PROFILE}"
      "--build=missing"
)

if(NOT CONAN_RETURN_CODE EQUAL 0)
  message(FATAL_ERROR "conan install failed")
endif()

message("conan installed successfully")

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
set(CONAN_CMAKE_SILENT_OUTPUT true)
conan_define_targets()
