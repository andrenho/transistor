#ifndef EDITOR_HH
#define EDITOR_HH

#include <list>

#include "board.hh"

class Editor {
public:
    [[nodiscard]] std::list<Board>& boards() { return boards_; }

private:
    std::list<Board> boards_ { { 10, 10 } };  // contains one empty board at startup
};

#endif //EDITOR_HH
