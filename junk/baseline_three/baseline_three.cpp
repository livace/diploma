#include "base_algo/subtract.h"
#include "base_algo/intersect.h"

#include "graph/graph.h"
#include "graph/graph_serializer.h"

#include "util/argparse.h"
#include "util/signal_handler.h"
#include "util/assert.h"

#include "baseline/adag_selection.h"
#include "baseline/gadag_redundant_trees.h"
#include "baseline/dfs_redundant_trees.h"
#include "baseline/dfs_gadag_redundant_trees.h"

#include "dot/dot_serializer.h"

#include <iostream>

struct CommandLineOptions {
  graph::Vertex vertex;
  std::filesystem::path path;
  std::string method;
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
  )(
    "method,m",
    boost::program_options::value<std::string>(&result.method)->required(),
    "method: one of `gadag`, `dfs`, `dfs-gadag`"
  ).parse(argc, argv);

  return result;
}

int main(int argc, char** argv) {
  util::initDefaultSignalHandler();

  auto args = parseArgs(argc, argv);
  auto graph = graph::deserialize(args.path);
  auto selector = baseline::AdagSelector(graph, args.vertex);
  {
    auto res = selector.build();

    if (!res.has_value()) {
      std::cerr << res.error();
      return 1;
    }

  }
  auto gadag = selector.get();

  util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> res;

  if (args.method == "gadag") {
    res = baseline::makeGadagThreeRedundantTrees(graph, args.vertex);
  } else if (args.method == "dfs") {
    res = baseline::makeDfsThreeRedundantTrees(graph, args.vertex);
  } else if (args.method == "dfs-gadag") {
    res = baseline::makeDfsGadagThreeRedundantTrees(graph, args.vertex);
  } else {
    ASSERT_FATAL(false, "Unknown method " << args.method);
  }

  if (!res.has_value()) {
    std::cerr << "Failed to find three trees: " << res.error();
    return 1;
  }

  auto [primary_path, secondary_path, third_path] = *res;

  ASSERT_FATAL(base_algo::intersect(primary_path, secondary_path).edges().empty());
  ASSERT_FATAL(base_algo::intersect(primary_path, third_path).edges().empty());
  ASSERT_FATAL(base_algo::intersect(secondary_path, third_path).edges().empty());

  auto diff = base_algo::subtract(base_algo::subtract(base_algo::subtract(graph, primary_path), secondary_path), third_path);

  dot::DotSerializer serializer;
  serializer.addGraph(primary_path, "red");
  serializer.addGraph(secondary_path, "blue");
  serializer.addGraph(third_path, "green");
  serializer.addGraph(diff, "grey");
  serializer.markVertex(args.vertex, "red");
  serializer.serialize(std::cout);
}
