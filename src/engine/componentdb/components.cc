#include "components.hh"

#include "engine/board/component.hh"

namespace component {

ComponentDefinition button()
{
    return {
        .name = "button",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = false,
        .pins = {
            { InputPinType::Output }, { InputPinType::Output },
            { InputPinType::Output }, { InputPinType::Output },
        },
        .data_size = 1,
        .on_click = [](Component& button) {
            button.data[0] = (!button.data[0]) & 1;
        },
        .simulate = [](Component& button) {
            button.pins[0] = button.pins[1] = button.pins[2] = button.pins[3] = button.data[0];
        },
        .render = [](std::optional<Component const*> component, Scene& scene, int x, int y, Pen pen) {
            scene.add("shadow_square", x + 1, y + 1, pen);
            if (component)
                scene.add((*component)->data[0] ? "button_on" : "button_off", x, y, pen);
            else
                scene.add("button_off", x, y, pen);
        },
    };
}

ComponentDefinition led()
{
    return {
        .name = "led",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = false,
        .pins = {
            { InputPinType::Input }, { InputPinType::Input },
            { InputPinType::Input }, { InputPinType::Input },
        },
        .data_size = 1,
        .simulate = [](Component& led) {
            led.data[0] = led.pins[0] | led.pins[1] | led.pins[2] | led.pins[3];
        },
        .render = [](std::optional<Component const*> component, Scene& scene, int x, int y, Pen pen) {
            scene.add("shadow_circle", x + 1, y + 1, pen);
            if (component)
                scene.add((*component)->data[0] ? "led_on" : "led_off", x, y, pen);
            else
                scene.add("led_off", x, y, pen);
        },
    };
}

ComponentDefinition vcc()
{
    return {
        .name = "vcc",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = false,
        .pins = {
            { InputPinType::Output }, { InputPinType::Output },
            { InputPinType::Output }, { InputPinType::Output },
        },
        .simulate = [](Component& vcc) {
            vcc.pins[0] = vcc.pins[1] = vcc.pins[2] = vcc.pins[3] = 1;
        },
        .render = [](std::optional<Component const*>, Scene& scene, int x, int y, Pen pen) {
            scene.add("shadow_square", x + 1, y + 1, pen);
            scene.add("vcc", x, y, pen);
        },
    };
}

ComponentDefinition npn()
{
    return {
        .name = "npn",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = true,
        .pins = {
            { InputPinType::Input }, { InputPinType::Input },
            { InputPinType::Input }, { InputPinType::Output },
        },
        .simulate = [](Component& npn) {
            constexpr size_t IN = 1, SWITCH_1 = 0, SWITCH_2 = 2, OUT = 3;
            npn.pins[OUT] = npn.pins[IN] & (npn.pins[SWITCH_1] | npn.pins[SWITCH_2]);
        },
        .render = [](std::optional<Component const*>, Scene& scene, int x, int y, Pen pen) {
            scene.add("shadow_rect", x + 1, y + 1, pen);
            scene.add("npn", x, y, pen);
        },
    };
}

ComponentDefinition pnp()
{
    return {
        .name = "pnp",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = true,
        .pins = {
            { InputPinType::Input }, { InputPinType::Input },
            { InputPinType::Input }, { InputPinType::Output },
        },
        .simulate = [](Component& pnp) {
            constexpr size_t IN = 1, SWITCH_1 = 0, SWITCH_2 = 2, OUT = 3;
            pnp.pins[OUT] = pnp.pins[IN] & !(pnp.pins[SWITCH_1] | pnp.pins[SWITCH_2]);
        },
        .render = [](std::optional<Component const*>, Scene& scene, int x, int y, Pen pen) {
            scene.add("shadow_rect", x + 1, y + 1, pen);
            scene.add("pnp", x, y, pen);
        },
    };
}

}
