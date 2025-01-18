#ifndef UI_COMMANDS_HH
#define UI_COMMANDS_HH

#include <variant>

#include "uistate.hh"
#include "gui/infobox.hh"

struct DeviceEditor;

namespace U {

struct StartDragginDevice { DeviceEditor* device_editor; };
struct StopDraggingDevice {};

struct ShowException {
    std::string text;
    bool        recoverable = false;
};
struct ClearException {};
struct SetInfobox { InfoboxContents contents; };

struct SetMouseCursor {
    enum MouseCursor { Normal, Delete };
    MouseCursor cursor;
};

struct SelectTool {
    SelectedTool tool;
    Direction    dir = Direction::N;
};

struct Quit {};

using Command = std::variant<
    StartDragginDevice, StopDraggingDevice,
    ShowException, ClearException, SetInfobox, SelectTool,
    SetMouseCursor, Quit
>;

}

#endif //UI_COMMANDS_HH
