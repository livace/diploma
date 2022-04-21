#pragma once

#include "graph.h"

namespace graph {

class Builider {
public:
  void addEdge(int from, int to);
  Graph build();

private:
  std::map<int, std::vector<int>> connectivity_list_;
};

} // namespace graph
