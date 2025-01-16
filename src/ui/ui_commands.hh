#ifndef UI_COMMANDS_HH
#define UI_COMMANDS_HH

#include <stdexcept>
#include <variant>

#include "engine/game/game.hh"

struct DeviceEditor;

namespace U {

struct StartDragginDevice { DeviceEditor* device_editor; };
struct StopDraggingDevice {};

struct ShowException { std::exception exception; };

struct SetMouseCursor {
    enum MouseCursor { Normal, Delete };
    MouseCursor cursor;
};

struct Quit {};

using Command = std::variant<
    StartDragginDevice, StopDraggingDevice,
    ShowException,
    SetMouseCursor, Quit
>;

}

#endif //UI_COMMANDS_HH
