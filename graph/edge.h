#pragma once

#include "vertex.h"

#include <functional>

namespace graph {

class Edge {
public:
  Edge(const Vertex& from, const Vertex& to);

  const Vertex& from() const;
  const Vertex& to() const;

  Edge reversed() const;

private:
  Vertex from_;
  Vertex to_;
};

bool operator==(const Edge& lhs, const Edge& rhs);

} // namespace graph

template<>
struct std::hash<graph::Edge> {
  std::size_t operator()(const graph::Edge& edge) const noexcept;
};
