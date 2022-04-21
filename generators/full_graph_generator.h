#pragma once

#include "generator.h"

#include <cstddef>

namespace generator {

class FullGraphGenerator : public Generator {
public:
  FullGraphGenerator(size_t n);
  graph::Graph generate() override;

private:
  size_t n_;
};

} // namespace generator
