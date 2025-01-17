#include "components.hh"

#include "engine/board/component.hh"

namespace component {

static constexpr int TILE_SIZE = 16;

ComponentDefinition button()
{
    res().add_tiles("icons", {
        { "button_off", 6, 2 },
        { "button_on", 6, 3 },
    }, TILE_SIZE);

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
        .render = [](Component const& component, Scene& scene, int x, int y) {
            scene.add("shadow_square", x + 1, y + 1);
            scene.add(component.data[0] ? "button_on" : "button_off", x, y);
        },
    };
}

ComponentDefinition led()
{
    res().add_tiles("icons", {
        { "led_off", 7, 2 },
        { "lef_on", 7, 3 },
    }, TILE_SIZE);

    return {
        .name = "led",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = false,
        .data_size = 1,
        .input_pins = [](Component const& component) {
            return std::vector<uintpin_t> { 0, 1, 2, 3 };
        },
        .simulate = [](Component& led) {
            led.data[0] = led.pins[0] | led.pins[1] | led.pins[2] | led.pins[3];
        },
        .render = [](Component const& component, Scene& scene, int x, int y) {
            scene.add("shadow_circle", x + 1, y + 1);
            scene.add(component.data[0] ? "led_on" : "led_off", x, y);
        },
    };
}

ComponentDefinition vcc()
{
    res().add_tile("vcc", "icons", 8, 2, TILE_SIZE);

    return {
        .name = "vcc",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = false,
        .data_size = 0,
        .simulate = [](Component& vcc) {
            vcc.pins[0] = vcc.pins[1] = vcc.pins[2] = vcc.pins[3] = 1;
        },
        .render = [](Component const& component, Scene& scene, int x, int y) {
            scene.add("shadow_square", x + 1, y + 1);
            scene.add("vcc", x, y);
        },
    };
}

ComponentDefinition npn()
{
    res().add_tile("npn", "icons", 5, 2, TILE_SIZE);

    return {
        .name = "npn",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = true,
        .data_size = 0,
        .input_pins = [](Component const& component) {
            return std::vector<uintpin_t> { 0, 1, 2 };
        },
        .simulate = [](Component& npn) {
            constexpr size_t IN = 1, SWITCH_1 = 0, SWITCH_2 = 2, OUT = 3;
            npn.pins[OUT] = npn.pins[IN] & !(npn.pins[SWITCH_1] | npn.pins[SWITCH_2]);
        },
        .render = [](Component const& component, Scene& scene, int x, int y) {
            Pen pen = { .rotation = component.rotation };
            scene.add("shadow_rect", x + 1, y + 1, pen);
            scene.add("npn", x, y, pen);
        },
    };
}

ComponentDefinition pnp()
{
    res().add_tile("pnp", "icons", 5, 3, TILE_SIZE);

    return {
        .name = "pnp",
        .type = ComponentDefinition::Type::SingleTile,
        .can_rotate = true,
        .data_size = 0,
        .input_pins = [](Component const& component) {
            return std::vector<uintpin_t> { 0, 1, 2 };
        },
        .simulate = [](Component& pnp) {
            constexpr size_t IN = 1, SWITCH_1 = 0, SWITCH_2 = 2, OUT = 3;
            pnp.pins[OUT] = pnp.pins[IN] & (pnp.pins[SWITCH_1] | pnp.pins[SWITCH_2]);
        },
        .render = [](Component const& component, Scene& scene, int x, int y) {
            Pen pen = { .rotation = component.rotation };
            scene.add("shadow_rect", x + 1, y + 1, pen);
            scene.add("pnp", x, y, pen);
        },
    };
}

}
