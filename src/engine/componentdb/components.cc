#include "components.hh"

#include "engine/editor/component.hh"
#include "rendercontext.hh"

namespace component {

ComponentDefinition button()
{
    return {
        .name = "button",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = false,
        .data_size = 1,
        .on_click = [](Component& button) {
            button.data[0] = (!button.data[0]) & 1;
        },
        .simulate = [](Component& button) {
            button.pins[0] = button.pins[1] = button.pins[2] = button.pins[3] = button.data[0];
        },
        .render = [](Component& component, ComponentRenderContext const& rctx) {
            rctx.add_to_scene(component.data[0] ? CSprite::ButtonOff : CSprite::ButtonOn, 0, 0);
        },
    };
}

ComponentDefinition led()
{
    return {
        .name = "led",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = false,
        .data_size = 1,
        .simulate = [](Component& led) {
            led.data[0] = led.pins[0] | led.pins[1] | led.pins[2] | led.pins[3];
        },
        .render = [](Component& component, ComponentRenderContext const& rctx) {
            rctx.add_to_scene(component.data[0] ? CSprite::LedOff : CSprite::LedOn, 0, 0);
        },
    };
}

}
