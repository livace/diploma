#include "edge.h"

namespace graph {

Edge::Edge(const Vertex& from, const Vertex& to) : from_(from), to_(to) {}


const Vertex& Edge::from() const {
  return from_;
}

const Vertex& Edge::to() const {
  return to_;
}

Edge Edge::reversed() const {
  return {to_, from_};
}

bool operator==(const Edge& lhs, const Edge& rhs) {
  return lhs.from() == rhs.from() && lhs.to() == rhs.to();
}

} // namespace graph

std::size_t std::hash<graph::Edge>::operator()(const graph::Edge& edge) const noexcept {
  return std::hash<graph::Vertex>{}(edge.from()) * 31 + std::hash<graph::Vertex>{}(edge.to());
}
