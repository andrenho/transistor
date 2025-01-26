#include "toolbox.hh"

#include "SDL2/SDL.h"

#include "backends/imgui_impl_sdlrenderer2.h"
#include "ui/ui.hh"

void Toolbox::init()
{
    SDL_Texture* icons = res().texture("__icons");
    SDL_QueryTexture(icons, nullptr, nullptr, &icons_tx_w, &icons_tx_h);

    res().add_tiles("__icons", {
        { &tb_arrow,       0, 8 },
        { &tb_vcc,         0, 9 },
        { &tb_button,      0, 10 },
        { &tb_led,         0, 11 },
        { &tb_npn,         0, 12 },
        { &tb_pnp,         0, 13 },
        { &tb_logic_gates, 1, 8 },
    }, 16);

    buttons_ = {
        { &tb_arrow,       "",           "Unselectd (ESC)" },
        { &tb_vcc,         "vcc",        "VCC [always `1`] (v)" },
        { &tb_button,      "button",     "Input button (b)" },
        { &tb_led,         "led",        "LED (l)" },
        { &tb_npn,         "npn",        "NPN transistor [activate to open] (n)" },
        { &tb_pnp,         "pnp",        "PNP transistor [activate to close] (p)" },
        { nullptr,         "",           "" },
        { &tb_logic_gates, "menu_logic", "Logic gates" },
    };
}

bool Toolbox::image_button(ResourceId const& resource, size_t i) const
{
    Tile tile = res().tile(resource);

    ImVec2 uv0 = ImVec2(
        (float) tile.x / icons_tx_w,
        (float) tile.y / icons_tx_h
    );
    ImVec2 uv1 = ImVec2(
        (float)(tile.x + tile.w) / icons_tx_w,
        (float)(tile.y + tile.h) / icons_tx_h
    );

    char label[30];
    snprintf(label, sizeof(label) - 6, "##btn%zu", i);

    return ImGui::ImageButton(label, (ImTextureID) tile.texture, { 32, 32 }, uv0, uv1);
}

void Toolbox::render() const
{
    render_popup_menus();
    render_toolbox();
}

void Toolbox::render_popup_menus() const
{
    // create menu structure
    std::map<ComponentDefinition::Category,
        std::map<std::string,
            std::map<std::string, ComponentDefinition const*>>> menus;
    for (auto const& [name, def]: game().sandbox().component_db().component_defs()) {
        if (def->category != ComponentDefinition::Category::Basic) {
            auto pos = def->tool_path.find('/');
            auto [menu, submenu] = std::pair(def->tool_path.substr(0, pos), pos == std::string::npos ? "" : def->tool_path.substr(pos + 1));
            menus[def->category][menu][submenu] = def.get();
        }
    }

    for (auto const& [category, menu]: menus) {
        for (auto const& [menu_name, submenu]: menu) {
            for (auto const& [submenu_name, def]: submenu) {
            }
        }
    }

}

void Toolbox::render_toolbox() const
{
    size_t i = 0;
    if (ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.f, 2.f));
        for (auto const& button: buttons_) {
            if (button.image == nullptr) {  // separator
                ImGui::Spacing();
                ImGui::Spacing();
                i = 0;
            } else {
                bool selected = ui().state().selected_component == button.component_name;
                if (selected) {
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(6, 50, 150, 102));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(6, 50, 150, 255));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(0, 105, 10, 255));
                }
                if (i % 2 == 1)
                    ImGui::SameLine(0, 5);
                if (image_button(*button.image, i)) {
                    ui() << U::SelectComponent { button.component_name };
                }
                if (!button.tooltip.empty())
                    ImGui::SetItemTooltip("%s", button.tooltip.c_str());
                if (selected)
                    ImGui::PopStyleColor(3);
                ++i;
            }
        }
        ImGui::PopStyleVar();
    }
    ImGui::End();
}

