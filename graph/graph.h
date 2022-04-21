#pragma once

#include "vertex.h"
#include "edge.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace graph {

class Graph {
public:
  Graph(const std::unordered_map<Vertex, std::unordered_set<Vertex>>& connectivity_list);

  const std::vector<Vertex>& vertices() const;
  const std::vector<Edge>& edges() const;

  const std::unordered_set<Vertex>& connections(const Vertex& v) const;
  const std::unordered_set<Vertex>& inversedConnections(const Vertex& v) const;

private:
  std::vector<Vertex> vertices_;

  std::unordered_map<Vertex, std::unordered_set<Vertex>> connectivity_list_;
  std::unordered_map<Vertex, std::unordered_set<Vertex>> inverse_connectivity_list_;

  std::vector<Edge> edges_;
};

}
