#include "graph.h"

#include "util/assert.h"

namespace graph {

Graph::Graph() {}

Graph::Graph(const std::unordered_map<Vertex, std::unordered_set<Vertex>>& connectivity_list) {
  for (const auto& [from, connections] : connectivity_list) {
    addVertex(from);

    for (const auto& to : connections) {
      addEdge({from, to});
    }
  }
}

const std::vector<Vertex>& Graph::vertices() const {
  return vertices_;
}

const std::vector<Edge>& Graph::edges() const {
  return edges_;
}

const std::unordered_set<Vertex>& Graph::connections(const Vertex& v) const {
  if (!connectivity_list_.count(v)) {
    ASSERT_FATAL(false, "No such vertex " << v << " in graph");
  }
  return connectivity_list_.at(v);
}

const std::unordered_set<Vertex>& Graph::inversedConnections(const Vertex& v) const {
  if (!inverse_connectivity_list_.count(v)) {
    ASSERT_FATAL(false, "No such vertex " << v << " in graph");
  }
  return inverse_connectivity_list_.at(v);
}

bool Graph::has(Vertex vertex) const {
  return connectivity_list_.count(vertex);
}

bool Graph::has(const Edge& edge) const {
  if (!has(edge.from())) {
    return false;
  }

  return connectivity_list_.at(edge.from()).count(edge.to());
}

void Graph::addEdge(Edge edge) {
  addVertex(edge.from());
  addVertex(edge.to());

  connectivity_list_[edge.from()].insert(edge.to());
  inverse_connectivity_list_[edge.to()].insert(edge.from());

  edges_.push_back(edge);
}

void Graph::addVertex(Vertex vertex) {
  if (!connectivity_list_.count(vertex)) {
    connectivity_list_[vertex];
    inverse_connectivity_list_[vertex];
    vertices_.push_back(vertex);
  }
}

bool operator==(const Graph& lhs, const Graph& rhs) {
  return lhs.vertices() == rhs.vertices() && lhs.edges() == rhs.edges();
}

} // namespace graph
