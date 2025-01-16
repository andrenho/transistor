#ifndef UI_COMMANDS_HH
#define UI_COMMANDS_HH

#include <stdexcept>
#include <variant>

#include "engine/game/game.hh"
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

struct Quit {};

using Command = std::variant<
    StartDragginDevice, StopDraggingDevice,
    ShowException, ClearException, SetInfobox,
    SetMouseCursor, Quit
>;

}

#endif //UI_COMMANDS_HH
