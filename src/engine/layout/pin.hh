#ifndef PIN_HH
#define PIN_HH

#include "engine/editor/component.hh"

struct Pin {
    Component const* component;
    uintpin_t  pin_no;

    bool operator==(Pin const& p) { return component == p.component && pin_no == p.pin_no; }
};

#endif //PIN_HH
