#include "dfs.h"

#include "util/assert.h"

namespace base_algo {

namespace {

class DfsVisitor {
public:
  DfsVisitor(const graph::Graph& graph, bool inversed) : graph_(graph), inversed_(inversed) {
    for (const auto& vertex : graph.vertices()) {
      visited_[vertex] = false;
    }
  }

  void visit(graph::Vertex vertex) {
    ASSERT_FATAL(visited_.count(vertex), "Failed to visit, vertex " << vertex << " is not in graph");

    if (visited_[vertex]) {
      return;
    }

    visited_[vertex] = true;

    auto connected = inversed_ ? graph_.inversedConnections(vertex) : graph_.connections(vertex);

    for (const auto& connection : connected) {
      visit(connection);
    }
  }

  std::unordered_map<graph::Vertex, bool> visited() const {
    return visited_;
  }

private:
  const graph::Graph& graph_;
  bool inversed_;

  std::unordered_map<graph::Vertex, bool> visited_;
};

}

std::unordered_map<graph::Vertex, bool> dfs(const graph::Graph& graph, graph::Vertex vertex) {
  auto visitor = DfsVisitor(graph, /* inversed = */ false);
  visitor.visit(vertex);
  return visitor.visited();
}


std::unordered_map<graph::Vertex, bool> inverseDfs(const graph::Graph& graph, graph::Vertex vertex) {
  auto visitor = DfsVisitor(graph, /* inversed = */ true);
  visitor.visit(vertex);
  return visitor.visited();
}

} // namespace base_algo
