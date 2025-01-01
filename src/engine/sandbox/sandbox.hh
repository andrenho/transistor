#ifndef SANDBOX_HH
#define SANDBOX_HH

#include "engine/componentdb/componentdb.hh"
#include "engine/connections/connection.hh"
#include "engine/editor/editor.hh"

class Sandbox {
public:
    [[nodiscard]] ComponentDatabase& component_db() { return component_db_; }
    [[nodiscard]] Editor&            editor() { return editor_; }

    void on_update();
    void simulate();

private:
    ComponentDatabase component_db_;
    Editor            editor_ { *this, component_db_ };

    Connections                              connections_cache_;
    std::vector<Component*>                  components_cache_;
};

#endif //SANDBOX_HH
