#include "dfs.h"

#include "util/assert.h"

namespace base_algo {

namespace {

class DfsVisitor {
public:
  DfsVisitor(
    const graph::Graph& graph,
    bool inversed,
    DfsListener* listener)
    : graph_(graph)
    , inversed_(inversed)
    , listener_(std::move(listener)) {
    for (const auto& vertex : graph.vertices()) {
      visited_[vertex] = false;
    }
  }

  void visit(graph::Vertex vertex) {
    ASSERT_FATAL(visited_.count(vertex), "Failed to visit, vertex " << vertex << " is not in graph");

    if (visited_[vertex]) {
      return;
    }

    if (listener_ != nullptr) {
      listener_->onBeforeVertex(vertex);
    }

    visited_[vertex] = true;

    auto connected = inversed_ ? graph_.inversedConnections(vertex) : graph_.connections(vertex);

    for (const auto& connection : connected) {
      visit(connection);
    }

    if (listener_ != nullptr) {
      listener_->onAfterVertex(vertex);
    }
  }

  std::unordered_map<graph::Vertex, bool> visited() const {
    return visited_;
  }

private:
  const graph::Graph& graph_;
  bool inversed_;
  DfsListener* listener_;

  std::unordered_map<graph::Vertex, bool> visited_;
};

} // namespace

std::unordered_map<graph::Vertex, bool> dfs(
  const graph::Graph& graph,
  graph::Vertex vertex,
  DfsListener* listener) {
  auto visitor = DfsVisitor(graph, /* inversed = */ false, std::move(listener));
  visitor.visit(vertex);
  return visitor.visited();
}

std::unordered_map<graph::Vertex, bool> inverseDfs(
  const graph::Graph& graph,
  graph::Vertex vertex,
  DfsListener* listener) {
  auto visitor = DfsVisitor(graph, /* inversed = */ true, std::move(listener));
  visitor.visit(vertex);
  return visitor.visited();
}

std::unordered_map<graph::Vertex, bool> dfsAll(
  const graph::Graph& graph,
  DfsListener* listener) {
  auto visitor = DfsVisitor(graph, /* inversed = */ false, std::move(listener));
  for (auto vertex : graph.vertices()) {
    visitor.visit(vertex);
  }
  return visitor.visited();
}

std::unordered_map<graph::Vertex, bool> inverseDfsAll(
  const graph::Graph& graph,
  DfsListener* listener) {
  auto visitor = DfsVisitor(graph, /* inversed = */ true, std::move(listener));
  for (auto vertex : graph.vertices()) {
    visitor.visit(vertex);
  }
  return visitor.visited();
}

} // namespace base_algo
