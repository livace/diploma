#pragma once

#include "util/stacktrace.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#define DO_ASSERT_MSG(cond, msg)                                                                                       \
  do {                                                                                                                 \
    if (!(cond)) {                                                                                                     \
      std::cerr << "Assertion failed:\t" << msg << '\n'                                                                \
                  << "Expected:\t" << #cond << "\n"                                                                    \
                  << "Source:\t\t" << __FILE__ << ":" << __LINE__ << "\n"                                              \
                  << ::util::stacktrace() << std::endl;                                                                \
                                                                                                                       \
      std::_Exit(2);                                                                                                   \
    }                                                                                                                  \
  } while (0)

#define DO_ASSERT(cond) DO_ASSERT_MSG(cond, "")

#define GET_ASSERT_MACRO(_1, _2, NAME, ...) NAME

#define ASSERT_FATAL(...) GET_ASSERT_MACRO(__VA_ARGS__, DO_ASSERT_MSG, DO_ASSERT)(__VA_ARGS__)
