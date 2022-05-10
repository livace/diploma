#include "dot_serializer.h"

#include "graph/vertex.h"

#include <boost/lexical_cast.hpp>

#include <iostream>

namespace dot {

DotSerializer::DotSerializer(std::string name) : name_(name) {}

void DotSerializer::addGraph(const graph::Graph& graph, std::string name, std::string color) {
  index_++;
  lines_.push_back("subgraph G_" + name + " {");

  lines_.push_back("edge [color = " + color + "]");

  for (auto edge : graph.edges()) {
    if (graph.has({edge.to(), edge.from()})) {
      if (!(edge.from() < edge.to())) {
        lines_.push_back("\"" + boost::lexical_cast<std::string>(edge.from().id()) + "\"" + " -> " + "\"" + boost::lexical_cast<std::string>(edge.to().id()) + "\"" + " [dir = none]");
      }
    } else {
      lines_.push_back("\"" + boost::lexical_cast<std::string>(edge.from().id()) + "\"" + " -> " + "\"" + boost::lexical_cast<std::string>(edge.to().id()) + "\"");
    }
  }

  lines_.push_back("}");
  lines_.push_back("");
}

void DotSerializer::markVertex(graph::Vertex vertex, std::string color) {
  lines_.push_back(boost::lexical_cast<std::string>(vertex.id()) + " [color = " + color + "]");
  lines_.push_back("");
}

void DotSerializer::serialize(std::ostream& ostream) {
  ostream << "digraph GG_" << name_ << " {\n";
  ostream << "\n";

  for (const auto& line : lines_) {
    ostream << line << "\n";
  }

  ostream << "}\n";
}

} // namespace dot
