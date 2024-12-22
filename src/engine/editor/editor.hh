#ifndef EDITOR_HH
#define EDITOR_HH

#include <vector>

#include "board.hh"

class Editor {
public:
    [[nodiscard]] std::vector<Board> const& boards() const { return boards_; }

private:
    std::vector<Board> boards_;
};

#endif //EDITOR_HH
