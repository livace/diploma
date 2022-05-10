#pragma once

#include "vertex.h"

namespace graph {

class Edge {
public:
  Edge(const Vertex& from, const Vertex& to);

  const Vertex& from() const;
  const Vertex& to() const;

private:
  Vertex from_;
  Vertex to_;
};

bool operator==(const Edge& lhs, const Edge& rhs);

} // namespace graph
