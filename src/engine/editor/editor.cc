#include "editor.hh"

Editor::Editor(Sandbox& sandbox, ComponentDatabase const& component_db)
    : sandbox_(sandbox), component_db_(component_db)
{
    boards_.emplace_back(10, 10, sandbox_, component_db_);
}

json Editor::serialize() const
{
    std::vector<json> jboards;
    jboards.resize(boards_.size());
    r::transform(boards_, jboards.begin(), [](Board const& board) { return board.serialize(); });
    return { { "boards", jboards } };
}
