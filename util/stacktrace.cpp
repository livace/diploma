#include "stacktrace.h"

#include <string>
#include <sstream>

#include <backward.hpp>

namespace util {

std::string stacktrace() {
  backward::StackTrace st;
  st.load_here();

  std::stringstream result;
  backward::Printer p;
  p.trace_context_size = 1;
  p.print(st, result);

  return result.str();
}

}  // namespace util
