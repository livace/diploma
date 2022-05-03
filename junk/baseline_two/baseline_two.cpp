#include "base_algo/subtract.h"
#include "graph/graph.h"
#include "graph/graph_serializer.h"
#include "util/argparse.h"
#include "util/signal_handler.h"
#include "baseline/adag_selection.h"
#include "baseline/gadag_redundant_trees.h"
#include "dot/dot_serializer.h"

#include <iostream>

struct CommandLineOptions {
  graph::Vertex vertex;
  std::filesystem::path path;
};

CommandLineOptions parseArgs(int argc, char** argv) {
  CommandLineOptions result;
  util::ArgumentParser("Full Graph Generator")(
    "vertex,v",
    boost::program_options::value<graph::Vertex>(&result.vertex)->required(),
    "start from vertex"
  )(
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
  auto selector = baseline::AdagSelector(graph, args.vertex);
  auto res = selector.build();

  if (!res.has_value()) {
    std::cerr << res.error();
    return 1;
  }

  auto gadag = selector.get();

  auto [primary_path, secondary_path] = baseline::makeTwoRedundantTrees(gadag, args.vertex);

  auto diff = base_algo::subtract(base_algo::subtract(graph, primary_path), secondary_path);

  dot::DotSerializer serializer;
  serializer.addGraph(primary_path, "red");
  serializer.addGraph(secondary_path, "blue");
  serializer.addGraph(diff, "grey");
  serializer.markVertex(args.vertex, "red");
  serializer.serialize(std::cout);
}
