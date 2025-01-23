#include "tools.hh"

void Tools::init()
{
    res().add_tiles("__icons", {
        { &tb_arrow,       0, 8 },
        { &tb_vcc,         0, 9 },
        { &tb_button,      0, 10 },
        { &tb_led,         0, 11 },
        { &tb_npn,         0, 12 },
        { &tb_pnp,         0, 13 },
        { &tb_logic_gates, 1, 8 },
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
        { "__infobox_diode_0", 6, 0, 3, 3 },
        { "__infobox_diode_1", 6, 3, 3, 3 },
    }, 16);

    tools_ = {
        Tool {
            .tool    = Type::Nothing,
            .image   = tb_arrow,
            .tooltip = "Unselect (ESC)",
        },
        Tool {
            .tool    = Type::VCC,
            .image   = tb_vcc,
            .tooltip = "VCC [always `1`] (v)",
            .infobox = R"(
                `VCC` is a component that generate an output of `1` in all pins.
                ${image: __infobox_vcc})",
        },
        Tool {
            .tool    = Type::Button,
            .image   = tb_button,
            .tooltip = "Input button (b)",
            .infobox = R"(
                `Buttons` accepts input from the user, and outputs `1` or `0` depending if the button is pressed or not.
                ${image: __infobox_button_0}${image_sl: __infobox_button_1})",
        },
        Tool {
            .tool    = Type::LED,
            .image   = tb_led,
            .tooltip = "LED (l)",
            .infobox = R"(
                `LEDs` are turned on or off, depending if the input is `0` or `1`.
                If there are multiple inputs, the LED is turned on if one of the inputs is `1`.
                ${image: __infobox_led_0}${image_sl: __infobox_led_1})",
        },
        Tool {
            .tool    = Type::NPN,
            .image   = tb_npn,
            .tooltip = "NPN transistor [activate to open] (n)",
            .infobox = R"(
                `NPN transistors` allow the passage of a signal if their input is `1`, and block it otherwise.
                There are two inputs, the input is considered `1` if one of the inputs is `1`.
                ${image: __infobox_npn_0}${image: __infobox_npn_1}
                Press `R` to rotate the component.)",
        },
        Tool {
            .tool    = Type::PNP,
            .image   = tb_pnp,
            .tooltip = "PNP transistor [activate to close] (p)",
            .infobox = R"(
                `PNP transistors` allow the passage of a signal if their input is `0`, and block it otherwise.
                There are two inputs, the input is considered `1` if one of the inputs is `1`.
                ${image: __infobox_pnp_0}${image: __infobox_pnp_1}
                The PNP transistor also works as a diode, blocking the signal in one direction but not the other.
                ${image: __infobox_diode_0}${image_sl: __infobox_diode_1}
                Press `R` to rotate the component.)",
        },
        /*
        { .tool = SelectedTool::Separator },
        {
            .tool = SelectedTool::Series,
            .image = tb_logic_gates,
            .tooltip = "Logic gates",
        }
        */
    };

    std::function<void(std::vector<ToolboxItem> const&)> add_tools = [&](std::vector<ToolboxItem> const& items) {
        for (auto const& item: items) {
            if (auto category = std::get_if<Category>(&item); category)
                add_tools(category->children);
            else if (auto tool = std::get_if<Tool>(&item); tool)
                tools_toplevel_.push_back(tool);
        }
    };
    add_tools(tools_);
}

Tools const& tools()
{
    static const Tools tools;
    return tools;
}

void tools_init()
{
    const_cast<Tools*>(&tools())->init();
}
