#ifndef UISTATE_HH
#define UISTATE_HH

#include <optional>

#include "tools.hh"
#include "engine/geometry/direction.hh"

struct UIState {
    std::optional<std::string> exception {};
    bool                       exception_recoverable = false;
    Tools::Type        selected_tool = Tools::Type::Nothing;
    Direction                  selected_tool_direction = Direction::N;
};

#endif //UISTATE_HH
