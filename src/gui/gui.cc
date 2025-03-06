
extern "C" {
#include "gui.hh"
#include <pastel2d.h>
}

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

ImGuiIO* io = nullptr;
bool show_demo_window_ = true;

void gui_init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(ps_graphics_window(), ps_graphics_renderer());
    ImGui_ImplSDLRenderer3_Init(ps_graphics_renderer());
}

void gui_events(SDL_Event* e)
{
    ImGui_ImplSDL3_ProcessEvent(e);
}

void gui_render()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window_)
        ImGui::ShowDemoWindow(&show_demo_window_);

    ImGui::Render();
}

void gui_finalize()
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}