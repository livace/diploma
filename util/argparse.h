#pragma once

#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>

namespace util {

class ArgumentParser {
public:
  ArgumentParser(std::string title);

  void parse(int argc, char* argv[]);

  template<class... Args>
  ArgumentParser& operator()(const Args&... args) {
    desc_.add_options()(args...);
    return *this;
  }

  template<class... Args>
  ArgumentParser& add(const Args&... args) {
    desc_.add(args...);
    return *this;
  }

private:
  boost::program_options::options_description desc_;
};

}  // namespace util
