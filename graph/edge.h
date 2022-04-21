#pragma once

#include "vertex.h"

namespace graph {

class Edge {
public:
  Edge(const Vertex& from, const Vertex& to);

  const Vertex& from() const;
  const Vertex& to() const;

private:
  const Vertex from_;
  const Vertex to_;
};

} // namespace graph
