#include "build_gadag_tree.h"

#include "util/assert.h"

#include <queue>

namespace algos {

graph::Graph buildGadagTree(const graph::Graph& gadag, graph::Vertex root, bool inverse) {
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

        if (inverse) {
          ASSERT_FATAL(gadag.has({vertex, connection}));
        } else {
          ASSERT_FATAL(gadag.has({connection, vertex}));
        }
      }
    }
  }

  return tree;
}

} // namespace algos
