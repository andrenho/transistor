#ifndef UISTATE_HH
#define UISTATE_HH

#include <optional>

#include "engine/geometry/direction.hh"
#include "gui/infobox.hh"

enum class SelectedTool {
    Nothing, VCC, Button, LED, NPN, PNP,
};

struct UIState {
    InfoboxContents            infobox_contents;
    std::optional<std::string> exception {};
    bool                       exception_recoverable = false;
    SelectedTool               selected_tool = SelectedTool::Nothing;
    Direction                  selected_tool_direction = Direction::N;
};

#endif //UISTATE_HH
