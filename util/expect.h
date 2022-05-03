#pragma once

#include <boost/system/error_code.hpp>
#include <sstream>
#include <string>
#include <tl/expected.hpp>

#define PROPAGATE_ERROR(f)                                                                                             \
  do {                                                                                                                 \
    auto __result = f;                                                                                                 \
    if (!__result.has_value()) {                                                                                       \
      return tl::make_unexpected(__result.error());                                                                    \
    }                                                                                                                  \
  } while (0)

#define EXPECT(cond, error)                                                                                \
  do {                                                                                                                 \
    if (!(cond)) {                                                                                                     \
      std::stringstream s;                                                                                             \
      s << error;                                                                                                      \
      return tl::make_unexpected(s.str());                                                                             \
    }                                                                                                                  \
  } while (0)

namespace util {

template <class ...Args>
using Expected = tl::expected<Args...>;

using Error = std::string;

} // namespace util
