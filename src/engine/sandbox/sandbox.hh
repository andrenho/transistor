#ifndef SANDBOX_HH
#define SANDBOX_HH

#include "engine/componentdb/componentdb.hh"
#include "engine/connections/connection.hh"
#include "engine/editor/editor.hh"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Sandbox {
public:
    Sandbox();
    explicit Sandbox(json const& content);

    [[nodiscard]] ComponentDatabase& component_db() { return component_db_; }
    [[nodiscard]] Editor&            editor() { return editor_; }

    void reset();
    void simulate();

    [[nodiscard]] bus_data_t wire_value(Position const& pos) const;

    [[nodiscard]] json serialize() const;

    friend bool operator==(Sandbox const& lhs, Sandbox const& rhs) { return lhs.editor_ == rhs.editor_; }
    friend bool operator!=(Sandbox const& lhs, Sandbox const& rhs) { return !(lhs == rhs); }

private:
    ComponentDatabase component_db_;
    Editor            editor_;

    Connections                              connections_cache_;
    std::vector<Component*>                  components_cache_;
    std::unordered_map<Position, bus_data_t> wire_value_cache_;
};

#endif //SANDBOX_HH
