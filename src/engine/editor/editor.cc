#include "editor.hh"

Editor::Editor(ComponentDatabase const& component_db)
    : component_db_(component_db)
{
    boards_.emplace_back(10, 10, component_db_);
}
