#ifndef EDITOR_HH
#define EDITOR_HH

#include <list>

#include "board.hh"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Editor {
public:
    explicit Editor(Sandbox& sandbox, ComponentDatabase const& component_db);
    explicit Editor(json const& content, Sandbox& sandbox, ComponentDatabase const& component_db);

    [[nodiscard]] std::list<Board>& boards() { return boards_; }

    [[nodiscard]] json serialize() const;

    friend bool operator==(Editor const& lhs, Editor const& rhs) { return lhs.boards_ == rhs.boards_; }
    friend bool operator!=(Editor const& lhs, Editor const& rhs) { return !(lhs == rhs); }

private:
    Sandbox& sandbox_;
    ComponentDatabase const& component_db_;
    std::list<Board> boards_;
};

#endif //EDITOR_HH
