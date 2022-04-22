#pragma once

#include "graph/graph.h"

#include <memory>

namespace base_algo {

class DfsListener {
public:
  virtual void onBeforeVertex(graph::Vertex vertex) {}
  virtual void onAfterVertex(graph::Vertex vertex) {}

  virtual ~DfsListener() = default;
};

std::unordered_map<graph::Vertex, bool> dfs(
  const graph::Graph& graph,
  graph::Vertex vertex,
  DfsListener* listener = nullptr);

std::unordered_map<graph::Vertex, bool> inverseDfs(
  const graph::Graph& graph,
  graph::Vertex vertex,
  DfsListener* listener = nullptr);

std::unordered_map<graph::Vertex, bool> dfsAll(
  const graph::Graph& graph,
  DfsListener* listener = nullptr);

std::unordered_map<graph::Vertex, bool> inverseDfsAll(
  const graph::Graph& graph,
  DfsListener* listener = nullptr);

} // namespace base_algo
