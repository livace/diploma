#include "argparse.h"

#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <iostream>

namespace po = boost::program_options;

namespace util {

ArgumentParser::ArgumentParser(std::string title):
    desc_(std::move(title)) {
  desc_.add_options()("help,h", "show help and exit");
}

void ArgumentParser::parse(int argc, char* argv[]) {
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc_).run(), vm);

  if (vm.count("help")) {
    std::cerr << desc_ << std::endl;
    exit(0);
  }

  po::notify(vm);
}

}  // namespace util
