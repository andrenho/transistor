#include "native.hh"

#include "engine/board/component.hh"

static constexpr InputPinType Input = InputPinType::Input;
static constexpr InputPinType Output = InputPinType::Output;
static constexpr int          TILE_SIZE = 16;

std::vector<ComponentDefinition> native_components()
{
    return {

        //
        // button
        //

        {
            .name = "button",
            .category = ComponentDefinition::Category::Basic,
            .infobox = R"(
                `Buttons` accepts input from the user, and outputs `1` or `0` depending if the button is pressed or not.
                ${image: __infobox_button_0}${image_sl: __infobox_button_1})",

            .type = ComponentDefinition::Type::SingleTile,
            .can_rotate = false,
            .pins = { { "O0", Output }, { "O1", Output }, { "O2", Output }, { "O3", Output } },
            .data_size = 1,

            .init = []() {
                res().add_tiles("__infobox", {
                                    { "__infobox_button_0", 0, 3, 3, 3 },
                                    { "__infobox_button_1", 0, 6, 3, 3 },
                                }, TILE_SIZE);
            },

            .on_click = [](Component& button) {
                button.data[0] = (!button.data[0]) & 1;
            },
            .simulate = [](Component& button) {
                button.pins[0] = button.pins[1] = button.pins[2] = button.pins[3] = button.data[0];
            },
            .render = [](std::optional<Component const *> component, Scene& scene, int x, int y, Pen pen) {
                scene.add("shadow_square", x + 1, y + 1, pen);
                if (component)
                    scene.add((*component)->data[0] ? "button_on" : "button_off", x, y, pen);
                else
                    scene.add("button_off", x, y, pen);
            },
        },

        //
        // led
        //

        {
            .name = "led",
            .category = ComponentDefinition::Category::Basic,
            .infobox = R"(
                `LEDs` are turned on or off, depending if the input is `0` or `1`.
                If there are multiple inputs, the LED is turned on if one of the inputs is `1`.
                ${image: __infobox_led_0}${image_sl: __infobox_led_1})",

            .type = ComponentDefinition::Type::SingleTile,
            .can_rotate = false,
            .pins = { { "I0", Input }, { "I1", Input }, { "I2", Input }, { "I3", Input } },
            .data_size = 1,

            .init = []() {
                res().add_tiles("__infobox", {
                                    { "__infobox_led_0", 3, 0, 3, 3 },
                                    { "__infobox_led_1", 3, 3, 3, 3 },
                                }, TILE_SIZE);
            },

            .simulate = [](Component& led) {
                led.data[0] = led.pins[0] | led.pins[1] | led.pins[2] | led.pins[3];
            },
            .render = [](std::optional<Component const *> component, Scene& scene, int x, int y, Pen pen) {
                scene.add("shadow_circle", x + 1, y + 1, pen);
                if (component)
                    scene.add((*component)->data[0] ? "led_on" : "led_off", x, y, pen);
                else
                    scene.add("led_off", x, y, pen);
            },
        },

        //
        // vcc
        //

        {
            .name = "vcc",
            .category = ComponentDefinition::Category::Basic,
            .infobox = R"(
                `VCC` is a component that generate an output of `1` in all pins.
                ${image: __infobox_vcc})",

            .type = ComponentDefinition::Type::SingleTile,
            .can_rotate = false,
            .pins = { { "O0", Output }, { "O1", Output }, { "O2", Output }, { "O3", Output } },

            .init = []() {
                res().add_tile("__infobox", "__infobox_vcc", 0, 0, 3, 3, TILE_SIZE);
            },

            .simulate = [](Component& vcc) {
                vcc.pins[0] = vcc.pins[1] = vcc.pins[2] = vcc.pins[3] = 1;
            },
            .render = [](std::optional<Component const *>, Scene& scene, int x, int y, Pen pen) {
                scene.add("shadow_square", x + 1, y + 1, pen);
                scene.add("vcc", x, y, pen);
            }
        },

        //
        // npn
        //
        {
            .name = "npn",
            .category = ComponentDefinition::Category::Basic,
            .infobox = R"(
                `NPN transistors` allow the passage of a signal if their input is `1`, and block it otherwise.
                There are two inputs, the input is considered `1` if one of the inputs is `1`.
                ${image: __infobox_npn_0}${image: __infobox_npn_1}
                Press `R` to rotate the component.)",

            .type = ComponentDefinition::Type::SingleTile,
            .pins = {
                { "SW0", Input }, { "IN", Input }, { "SW2", Input }, { "OUT", Output }
            },

            .init = []() {
                res().add_tiles("__infobox", {
                                    { "__infobox_npn_0", 0, 9, 5, 4 },
                                    { "__infobox_npn_1", 5, 9, 5, 4 },
                                }, TILE_SIZE);
            },

            .simulate = [](Component& npn) {
                constexpr size_t IN = 1, SWITCH_1 = 0, SWITCH_2 = 2, OUT = 3;
                npn.pins[OUT] = npn.pins[IN] & (npn.pins[SWITCH_1] | npn.pins[SWITCH_2]);
            },
            .render = [](std::optional<Component const *>, Scene& scene, int x, int y, Pen pen) {
                scene.add("shadow_rect", x + 1, y + 1, pen);
                scene.add("npn", x, y, pen);
            },
        },

        //
        // pnp
        //

        {
            .name = "pnp",
            .category = ComponentDefinition::Category::Basic,
            .infobox = R"(
                `PNP transistors` allow the passage of a signal if their input is `0`, and block it otherwise.
                There are two inputs, the input is considered `1` if one of the inputs is `1`.
                ${image: __infobox_pnp_0}${image: __infobox_pnp_1}
                The PNP transistor also works as a diode, blocking the signal in one direction but not the other.
                ${image: __infobox_diode_0}${image_sl: __infobox_diode_1}
                Press `R` to rotate the component.)",

            .type = ComponentDefinition::Type::SingleTile,
            .pins = { { "SW0", Input }, { "IN", Input }, { "SW1", Input }, { "OUT", Output } },

            .init = []() {
                res().add_tiles("__infobox", {
                                    { "__infobox_pnp_0", 5, 13, 5, 4 },
                                    { "__infobox_pnp_1", 0, 13, 5, 4 },
                                    { "__infobox_diode_0", 6, 0, 3, 3 },
                                    { "__infobox_diode_1", 6, 3, 3, 3 },
                                }, TILE_SIZE);
            },

            .simulate = [](Component& pnp) {
                constexpr size_t IN = 1, SWITCH_1 = 0, SWITCH_2 = 2, OUT = 3;
                pnp.pins[OUT] = pnp.pins[IN] & !(pnp.pins[SWITCH_1] | pnp.pins[SWITCH_2]);
            },
            .render = [](std::optional<Component const *>, Scene& scene, int x, int y, Pen pen) {
                scene.add("shadow_rect", x + 1, y + 1, pen);
                scene.add("pnp", x, y, pen);
            },
        },

        //
        // or_i2
        //
        {
            .name = "or_2i",
            .category = ComponentDefinition::Category::LogicGates,
            .tool_path = "OR gate/2-input",

            .type = ComponentDefinition::Type::IC_DIP,
            .pins = { { "IN0", Input }, { "IN1", Input }, { "~Q", Output }, { "Q", Output } },

            .init = []() {
                res().add_tile("__icons", "__or_2i", 1, 12, 1, 2, TILE_SIZE);
            },

            .simulate = [](Component& c) {
                constexpr size_t IN0 = 0, IN1 = 0, Q_ = 2, Q = 3;
                c.pins[Q] = c.pins[IN0] || c.pins[IN1];
                c.pins[Q_] = !c.pins[Q];
            },
            .render = [](std::optional<Component const *>, Scene& scene, int x, int y, Pen pen) {
                scene.add("__or_2i", x, y, pen);
            },
        },

    };
}
