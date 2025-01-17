#include "toolbox.hh"

#include "SDL2/SDL.h"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

void Toolbox::init()
{
    SDL_Texture* icons = res().texture("icons");
    SDL_QueryTexture(icons, nullptr, nullptr, &icons_tx_w, &icons_tx_h);

    res().add_tiles("icons", {
        { &tb_arrow, 5, 1 }
    }, 16);
}

bool Toolbox::image_button(ResourceId const& resource) const
{
    Tile tile = res().tile(tb_arrow);

    ImVec2 uv0 = ImVec2(
        (float) tile.x / icons_tx_w,
        (float) tile.y / icons_tx_h
    );
    ImVec2 uv1 = ImVec2(
        (float)(tile.x + tile.w) / icons_tx_w,
        (float)(tile.y + tile.h) / icons_tx_h
    );

    return ImGui::ImageButton("X", (ImTextureID)(intptr_t) tile.texture, { 34, 34 }, uv0, uv1);
}

void Toolbox::render() const
{
    if (ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
        if (image_button(tb_arrow))
            ;
    }
    ImGui::End();
}
