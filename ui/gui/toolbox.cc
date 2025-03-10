#include "toolbox.hh"

#include <optional>
#include <string>
#include <vector>

#include <imgui.h>
#include <SDL3/SDL.h>

extern "C" {
#include "pastel2d.h"
}

static ps_res_idx_t tb_arrow, tb_vcc, tb_button, tb_led, tb_npn, tb_pnp,
                    tb_logic_gates, tb_digital, tb_memory, tb_cpu;

static int icons_tx_w, icons_tx_h;

static std::optional<std::string> selected_component;

struct Button {
    ps_res_idx_t                        image;
    std::string                         component_name;
    std::optional<ts_ComponentCategory> category;
    std::string                         tooltip;
    bool                                separator;
};
static std::vector<Button> buttons;

void toolbox_init()
{
    tb_arrow       = PS_IDX("tb_arrow");
    tb_vcc         = PS_IDX("tb_vcc");
    tb_button      = PS_IDX("tb_button");
    tb_led         = PS_IDX("tb_led");
    tb_npn         = PS_IDX("tb_npn");
    tb_pnp         = PS_IDX("tb_pnp");
    tb_logic_gates = PS_IDX("tb_logic_gates");
    tb_digital     = PS_IDX("tb_digital");
    tb_memory      = PS_IDX("tb_memory");
    tb_cpu         = PS_IDX("tb_cpu");

    buttons = {
        { .image = tb_arrow,  .component_name = " ",        .tooltip = "Unselectd (ESC)" },
        { .image = tb_vcc,    .component_name = "__vcc",    .tooltip = "VCC [always `1`] (v)" },
        { .image = tb_button, .component_name = "__button", .tooltip = "Input button (b)" },
        { .image = tb_led,    .component_name = "__led",    .tooltip = "LED (l)" },
        { .image = tb_npn,    .component_name = "__npn",    .tooltip = "NPN transistor [activate to open] (n)" },
        { .image = tb_pnp,    .component_name = "__pnp",    .tooltip = "PNP transistor [activate to close] (p)" },
        { .separator = true },
        { .image = tb_logic_gates, .category = TS_CAT_LOGIC_GATES, .tooltip = "Logic gates" },
        { .image = tb_digital,     .category = TS_CAT_DIGITAL,     .tooltip = "Digital components" },
        { .image = tb_memory,      .category = TS_CAT_MEMORY, .     tooltip = "Memories" },
        { .image = tb_cpu,         .category = TS_CAT_CPU,         .tooltip = "CPUs and microcontrollers" },
    };

    SDL_Texture* circuit = ps_res_get_texture(PS_IDX("circuit"));
    icons_tx_w = circuit->w;
    icons_tx_h = circuit->h;
}

static bool image_button(ps_res_idx_t resource, size_t i)
{
    ps_Tile tile = ps_res_get_tile(resource);

    ImVec2 uv0 = ImVec2(
        (float) tile.rect.x / icons_tx_w,
        (float) tile.rect.y / icons_tx_h
    );
    ImVec2 uv1 = ImVec2(
        (float)(tile.rect.x + tile.rect.w) / icons_tx_w,
        (float)(tile.rect.y + tile.rect.h) / icons_tx_h
    );

    char label[30];
    snprintf(label, sizeof(label) - 6, "##btn%zu", i);

    return ImGui::ImageButton(label, (ImTextureID) tile.texture, { 32, 32 }, uv0, uv1);
}

void toolbox_render(ts_Transistor* T)
{
    size_t i = 0;
    if (ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.f, 2.f));
        for (auto const& button: buttons) {
            if (button.separator) {  // separator
                ImGui::Spacing();
                ImGui::Spacing();
                i = (i / 2) * 2;
            } else {
                bool selected = selected_component == button.component_name;
                if (selected) {
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(6, 50, 150, 102));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(6, 50, 150, 255));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(0, 105, 10, 255));
                }
                if (i % 2 == 1)
                    ImGui::SameLine(0, 5);
                if (button.image != 0 && image_button(button.image, i)) {
                    selected_component = button.component_name;
                    /*
                    if (button.category)
                        ImGui::OpenPopup(popup_name(*button.category).c_str());
                    else
                        ui() << U::SelectComponent { button.component_name };
                    */
                }
                if (!button.tooltip.empty())
                    ImGui::SetItemTooltip("%s", button.tooltip.c_str());
                if (selected)
                    ImGui::PopStyleColor(3);
                ++i;
            }
        }
        ImGui::PopStyleVar();

        // render_popup_menus();
    }
    ImGui::End();

}
