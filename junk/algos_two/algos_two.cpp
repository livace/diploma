#include "base_algo/subtract.h"
#include "base_algo/intersect.h"
#include "base_algo/is_rooted_tree.h"

#include "graph/graph.h"
#include "graph/graph_serializer.h"

#include "util/argparse.h"
#include "util/assert.h"
#include "util/log.h"
#include "util/signal_handler.h"

#include "algos/adag_selection.h"
#include "algos/gadag_redundant_trees.h"
#include "algos/dfs_redundant_trees.h"

#include "dot/dot_serializer.h"

#include <iostream>
#include <fstream>

struct CommandLineOptions {
  graph::Vertex vertex;
  std::filesystem::path path;
  std::filesystem::path score_file;
  std::string method;
};

CommandLineOptions parseArgs(int argc, char** argv) {
  CommandLineOptions result;
  util::ArgumentParser("Select two redundant trees")(
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
    "method: one of `gadag`, `dfs`"
  )(
    "score,s",
    boost::program_options::value<std::filesystem::path>(&result.score_file)->required(),
    "score file path"
  ).parse(argc, argv);

  return result;
}

int main(int argc, char** argv) {
  util::initDefaultSignalHandler();

  auto args = parseArgs(argc, argv);
  auto graph = graph::deserialize(args.path);

  graph::Graph primary_path;
  graph::Graph secondary_path;

  if (args.method == "gadag") {
    auto selector = algos::AdagSelector(graph, args.vertex);
    auto res = selector.build();

    if (!res.has_value()) {
      std::cerr << res.error() << "\n";
      return 1;
    }

    auto gadag = selector.get();

    std::tie(primary_path, secondary_path) = algos::makeTwoRedundantTrees(
      gadag,
      selector.localRoot(),
      args.vertex);
  } else if (args.method == "dfs") {
    auto res = algos::makeDfsRedundantTrees(2, /* seed = */ 0, graph, args.vertex);

    if (!res.has_value()) {
      std::cerr << res.error() << "\n";
      return 1;
    }

    primary_path = (*res)[0];
    secondary_path = (*res)[1];
  }

  auto diff = base_algo::subtract(base_algo::subtract(graph, primary_path), secondary_path);

  dot::DotSerializer serializer("algos_two");
  serializer.addGraph(primary_path, "primary_path", "red");
  serializer.addGraph(secondary_path, "secondary_path", "blue");
  serializer.addGraph(diff, "diff", "grey");
  serializer.markVertex(args.vertex, "red");
  serializer.addGraph(base_algo::intersect(primary_path, secondary_path), "intersection", "green");
  serializer.serialize(std::cout);

  std::ofstream score_file(args.score_file);
  auto score = base_algo::intersect(primary_path, secondary_path).edges().size();

  INFO() << "Score is " << score;
  score_file << score << '\n';

  ASSERT_FATAL(primary_path.vertices().size() == graph.vertices().size());
  ASSERT_FATAL(secondary_path.vertices().size() == graph.vertices().size());

  ASSERT_FATAL(base_algo::isRootedTree(primary_path, args.vertex));
  ASSERT_FATAL(base_algo::isRootedTree(secondary_path, args.vertex));

  ASSERT_FATAL(base_algo::intersect(primary_path, graph) == primary_path);
  ASSERT_FATAL(base_algo::intersect(secondary_path, graph) == secondary_path);
}
