#include "base_algo/subtract.h"
#include "base_algo/intersect.h"
#include "base_algo/is_rooted_tree.h"

#include "graph/graph.h"
#include "graph/graph_serializer.h"

#include "util/argparse.h"
#include "util/signal_handler.h"
#include "util/assert.h"

#include "algos/adag_selection.h"
#include "algos/gadag_redundant_trees.h"
#include "algos/dfs_redundant_trees.h"
#include "algos/dfs_gadag_redundant_trees.h"
#include "algos/dfs_two_connected.h"

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
    "method: one of `gadag`, `dfs`, `dfs-gadag`, `dfs-two-connected`, `dfs-two-connected-strict`"
  ).parse(argc, argv);

  return result;
}

int main(int argc, char** argv) {
  util::initDefaultSignalHandler();

  auto args = parseArgs(argc, argv);
  auto graph = graph::deserialize(args.path);

  util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> res;

  if (args.method == "gadag") {
    res = algos::makeGadagThreeRedundantTrees(graph, args.vertex);
  } else if (args.method == "dfs") {
    // res = algos::makeDfsThreeRedundantTrees(3, 0, graph, args.vertex);
  } else if (args.method == "dfs-gadag") {
    res = algos::makeDfsGadagThreeRedundantTrees(graph, args.vertex);
  } else if (args.method == "dfs-two-connected") {
    res = algos::makeDfsTwoConnectedThreeRedundantTrees(graph, args.vertex, 1);
  } else if (args.method == "dfs-two-connected-strict") {
    res = algos::makeDfsTwoConnectedThreeRedundantTrees(graph, args.vertex, 2);
  } else {
    ASSERT_FATAL(false, "Unknown method " << args.method);
  }

  if (!res.has_value()) {
    std::cerr << "Failed to find three trees: " << res.error();
    return 1;
  }

  auto [primary_path, secondary_path, third_path] = *res;

  auto diff = base_algo::subtract(base_algo::subtract(base_algo::subtract(graph, primary_path), secondary_path), third_path);

  dot::DotSerializer serializer("algos");
  serializer.addGraph(primary_path, "primary_path", "red");
  serializer.addGraph(secondary_path, "secondary_path", "blue");
  serializer.addGraph(third_path, "third_path", "green");
  serializer.addGraph(diff, "diff", "grey");
  serializer.markVertex(args.vertex, "red");
  serializer.serialize(std::cout);

  ASSERT_FATAL(base_algo::intersect(primary_path, secondary_path).edges().empty());
  ASSERT_FATAL(base_algo::intersect(primary_path, third_path).edges().empty());
  ASSERT_FATAL(base_algo::intersect(secondary_path, third_path).edges().empty());

  ASSERT_FATAL(base_algo::isRootedTree(primary_path, args.vertex));
  ASSERT_FATAL(base_algo::isRootedTree(secondary_path, args.vertex));
  ASSERT_FATAL(base_algo::isRootedTree(third_path, args.vertex));

  ASSERT_FATAL(primary_path.vertices().size() == graph.vertices().size());
  ASSERT_FATAL(secondary_path.vertices().size() == graph.vertices().size());
  ASSERT_FATAL(third_path.vertices().size() == graph.vertices().size());
}
