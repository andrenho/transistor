#ifndef COMPILER_HH
#define COMPILER_HH

#include "engine/connections/connection.hh"
#include "engine/editor/board.hh"
#include "engine/layout/layout.hh"

namespace compiler {

Layout      compile_to_layout(Board const& board);
Connections compile_to_connections(std::vector<Layout> const& layout);

};

#endif //COMPILER_HH
