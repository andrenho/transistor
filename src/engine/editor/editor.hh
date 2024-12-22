#ifndef EDITOR_HH
#define EDITOR_HH

#include <list>

#include "board.hh"

class Editor {
public:
    explicit Editor(ComponentDatabase const& component_db);

    [[nodiscard]] std::list<Board>& boards() { return boards_; }

private:
    ComponentDatabase const& component_db_;
    std::list<Board> boards_;
};

#endif //EDITOR_HH
