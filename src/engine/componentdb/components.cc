#include "components.hh"

#include "engine/editor/component.hh"

#include "ui/scene.hh"
#include "ui/layers/board/circuit_atlas.hh"


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
        .render = [](Component& component, ComponentDefinition::RenderContext const& rctx) {
            if (component.data[0] == 0)
                rctx.scene.add(rctx.context, rctx.icons.at((size_t) CSprite::ButtonOff), 0, 0);
            else
                rctx.scene.add(rctx.context, rctx.icons.at((size_t) CSprite::ButtonOn), 0, 0);
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
    };
}

}
