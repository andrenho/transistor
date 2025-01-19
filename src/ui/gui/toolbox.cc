#include "toolbox.hh"

#include <functional>

#include "SDL2/SDL.h"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "ui/tools.hh"
#include "ui/ui.hh"

void Toolbox::init()
{
    SDL_Texture* icons = res().texture("icons");
    SDL_QueryTexture(icons, nullptr, nullptr, &icons_tx_w, &icons_tx_h);
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
        for (auto const& tool: tools()) {
            if (ui().state().selected_tool == tool.tool) {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor(6, 50, 150, 102));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor(6, 50, 150, 255));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor(0, 105, 10, 255));
            }
            if (i % 2 == 1)
                ImGui::SameLine(0, 5);
            if (image_button(tool.image, i)) {
                ui() << U::SelectTool { tool.tool };
            }
            ImGui::SetItemTooltip("%s", tool.tooltip.c_str());
            if (ui().state().selected_tool == tool.tool)
                ImGui::PopStyleColor(3);
            ++i;
        }
        ImGui::PopStyleVar();
    }
    ImGui::End();
}
