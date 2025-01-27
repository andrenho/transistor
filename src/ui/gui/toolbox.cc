#include "toolbox.hh"

#include <iostream>
#include <string>
using namespace std::string_literals;

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
        { .image = &tb_arrow,  .component_name = " ",       .tooltip = "Unselectd (ESC)" },
        { .image = &tb_vcc,    .component_name = "vcc",    .tooltip = "VCC [always `1`] (v)" },
        { .image = &tb_button, .component_name = "button", .tooltip = "Input button (b)" },
        { .image = &tb_led,    .component_name = "led",    .tooltip = "LED (l)" },
        { .image = &tb_npn,    .component_name = "npn",    .tooltip = "NPN transistor [activate to open] (n)" },
        { .image = &tb_pnp,    .component_name = "pnp",    .tooltip = "PNP transistor [activate to close] (p)" },
        { .image = nullptr },
        { .image = &tb_logic_gates, .category = ComponentDefinition::Category::LogicGates, .tooltip = "Logic gates" },
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
    size_t i = 0;
    if (ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.f, 2.f));
        for (auto const& button: buttons_) {
            if (button.image == nullptr) {  // separator
                ImGui::Spacing();
                ImGui::Spacing();
                i = (i / 2) * 2;
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
                    if (button.category)
                        ImGui::OpenPopup(popup_name(*button.category).c_str());
                    else
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

        render_popup_menus();
    }
    ImGui::End();
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
        if (ImGui::BeginPopup(popup_name(category).c_str())) {
            for (auto const& [menu_name, submenu]: menu) {
                if (ImGui::BeginMenu(menu_name.c_str())) {
                    for (auto const& [submenu_name, def]: submenu) {
                        ImGui::MenuItem(submenu_name.c_str());
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::EndPopup();
        }
    }

}

std::string Toolbox::popup_name(ComponentDefinition::Category category) const
{
    return "category" + std::to_string((int) category);
}
