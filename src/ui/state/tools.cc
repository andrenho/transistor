#include "tools.hh"

static std::vector<Tool> tools_;
static resource_idx_t
        tb_arrow = -1, tb_vcc = -1, tb_button = -1, tb_led = -1, tb_npn = -1, tb_pnp = -1;

void tools_init()
{
    res().add_tiles("__icons", {
        { &tb_arrow,  0, 8 },
        { &tb_vcc,    0, 9 },
        { &tb_button, 0, 10 },
        { &tb_led,    0, 11 },
        { &tb_npn,    0, 12 },
        { &tb_pnp,    0, 13 },
    }, 16);

    resource_idx_t infobox = res().add_texture<"resources/images/infobox.png">();
    res().add_tiles(infobox, {
        { "__infobox_vcc", 0, 0, 3, 3 },
        { "__infobox_button_0", 0, 3, 3, 3 },
        { "__infobox_button_1", 0, 6, 3, 3 },
        { "__infobox_led_0", 3, 0, 3, 3 },
        { "__infobox_led_1", 3, 3, 3, 3 },
        { "__infobox_npn_0", 0, 9, 5, 4 },
        { "__infobox_npn_1", 5, 9, 5, 4 },
        { "__infobox_pnp_0", 5, 13, 5, 4 },
        { "__infobox_pnp_1", 0, 13, 5, 4 },
        { "__infobox_diode_0", 9, 0, 4, 4 },
        { "__infobox_diode_1", 9, 4, 4, 4 },
    }, 16);

    tools_ = {
        {
            .tool    = SelectedTool::Nothing,
            .image   = tb_arrow,
            .tooltip = "Unselect (ESC)",
        },
        {
            .tool    = SelectedTool::VCC,
            .image   = tb_vcc,
            .tooltip = "VCC [always `1`] (v)",
            .infobox = R"(
                `VCC` is a component that generate an output of `1` in all pins.
                ${image: __infobox_vcc})",
        },
        {
            .tool    = SelectedTool::Button,
            .image   = tb_button,
            .tooltip = "Input button (b)",
            .infobox = R"(
                `Buttons` accepts input from the user, and outputs `1` or `0` depending if the button is pressed or not.
                ${image: __infobox_button_0}${image_sl: __infobox_button_1})",
        },
        {
            .tool    = SelectedTool::LED,
            .image   = tb_led,
            .tooltip = "LED (l)",
            .infobox = R"(
                `LEDs` are turned on or off, depending if the input is `0` or `1`.
                If there are multiple inputs, the LED is turned on if one of the inputs is `1`.
                ${image: __infobox_led_0}${image_sl: __infobox_led_1})",
        },
        {
            .tool    = SelectedTool::NPN,
            .image   = tb_npn,
            .tooltip = "NPN transistor [activate to open] (n)",
            .infobox = R"(
                `NPN transistors` allow the passage of a signal if their input is `1`, and block it otherwise.
                There are two inputs, the input is considered `1` if one of the inputs is `1`.
                ${image: __infobox_npn_0}${image: __infobox_npn_1}
                The NPN transistor can also serve as a diode, blocking the signal in one direction but not the other.
                ${image: __infobox_diode_0}${image_sl: __infobox_diode_1})",
        },
        {
            .tool    = SelectedTool::PNP,
            .image   = tb_pnp,
            .tooltip = "PNP transistor [activate to close] (p)",
            .infobox = R"(
                `PNP transistors` allow the passage of a signal if their input is `0`, and block it otherwise.
                There are two inputs, the input is considered `1` if one of the inputs is `1`.
                ${image: __infobox_pnp_0}${image: __infobox_pnp_1})",
        },
    };
}

std::vector<Tool> const& tools()
{
    return tools_;
}
