#pragma once

#include <iostream>

namespace util::log::impl {

class Logger {
public:
  Logger(std::ostream& os) : os_(os) {}

  template <class T>
  Logger& operator<<(T&& arg) {
    os_ << arg;
    return *this;
  }

  ~Logger() {
    os_ << "\n";
  }

private:
  std::ostream& os_;
};

} // namespace util::log::impl


#define INFO() util::log::impl::Logger(std::cout)
#define DEBUG() util::log::impl::Logger(std::cerr)
#define ERROR() util::log::impl::Logger(std::cerr)

#define NAMED(x) #x << " = " << (x)
