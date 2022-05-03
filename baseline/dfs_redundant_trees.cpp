#include "dfs_redundant_trees.h"

#include "base_algo/subtract.h"
#include "base_algo/intersect.h"

#include "util/expect.h"
#include "util/assert.h"

#include <iostream>

namespace baseline {

namespace {

class DfsVisitor {
public:
  DfsVisitor(const graph::Graph& graph) : graph_(graph) {}

  void visit(graph::Vertex vertex) {
    tree_.addVertex(vertex);
    visited_.insert(vertex);

    for (auto connection : graph_.inversedConnections(vertex)) {
      if (!visited_.count(connection)) {
        visit(connection);

        tree_.addEdge({connection, vertex});
      }
    }
  }

  graph::Graph get() {
    return tree_;
  }

private:
  std::unordered_set<graph::Vertex> visited_;
  graph::Graph graph_;

  graph::Graph tree_;
};

} // namespace

util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> makeDfsThreeRedundantTrees(
  const graph::Graph& graph,
  graph::Vertex root) {

  auto remaining_graph = graph;
  std::vector<graph::Graph> trees;
  for (size_t i = 0; i < 3; i++) {
    DfsVisitor visitor(remaining_graph);
    visitor.visit(root);
    auto tree = visitor.get();

    EXPECT(tree.vertices().size() == graph.vertices().size(), "Failed to get " << i + 1 << "-th tree, tree size " << tree.vertices().size() << ", graph size " << graph.vertices().size());

    remaining_graph = base_algo::subtract(remaining_graph, tree);

    trees.push_back(std::move(tree));
  }

  return {{
    trees[0],
    trees[1],
    trees[2],
  }};
}

} // namespace baseline
