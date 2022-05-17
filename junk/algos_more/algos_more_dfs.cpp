#include "base_algo/subtract.h"
#include "base_algo/intersect.h"
#include "base_algo/is_rooted_tree.h"
#include "base_algo/distance.h"
#include "base_algo/test_connectivity.h"

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
  double depth = 1.0;
  bool reset_depth;
  int vertex_limit = 0;
  int trees;
  algos::EdgeSkippingPolicy edge_skipping_policy;
  bool greedy_size;
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
    "score,s",
    boost::program_options::value<std::filesystem::path>(&result.score_file)->required(),
    "score file path"
  )(
    "depth",
    boost::program_options::value<double>(&result.depth)->required(),
    "depth"
  )(
    "reset-depth",
    boost::program_options::value<bool>(&result.reset_depth)->required(),
    "reset depth"
  )(
    "vertex-limit",
    boost::program_options::value<int>(&result.vertex_limit)->required(),
    "neighbor vertex limit"
  )(
    "trees",
    boost::program_options::value<int>(&result.trees)->required(),
    "trees number"
  )(
    "policy",
    boost::program_options::value<algos::EdgeSkippingPolicy>(&result.edge_skipping_policy)->required()
  )(
    "greedy-size",
    boost::program_options::value<bool>(&result.greedy_size)->required()
  ).parse(argc, argv);

  return result;
}

int main(int argc, char** argv) {
  util::initDefaultSignalHandler();

  auto args = parseArgs(argc, argv);
  auto graph = graph::deserialize(args.path);

  graph::Graph primary_path;
  graph::Graph secondary_path;

  auto res = algos::makeDfsRedundantTrees(
    args.trees,
    /* seed = */ 0,
    graph,
    args.vertex,
    args.depth,
    args.edge_skipping_policy,
    args.reset_depth,
    args.vertex_limit,
    args.greedy_size);

  if (!res.has_value()) {
    std::cerr << res.error() << "\n";
    return 1;
  }

  // primary_path = (*res)[0];
  // secondary_path = (*res)[1];

  auto diff = base_algo::subtract(base_algo::subtract(graph, primary_path), secondary_path);

  dot::DotSerializer serializer("algos");

  std::vector<std::string> colors = {
    "red",
    "greed",
    "blue",
    "orange",
    "purple"
  };


  for (int i = 0; i < res->size(); i++) {
    serializer.addGraph(res->at(i), "g_" + boost::lexical_cast<std::string>(i), colors[i % colors.size()]);
  }

  // serializer.addGraph(primary_path, "primary_path", "red");
  // serializer.addGraph(secondary_path, "secondary_path", "blue");
  // serializer.addGraph(diff, "diff", "grey");
  serializer.markVertex(args.vertex, "red");
  // serializer.addGraph(base_algo::intersect(primary_path, secondary_path), "intersection", "green");
  serializer.serialize(std::cout);

  std::ofstream score_file(args.score_file);

  INFO() << "Score is " << 0;
  INFO() << "Mean len 1 " << 0;
  INFO() << "Mean len 2 " << 0;
  score_file << 0 << ' ' << 0 << ' ' << 0 << ' ';

  INFO() << "Trees: " << (*res).size();

  std::vector<int> broken_numbers = {1, 2, 3, 5, 10, 15, 20};
  score_file << broken_numbers.size() << ' ';

  std::mt19937_64 rng(0);

  for (auto broken_number : broken_numbers) {
    auto attempts = 1000;

    double fails = 0;
    double disconnected_sum = 0;
    for (int i = 0; i < attempts; i++) {
      auto visited = base_algo::testConnectivity(graph, *res, broken_number, rng, args.vertex);

      if (visited < graph.vertices().size()) {
        fails++;
      }

      disconnected_sum += graph.vertices().size() - visited;
    }

    INFO() << "For " << broken_number << " broken links";
    INFO() << "Fail probability " << fails / attempts;
    INFO() << "Mean disconnected " << disconnected_sum / attempts;

    score_file << broken_number << ' ' << fails / attempts << ' ' << disconnected_sum / attempts << ' ';
  }

  for (auto tree : *res) {
    ASSERT_FATAL(tree.vertices().size() == graph.vertices().size());

    ASSERT_FATAL(base_algo::isRootedTree(tree, args.vertex));

    ASSERT_FATAL(base_algo::intersect(tree, graph) == tree);
  }
}
