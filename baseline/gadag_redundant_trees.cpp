#include "gadag_redundant_trees.h"

#include "adag_selection.h"

#include "base_algo/is_rooted_tree.h"
#include "base_algo/subtract.h"

#include "util/assert.h"

#include <queue>

namespace baseline {

namespace {

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

std::pair<graph::Graph, graph::Graph> makeTwoRedundantTrees(
  const graph::Graph& gadag,
  graph::Vertex root
) {
  auto primary_tree = buildTree(gadag, root, /* inverse */ false);
  ASSERT_FATAL(base_algo::isRootedTree(primary_tree, root));
  auto secondary_tree = buildTree(gadag, root, /* inverse */ true);
  ASSERT_FATAL(base_algo::isRootedTree(secondary_tree, root));

  return {
    primary_tree,
    secondary_tree
  };
}

util::Expected<std::tuple<graph::Graph, graph::Graph, graph::Graph>, std::string> makeGadagThreeRedundantTrees(
  const graph::Graph& graph,
  graph::Vertex root
) {
  auto selector = baseline::AdagSelector(graph, root);

  {
    auto res = selector.build();
    PROPAGATE_ERROR(res);
  }

  auto gadag = selector.get();

  auto primary_tree = buildTree(gadag, root, /* inverse */ false);
  ASSERT_FATAL(base_algo::isRootedTree(primary_tree, root));
  auto secondary_tree = buildTree(gadag, root, /* inverse */ true);
  ASSERT_FATAL(base_algo::isRootedTree(secondary_tree, root));

  auto remaining_graph = base_algo::subtract(base_algo::subtract(graph, primary_tree), secondary_tree);

  auto third_tree = buildTree(remaining_graph, root, /* inverse */ true);

  EXPECT(third_tree.vertices().size() == graph.vertices().size(), "Failed to select third tree");

  return {{
    primary_tree,
    secondary_tree,
    third_tree
  }};
}

} // namespace baseline
