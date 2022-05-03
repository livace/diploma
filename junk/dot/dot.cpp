#include "base_algo/subtract.h"
#include "graph/graph.h"
#include "graph/graph_serializer.h"
#include "util/argparse.h"
#include "util/signal_handler.h"
#include "baseline/adag_selection.h"
#include "dot/dot_serializer.h"

#include <iostream>

struct CommandLineOptions {
  std::filesystem::path path;
};

CommandLineOptions parseArgs(int argc, char** argv) {
  CommandLineOptions result;
  util::ArgumentParser("Full Graph Generator")(
    "path,p",
    boost::program_options::value<std::filesystem::path>(&result.path)->required(),
    "graph path"
  ).parse(argc, argv);

  return result;
}

int main(int argc, char** argv) {
  util::initDefaultSignalHandler();

  auto args = parseArgs(argc, argv);
  auto graph = graph::deserialize(args.path);
  dot::DotSerializer serializer;
  serializer.addGraph(graph);
  serializer.serialize(std::cout);
}
