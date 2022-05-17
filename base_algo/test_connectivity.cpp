#include "test_connectivity.h"

namespace base_algo {

namespace {

class DfsVisitor {
public:
  DfsVisitor(
    const std::vector<graph::Graph>& trees,
    const std::unordered_set<graph::Edge>& broken
  )
  : trees_(trees)
  , broken_(broken)
  {}

  void visit(const graph::Vertex& vertex) {
    if (visited_.count(vertex)) {
      return;
    }

    visited_.insert(vertex);

    for (const auto& tree : trees_) {
      for (const auto& connection : tree.inversedConnections(vertex)) {
        if (!broken_.count({connection, vertex}) && !broken_.count({vertex, connection})) {
          visit(connection);
        }
      }
    }
  }

  size_t visited() {
    return visited_.size();
  }

private:
  std::unordered_set<graph::Vertex> visited_;

  const std::vector<graph::Graph>& trees_;
  const std::unordered_set<graph::Edge>& broken_;
};

} // namespace


size_t testConnectivity(
  const graph::Graph& initial_graph,
  const std::vector<graph::Graph>& trees,
  size_t num_broken,
  std::mt19937_64& rng,
  graph::Vertex root) {

  if (num_broken * 2 >= initial_graph.edges().size()) {
    return 1;
  }

  std::unordered_set<graph::Edge> broken;

  while (broken.size() < num_broken) {
    std::uniform_int_distribution<std::mt19937_64::result_type> edge_dist(0, initial_graph.edges().size() - 1);
    auto edge_index = edge_dist(rng);

    auto random_edge = initial_graph.edges()[edge_index];

    if (!broken.count(random_edge) && !broken.count(random_edge.reversed())) {
      broken.insert(random_edge);
    }
  }

  DfsVisitor visitor(
    trees,
    broken
  );

  visitor.visit(root);

  return visitor.visited();
}

}
