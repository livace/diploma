#include "gadag_redundant_trees.h"

#include "adag_selection.h"

#include "base_algo/is_rooted_tree.h"
#include "base_algo/subtract.h"

#include "util/assert.h"

#include <queue>

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


graph::Graph buildTree(const graph::Graph& gadag, graph::Vertex root, bool inverse) {
  graph::Graph tree;

  std::queue<graph::Vertex> queue;
  std::unordered_set<graph::Vertex> visited;
  queue.push(root);
  visited.insert(root);
  while (!queue.empty()) {
    auto vertex = queue.front();
    tree.addVertex(vertex);
    queue.pop();

    const auto& connections = inverse ? gadag.connections(vertex) : gadag.inversedConnections(vertex);
    for (auto connection : connections) {
      if (!visited.count(connection)) {
        visited.insert(connection);
        queue.push(connection);
        tree.addEdge({connection, vertex});
      }
    }
  }

  return tree;
}


} // namespace

util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> makeDfsGadagThreeRedundantTrees(
  const graph::Graph& graph,
  graph::Vertex root
) {
  DfsVisitor visitor(graph);
  visitor.visit(root);
  auto dfs_tree = visitor.get();

  EXPECT(dfs_tree.vertices().size() == graph.vertices().size(),
    "Failed to get dfs tree, tree size " <<
    dfs_tree.vertices().size() <<
    ", graph size " <<
    graph.vertices().size());

  auto remaining_graph = base_algo::subtract(graph, dfs_tree);

  auto selector = baseline::AdagSelector(remaining_graph, root);

  {
    auto res = selector.build();
    PROPAGATE_ERROR(res);
  }

  auto gadag = selector.get();

  auto primary_tree = buildTree(gadag, root, /* inverse */ false);
  ASSERT_FATAL(base_algo::isRootedTree(primary_tree, root));
  auto secondary_tree = buildTree(gadag, root, /* inverse */ true);
  ASSERT_FATAL(base_algo::isRootedTree(secondary_tree, root));

  return {{
    dfs_tree,
    primary_tree,
    secondary_tree,
  }};
}

} // namespace baseline
