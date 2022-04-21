#pragma once

#include <functional>
#include <istream>
#include <ostream>

namespace graph {

class Vertex {
public:
  Vertex();
  Vertex(int id);

  int id() const;

private:
  int id_;
};

bool operator<(const Vertex& lhs, const Vertex& rhs);
bool operator==(const Vertex& lhs, const Vertex& rhs);

} // namespace graph

template<>
struct std::hash<graph::Vertex> {
  std::size_t operator()(const graph::Vertex& vertex) const noexcept; // {
  //   return std::hash<int>{}(vertex.id());
  // }
};

std::ostream& operator<<(std::ostream& os, const graph::Vertex& vertex);
std::istream& operator>>(std::istream& is, graph::Vertex& vertex);
