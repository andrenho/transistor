#ifndef COMPILER_HH
#define COMPILER_HH

#include "engine/editor/board.hh"
#include "engine/layout/layout.hh"

namespace compiler {

Layout compile_to_layout(Board const& board);

};

#endif //COMPILER_HH
