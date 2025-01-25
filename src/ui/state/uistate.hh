#ifndef UISTATE_HH
#define UISTATE_HH

#include <optional>

#include "engine/geometry/direction.hh"

struct UIState {
    std::optional<std::string> exception {};
    bool                       exception_recoverable = false;
    std::optional<std::string> selected_component {};
    Direction                  selected_tool_direction = Direction::N;
};

#endif //UISTATE_HH
