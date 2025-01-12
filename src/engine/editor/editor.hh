#ifndef EDITOR_HH
#define EDITOR_HH

#include <list>

#include "board.hh"

#include <nlohmann/json.hpp>

#include "engine/sandbox/recompilation.hh"
using json = nlohmann::json;

class Editor {
public:
    explicit Editor(ComponentDatabase const& component_db, SandboxRecompilationFn recompile);
    explicit Editor(json const& content, ComponentDatabase const& component_db, SandboxRecompilationFn recompile);

    [[nodiscard]] std::list<Board>& boards() { return boards_; }
    Board& board(size_t);

    [[nodiscard]] json serialize() const;

    friend bool operator==(Editor const& lhs, Editor const& rhs) { return lhs.boards_ == rhs.boards_; }
    friend bool operator!=(Editor const& lhs, Editor const& rhs) { return !(lhs == rhs); }

private:
    ComponentDatabase const& component_db_;
    std::list<Board> boards_;
};

#endif //EDITOR_HH
