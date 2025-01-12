#include "editor.hh"

Editor::Editor(Sandbox& sandbox, ComponentDatabase const& component_db)
    : sandbox_(sandbox), component_db_(component_db)
{
    boards_.emplace_back(20, 10, sandbox_, component_db_);
}

Editor::Editor(json const& content, Sandbox& sandbox, ComponentDatabase const& component_db)
    : sandbox_(sandbox), component_db_(component_db)
{
    for (auto const& jboard: content.at("boards"))
        boards_.emplace_back(jboard, sandbox_, component_db_);
}

json Editor::serialize() const
{
    std::vector<json> jboards;
    jboards.resize(boards_.size());
    r::transform(boards_, jboards.begin(), [](Board const& board) { return board.serialize(); });
    return { { "boards", jboards } };
}
