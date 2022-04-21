#pragma once

#include "graph.h"

#include <filesystem>

namespace graph {

void serialize(std::ostream& stream, const Graph& graph);
void serialize(const std::filesystem::path& filename, const Graph& graph);

Graph deserialize(std::istream& stream);
Graph deserialize(const std::filesystem::path& filename);

} // namespace graph
