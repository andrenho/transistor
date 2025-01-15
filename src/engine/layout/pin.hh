#ifndef PIN_HH
#define PIN_HH

#include <ostream>

#include "engine/board/component.hh"

struct Pin {
    Component const* component;
    uintpin_t        pin_no;
    bool             input;

    bool operator==(Pin const& p) const { return component == p.component && pin_no == p.pin_no; }
    friend std::ostream& operator<<(std::ostream& os, Pin const& pin) {
        os << "Component: " << pin.component << ", pin: " << (int) pin.pin_no << ", input: " << (pin.input ? "yes" : "no");
        return os;
    }
};

template<>
struct std::hash<Pin> {
    std::size_t operator()(Pin const& pin) const noexcept
    {
        return (std::hash<Component const*>{}(pin.component) << 1) ^ std::hash<uintpin_t>{}(pin.pin_no) << 1;
    }
};


#endif //PIN_HH
