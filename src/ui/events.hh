#ifndef EVENTS_HH
#define EVENTS_HH

#include <deque>
#include <variant>

struct DeviceEditor;

namespace event {

struct StartDragging {
    DeviceEditor *layer;
};

struct StopDragging {};

struct SetMouseCursor {
    enum MouseCursor { Normal, Delete };
    MouseCursor cursor;
};

}

using Event = std::variant<event::StartDragging, event::StopDragging, event::SetMouseCursor>;
using Events = std::deque<Event>;

#endif //EVENTS_HH
