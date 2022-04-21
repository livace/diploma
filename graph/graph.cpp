#include "graph.h"

namespace graph {

Graph::Graph(const std::unordered_map<Vertex, std::unordered_set<Vertex>>& connectivity_list) {
  for (const auto& [from, connections] : connectivity_list) {
    // init empty set
    connectivity_list_[from];

    for (const auto& to : connections) {
      connectivity_list_[from].insert(to);
      inverse_connectivity_list_[to].insert(from);

      edges_.emplace_back(from, to);
    }

    vertices_.push_back(from);
  }
}

const std::vector<Vertex>& Graph::vertices() const {
  return vertices_;
}

const std::vector<Edge>& Graph::edges() const {
  return edges_;
}

const std::unordered_set<Vertex>& Graph::connections(const Vertex& v) const {
  return connectivity_list_.at(v);
}

const std::unordered_set<Vertex>& Graph::inversedConnections(const Vertex& v) const {
  return inverse_connectivity_list_.at(v);
}

} // namespace graph
