#pragma once

#include <functional>
#include <istream>
#include <ostream>

namespace graph {

class Vertex {
public:
  Vertex();
  explicit Vertex(std::string id);

  std::string id() const;

private:
  std::string id_;
};

bool operator<(const Vertex& lhs, const Vertex& rhs);
bool operator==(const Vertex& lhs, const Vertex& rhs);

} // namespace graph

template<>
struct std::hash<graph::Vertex> {
  std::size_t operator()(const graph::Vertex& vertex) const noexcept;
};

std::ostream& operator<<(std::ostream& os, graph::Vertex vertex);
std::istream& operator>>(std::istream& is, graph::Vertex& vertex);
