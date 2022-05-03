#include "vertex.h"

namespace graph {

Vertex::Vertex() : id_(0) {}
Vertex::Vertex(int id) : id_(id) {}

int Vertex::id() const {
  return id_;
}

bool operator<(const Vertex& lhs, const Vertex& rhs) {
  return lhs.id() < rhs.id();
}

bool operator==(const Vertex& lhs, const Vertex& rhs) {
  return lhs.id() == rhs.id();
}

} // namespace graph

std::size_t std::hash<graph::Vertex>::operator()(const graph::Vertex& vertex) const noexcept {
  return std::hash<int>{}(vertex.id());
}

std::ostream& operator<<(std::ostream& os, graph::Vertex vertex) {
  return os << vertex.id();
}

std::istream& operator>>(std::istream& is, graph::Vertex& vertex) {
  int id;
  is >> id;
  vertex = graph::Vertex(id);

  return is;
}
