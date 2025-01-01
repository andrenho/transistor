#ifndef PIN_HH
#define PIN_HH

#include "engine/editor/component.hh"

struct Pin {
    Component const* component;
    uintpin_t        pin_no;
    bool             input;

    bool operator==(Pin const& p) const { return component == p.component && pin_no == p.pin_no; }
};

template<>
struct std::hash<Pin> {
    std::size_t operator()(Pin const& pin) const noexcept
    {
        return (std::hash<Component const*>{}(pin.component) << 1) ^ std::hash<uintpin_t>{}(pin.pin_no) << 1;
    }
};


#endif //PIN_HH
