
#include "mainmenu.hh"


extern "C" {
#include "gui.hh"
#include <pastel2d.h>
}

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "resources/fonts/Pixellari.ttf.h"

ImGuiIO* io = nullptr;
bool show_demo_window_ = false;

static void setup_theme()
{
    ImGui::StyleColorsLight();

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 0.94f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.55f, 0.55f, 0.55f, 0.98f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.77f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.50f, 0.53f, 0.58f, 0.93f);
    colors[ImGuiCol_TabSelected]            = ImVec4(0.25f, 0.38f, 0.66f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 0.80f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = { 8, 8 };
    style.FramePadding = { 7, 5 };
    style.ItemSpacing = { 8, 4 };
    style.WindowRounding = 5;
    style.FrameRounding = 3;
    style.ScrollbarRounding = 0;
}

static void setup_font()
{
    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;
    io->Fonts->AddFontFromMemoryTTF((void *) resources_fonts_Pixellari_ttf, resources_fonts_Pixellari_ttf_sz, 16, &cfg);
}

void gui_init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    setup_theme();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(ps_graphics_window(), ps_graphics_renderer());
    ImGui_ImplSDLRenderer3_Init(ps_graphics_renderer());

    setup_font();
}

void gui_events(SDL_Event* e)
{
    ImGui_ImplSDL3_ProcessEvent(e);
}

void gui_render(ts_Transistor* T)
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window_)
        ImGui::ShowDemoWindow(&show_demo_window_);

    main_menu_render(T);

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), ps_graphics_renderer());
}

void gui_finalize()
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}