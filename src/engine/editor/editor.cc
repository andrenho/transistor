#include "editor.hh"

#include <stdexcept>

Editor::Editor(ComponentDatabase const& component_db)
    : component_db_(component_db)
{
    boards_.emplace_back(20, 10, component_db_);
}

Editor::Editor(json const& content, ComponentDatabase const& component_db)
    : component_db_(component_db)
{
    for (auto const& jboard: content.at("boards"))
        boards_.emplace_back(jboard, component_db_);
}

Board& Editor::board(size_t id)
{
    for (auto& board : boards_)
        if (board.id() == id)
            return board;
    throw std::out_of_range("Board not found");
}

json Editor::serialize() const
{
    std::vector<json> jboards;
    jboards.resize(boards_.size());
    r::transform(boards_, jboards.begin(), [](Board const& board) { return board.serialize(); });
    return { { "boards", jboards } };
}
