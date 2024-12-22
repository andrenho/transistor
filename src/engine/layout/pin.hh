#ifndef PIN_HH
#define PIN_HH

#include "engine/editor/component.hh"

struct Pin {
    Component* component;
    uint8_t    pin_no;
};

#endif //PIN_HH
