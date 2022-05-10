#include "generator/random_graph_generator.h"

#include "graph/graph_serializer.h"
#include "util/argparse.h"
#include "util/signal_handler.h"
#include "dot/dot_serializer.h"

#include <filesystem>
#include <fstream>

struct CommandLineOptions {
  size_t connectivity;
  size_t n;
  size_t m;
  uint64_t seed;
  std::filesystem::path dest;
  std::filesystem::path dot_dest;
};

CommandLineOptions parseArgs(int argc, char** argv) {
  CommandLineOptions result;
  util::ArgumentParser("Random Graph Generator")(
    "connectivity,c",
    boost::program_options::value<size_t>(&result.connectivity)->required(),
    "connectivity number"
  )(
    "n,n",
    boost::program_options::value<size_t>(&result.n)->required(),
    "number of vertices"
  )(
    "m,m",
    boost::program_options::value<size_t>(&result.m)->required(),
    "number of edges"
  )(
    "seed,s",
    boost::program_options::value<uint64_t>(&result.seed)->required(),
    "random seed"
  )(
    "dest,d",
    boost::program_options::value<std::filesystem::path>(&result.dest)->required(),
    "serialize graph"
  )(
    "dot",
    boost::program_options::value<std::filesystem::path>(&result.dot_dest),
    "serialize graph into dot"
  ).parse(argc, argv);

  return result;
}

int main(int argc, char** argv) {
  util::initDefaultSignalHandler();

  auto args = parseArgs(argc, argv);

  auto graph = generator::RandomGraphGenerator(args.connectivity, args.n, args.m, args.seed).generate();
  graph::serialize(args.dest, graph);

  if (!args.dot_dest.empty()) {
    dot::DotSerializer serializer("graph");
    serializer.addGraph(graph, "graph", "black");

    std::ofstream of(args.dot_dest);
    serializer.serialize(of);
  }
}
