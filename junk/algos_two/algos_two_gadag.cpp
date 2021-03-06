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
  ).parse(argc, argv);

  return result;
}

int main(int argc, char** argv) {
  util::initDefaultSignalHandler();

  auto args = parseArgs(argc, argv);
  auto graph = graph::deserialize(args.path);

  graph::Graph primary_path;
  graph::Graph secondary_path;

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


  double sum_len_1 = 0;
  double sum_len_2 = 0;

  for (auto vertex : graph.vertices()) {
    sum_len_1 += base_algo::distance(primary_path, vertex, args.vertex);
    sum_len_2 += base_algo::distance(secondary_path, vertex, args.vertex);
  }

  INFO() << "Score is " << score;
  INFO() << "Mean len 1 " << sum_len_1 / graph.vertices().size();
  INFO() << "Mean len 2 " << sum_len_2 / graph.vertices().size();

  score_file << score << ' ' << sum_len_1 / graph.vertices().size() << ' ' << sum_len_2 / graph.vertices().size() << ' ';

  std::vector<int> broken_numbers = {1, 2, 3, 5, 10, 15, 20};
  score_file << broken_numbers.size() << ' ';

  std::mt19937_64 rng(0);
  for (auto broken_number : broken_numbers) {
    auto attempts = 1000;

    double fails = 0;
    double disconnected_sum = 0;
    for (int i = 0; i < attempts; i++) {
      auto visited = base_algo::testConnectivity(graph, {primary_path, secondary_path}, broken_number, rng, args.vertex);

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

  ASSERT_FATAL(primary_path.vertices().size() == graph.vertices().size());
  ASSERT_FATAL(secondary_path.vertices().size() == graph.vertices().size());

  ASSERT_FATAL(base_algo::isRootedTree(primary_path, args.vertex));
  ASSERT_FATAL(base_algo::isRootedTree(secondary_path, args.vertex));

  ASSERT_FATAL(base_algo::intersect(primary_path, graph) == primary_path);
  ASSERT_FATAL(base_algo::intersect(secondary_path, graph) == secondary_path);
}
