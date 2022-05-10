#include "dfs_redundant_trees.h"

#include "base_algo/subtract.h"
#include "base_algo/intersect.h"
#include "base_algo/without.h"
#include "base_algo/is_two_connected.h"

#include "util/expect.h"
#include "util/assert.h"

#include "dot/dot_serializer.h"

#include <iostream>

namespace algos {

namespace {

class DfsVisitor {
public:
  DfsVisitor(const graph::Graph& graph, bool check_connectivity)
    : graph_(graph)
    , check_connectivity_(check_connectivity)
  {}

  void visit(graph::Vertex vertex) {
    tree_.addVertex(vertex);
    visited_.insert(vertex);

    auto connections = graph_.inversedConnections(vertex);
    for (auto connection : connections) {
      if (!visited_.count(connection)) {
        auto new_graph = base_algo::without(graph_, {connection, vertex});
        ASSERT_FATAL(graph_.edges().size() == new_graph.edges().size() + 1);
        if (!check_connectivity_ || base_algo::isTwoConnected(new_graph)) {
          graph_ = new_graph;
          visit(connection);
          tree_.addEdge({connection, vertex});
        }
      }
    }
  }

  graph::Graph get() {
    return tree_;
  }

  graph::Graph remainingGraph() {
    return graph_;
  }

private:
  graph::Graph graph_;
  bool check_connectivity_;

  std::unordered_set<graph::Vertex> visited_;
  graph::Graph tree_;
};

} // namespace

util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> makeDfsTwoConnectedThreeRedundantTrees(
  const graph::Graph& graph,
  graph::Vertex root,
  int strict_cnt) {

  auto remaining_graph = graph;
  std::vector<graph::Graph> trees;

  for (size_t i = 0; i < 3; i++) {
    DfsVisitor visitor(remaining_graph, /* check_connectivity = */ i < strict_cnt);
    visitor.visit(root);
    auto tree = visitor.get();

    EXPECT(tree.vertices().size() == graph.vertices().size(), "Failed to get " << i + 1 << "-th tree, tree size " << tree.vertices().size() << ", graph size " << graph.vertices().size());

    remaining_graph = base_algo::subtract(remaining_graph, tree);

    if (i < strict_cnt) {
      dot::DotSerializer serializer("idk");
      serializer.addGraph(visitor.remainingGraph(), "remaining1", "red");
      serializer.addGraph(remaining_graph, "remaining2", "blue");
      serializer.serialize(std::cout);


      ASSERT_FATAL(base_algo::isTwoConnected(remaining_graph));
    }

    trees.push_back(std::move(tree));
  }

  return {{
    trees[0],
    trees[1],
    trees[2],
  }};
}

} // namespace algos
