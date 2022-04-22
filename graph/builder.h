#pragma once

#include "graph.h"

#include <unordered_map>
#include <unordered_set>

namespace graph {

class Builder {
public:
  void addVertex(Vertex vertex);
  void addEdge(Edge edge);

  Graph build();

private:
  std::unordered_map<Vertex, std::unordered_set<Vertex>> connectivity_list_;
};

} // namespace graph
