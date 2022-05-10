#include "is_two_connected.h"

#include "graph/graph.h"

#include "base_algo/is_connected.h"

#include "util/assert.h"

namespace base_algo {

namespace {

class DfsVisitor {
public:
  DfsVisitor(
    const graph::Graph& graph)
    : graph_(graph){
  }

  bool check() {
    for (auto vertex : graph_.vertices()) {
      orderDfs(vertex);
    }
    ASSERT_FATAL(order_.size() == graph_.vertices().size());

    visited_.clear();

    componentDfs(order_.back());

    return visited_.size() == graph_.vertices().size();
  }

private:
  void orderDfs(graph::Vertex& vertex) {
    if (visited_.count(vertex)) {
      return;
    }
    visited_.insert(vertex);

    for (auto connection : graph_.connections(vertex)) {
      orderDfs(connection);
    }

    order_.push_back(vertex);
  }

  void componentDfs(graph::Vertex& vertex) {
    if (visited_.count(vertex)) {
      return;
    }
    visited_.insert(vertex);

    for (auto connection : graph_.inversedConnections(vertex)) {
      componentDfs(connection);
    }
  }

  const graph::Graph& graph_;

  std::vector<graph::Vertex> order_;

  std::unordered_set<graph::Vertex> visited_;
};

} // namespace

bool isTwoConnected(graph::Graph graph) {
  return isConnected(graph) && DfsVisitor(graph).check();
}

int connectivityNumber(graph::Graph graph) {
  ASSERT_FATAL(false, "not implemented");
}

}
