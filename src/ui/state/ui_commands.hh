#ifndef UI_COMMANDS_HH
#define UI_COMMANDS_HH

#include <variant>

#include "uistate.hh"

struct DeviceEditor;

namespace U {

struct StartDragginDevice { DeviceEditor* device_editor; };
struct StopDraggingDevice {};

struct ShowException {
    std::string text;
    bool        recoverable = false;
};
struct ClearException {};

struct SetMouseCursor {
    enum MouseCursor { Normal, Delete };
    MouseCursor cursor;
};

struct SelectComponent {
    std::string name;
    Direction   dir = Direction::N;
};

struct Quit {};

using Command = std::variant<
    StartDragginDevice, StopDraggingDevice,
    ShowException, ClearException, SelectComponent,
    SetMouseCursor, Quit
>;

}

#endif //UI_COMMANDS_HH
