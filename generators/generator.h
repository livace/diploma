#pragma once

#include "graph/graph.h"

namespace generator {

class Generator {
public:
  virtual graph::Graph generate() = 0;
};

} // namespace generator
