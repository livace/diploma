#pragma once

#include "graph/graph.h"
#include "graph/builder.h"
#include "util/expect.h"

namespace algos {

class AdagSelector {
public:
  AdagSelector(const graph::Graph& graph, graph::Vertex vertex);

  util::Expected<void, util::Error> build();

  graph::Graph get();
  graph::Graph getDfsTree();
  std::unordered_map<graph::Vertex, graph::Vertex> localRoot();

private:
  bool dfs(graph::Vertex vertex);
  void selectGadag();

private:
  int current_depth_ = 0;

  std::unordered_map<graph::Vertex, int> depth_;
  std::unordered_map<graph::Vertex, int> lowpoint_number_;
  std::unordered_map<graph::Vertex, graph::Vertex> lowpoint_source_;
  std::unordered_map<graph::Vertex, graph::Vertex> parent_;
  std::unordered_map<graph::Vertex, bool> visited_;

  graph::Graph dfs_tree_;

  std::unordered_map<graph::Vertex, graph::Vertex> local_root_;
  std::unordered_set<graph::Vertex> ready_;

  graph::Graph graph_;
  graph::Vertex root_;

  graph::Graph gadag_;
};

} // namespace algos
