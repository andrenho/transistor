#ifndef UI_COMMANDS_HH
#define UI_COMMANDS_HH

#include <stdexcept>
#include <variant>

#include "engine/game/game.hh"

struct DeviceEditor;

namespace U {

struct StartDragginDevice { DeviceEditor* device_editor; };
struct StopDraggingDevice {};

struct ShowException {
    std::string text;
    bool        recoverable = false;
};
struct ClearException {};
struct SetInfoboxText { std::string text; };

struct SetMouseCursor {
    enum MouseCursor { Normal, Delete };
    MouseCursor cursor;
};

struct Quit {};

using Command = std::variant<
    StartDragginDevice, StopDraggingDevice,
    ShowException, ClearException, SetInfoboxText,
    SetMouseCursor, Quit
>;

}

#endif //UI_COMMANDS_HH
