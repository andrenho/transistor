#include "toolbox.hh"

#include <string>
#include <unordered_set>
using namespace std;

#include <imgui.h>

static bool image_button(ps_res_idx_t resource, size_t i, int circuit_tx_w, int circuit_tx_h)
{
    ps::res::Tile tile = ps::res::tile(resource);

    ImVec2 uv0 = ImVec2(
        tile.rect.x / (float) circuit_tx_w,
        tile.rect.y / (float) circuit_tx_h
    );
    ImVec2 uv1 = ImVec2(
        (tile.rect.x + tile.rect.w) / (float) circuit_tx_w,
        (tile.rect.y + tile.rect.h) / (float) circuit_tx_h
    );

    return ImGui::ImageButton(("##btn" + std::to_string(i)).c_str(), (ImTextureID) tile.texture, { 32, 32 }, uv0, uv1);
}

static std::string popup_name(std::string const& category)
{
    return "category_" + category;
}

static void render_popup_menu(luaobj::ToolItem const& item, std::vector<luaobj::Event>& events)
{
    if (ImGui::BeginPopup(popup_name(*item.category).c_str())) {
        for (auto const& menu: *item.menus) {
            if (ImGui::BeginMenu(menu.c_str())) {
                for (auto const& submenu: item.submenus->at(menu))
                    if (ImGui::MenuItem(submenu.name.c_str()))
                        events.push_back({ .type = "select_key", .key = submenu.key });
                ImGui::EndMenu();
            }
        }
        ImGui::EndPopup();
    }
}

void render_toolbox(luaobj::Render const& render, std::vector<luaobj::Event>& events, int circuit_tx_w, int circuit_tx_h)
{
    size_t i = 0;
    if (ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.f, 2.f));
        for (auto const& item: render.toolbox) {
            if (item.separator) {  // separator
                ImGui::Spacing();
                ImGui::Spacing();
                i = (i / 2) * 2;
            } else {
                if (item.selected) {
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(6, 50, 150, 102));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(6, 50, 150, 255));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(0, 105, 10, 255));
                }
                if (i % 2 == 1)
                    ImGui::SameLine(0, 5);
                if (item.image != 0 && image_button(item.image, i, circuit_tx_w, circuit_tx_h)) {
                    if (item.category)
                        ImGui::OpenPopup(popup_name(*item.category).c_str());
                    else if (item.key)
                        events.push_back({ .type = "select_key", .key = *item.key });
                }
                if (!item.tooltip.empty())
                    ImGui::SetItemTooltip("%s", item.tooltip.c_str());
                if (item.selected)
                    ImGui::PopStyleColor(3);
                ++i;
            }
        }
        ImGui::PopStyleVar();

        // draw popup menus
        for (auto const& item: render.toolbox)
            if (item.category)
                render_popup_menu(item, events);
    }
    ImGui::End();
}
