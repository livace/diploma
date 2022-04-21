#include "generator/full_graph_generator.h"

#include "graph/graph_serializer.h"
#include "util/argparse.h"
#include "util/signal_handler.h"

#include <filesystem>

struct CommandLineOptions {
  size_t n;
  std::filesystem::path dest;
};

CommandLineOptions parseArgs(int argc, char** argv) {
  CommandLineOptions result;
  util::ArgumentParser("Full Graph Generator")(
    "n,n",
    boost::program_options::value<size_t>(&result.n)->required(),
    "number of vertices"
  )(
    "dest,d",
    boost::program_options::value<std::filesystem::path>(&result.dest)->required(),
    "destination path"
  ).parse(argc, argv);

  return result;
}

int main(int argc, char** argv) {
  util::initDefaultSignalHandler();

  auto args = parseArgs(argc, argv);

  auto graph = generator::FullGraphGenerator(args.n).generate();
  graph::serialize(args.dest, graph);
}
