#pragma once

#include "graph/graph.h"

namespace dot {

class DotSerializer {
public:
  void addGraph(const graph::Graph& graph, std::string color = "");
  void markVertex(graph::Vertex vertex, std::string color);

  void serialize(std::ostream& ostream);

private:
  std::vector<std::string> lines_;

  size_t index_ = 0;
};

} // namespace dot
