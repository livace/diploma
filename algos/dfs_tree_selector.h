#pragma once

#include "graph/graph.h"

namespace algos {

class DfsTreeSelector {
public:
  DfsTreeSelector(const graph::Graph& graph);

  void visit(graph::Vertex vertex);

  graph::Graph get();

private:
  std::unordered_set<graph::Vertex> visited_;
  graph::Graph graph_;

  graph::Graph tree_;
};

} // namespace algos
