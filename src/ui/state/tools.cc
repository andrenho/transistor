#include "tools.hh"

static std::vector<Tool> tools_;
static resource_idx_t
    tb_arrow = -1, tb_vcc = -1, tb_button = -1, tb_led = -1, tb_npn = -1, tb_pnp = -1;

void tools_init()
{
    res().add_tiles("icons", {
        { &tb_arrow,  0,  8 },
        { &tb_vcc,    0,  9 },
        { &tb_button, 0, 10 },
        { &tb_led,    0, 11 },
        { &tb_npn,    0, 12 },
        { &tb_pnp,    0, 13 },
    }, 16);

    tools_ = {
        { SelectedTool::Nothing, tb_arrow, "Unselect (ESC)" },
        { SelectedTool::VCC,     tb_vcc, "VCC [always `1`] (v)" },
        { SelectedTool::Button,  tb_button, "Input button (b)" },
        { SelectedTool::LED,     tb_led, "LED (l)" },
        { SelectedTool::NPN,     tb_npn, "NPN transistor [activate to open] (n)" },
        { SelectedTool::PNP,     tb_pnp, "PNP transistor [activate to close] (p)" },
    };
}

std::vector<Tool> tools()
{
    return tools_;
}