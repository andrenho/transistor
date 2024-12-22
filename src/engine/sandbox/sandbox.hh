#ifndef SANDBOX_HH
#define SANDBOX_HH

#include "engine/componentdb/componentdb.hh"
#include "engine/editor/editor.hh"

class Sandbox {
public:
    [[nodiscard]] ComponentDatabase& component_db() { return component_db_; }
    [[nodiscard]] Editor&            editor() { return editor_; }

private:
    ComponentDatabase component_db_;
    Editor            editor_;
};

#endif //SANDBOX_HH
