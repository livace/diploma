#include "edge.h"

namespace graph {

Edge::Edge(const Vertex& from, const Vertex& to) : from_(from), to_(to) {}


const Vertex& Edge::from() const {
  return from_;
}

const Vertex& Edge::to() const {
  return to_;
}


} // namespace graph
