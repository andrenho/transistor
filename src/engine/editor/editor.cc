#include "editor.hh"

Editor::Editor(Sandbox& sandbox, ComponentDatabase const& component_db)
    : sandbox_(sandbox), component_db_(component_db)
{
    boards_.emplace_back(10, 10, sandbox_, component_db_);
}
