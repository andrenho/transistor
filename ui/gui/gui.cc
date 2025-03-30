#include "gui.hh"

#include <pastel2d.hh>

#include "dialog.hh"
#include "menu.hh"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "../resources/fonts/Pixellari.ttf.h"

GUI::GUI()
{
#ifdef DEV
    show_error_window_ = true;
#endif
}

GUI::~GUI()
{
    if (io) {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }
}

void GUI::init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    setup_theme();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(ps::graphics::window(), ps::graphics::renderer());
    ImGui_ImplSDLRenderer3_Init(ps::graphics::renderer());

    setup_font();
}

void GUI::do_event(SDL_Event* e) const
{
    ImGui_ImplSDL3_ProcessEvent(e);
}

void GUI::render(luaobj::Render const& render, Engine& engine) const
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window_)
        ImGui::ShowDemoWindow(&show_demo_window_);
    if (show_error_window_)
        error_window(render, engine);

    render_menu(render, engine);
    render_dialogs(dialog_list(render), engine);

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), ps::graphics::renderer());
}

void GUI::setup_theme()
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

void GUI::setup_font()
{
    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;
    io->Fonts->AddFontFromMemoryTTF((void *) ui_resources_fonts_Pixellari_ttf, ui_resources_fonts_Pixellari_ttf_sz, 16, &cfg);
}

void GUI::error_window(luaobj::Render const& render, Engine const& engine) const
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    const float PAD = 10.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = work_pos.x + PAD;
    window_pos.y = work_pos.y + work_size.y - PAD;
    window_pos_pivot.x = 0.0f;
    window_pos_pivot.y = 1.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    window_flags |= ImGuiWindowFlags_NoMove;
    ImGui::SetNextWindowBgAlpha(0.65f); // Transparent background
    if (ImGui::Begin("Error overlay", &show_error_window_, window_flags)) {
        if (engine.test_errors())
            ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.6f, 1.0f), "Tests failing: %s", engine.test_errors().value().c_str());
        else
            ImGui::Text("Tests ok.");
        ImGui::Separator();
        if (engine.compilation_messages().empty()) {
            ImGui::Text("No warnings.");
        } else {
            ImVec4 color = engine.compilation_successful() ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(1.0f, 0.6f, 0.6f, 1.0f);
            ImGui::TextColored(color, "%s", engine.compilation_messages().c_str());
        }
    }
    ImGui::End();
}

std::vector<luaobj::Dialog const*> GUI::dialog_list(luaobj::Render const& render)
{
    std::vector<luaobj::Dialog const*> dialogs;

    std::function<void(std::vector<luaobj::MenuItem> const&)> find_dialogs = [&](std::vector<luaobj::MenuItem> const& menus) {
        for (auto const& menu: menus) {
            if (menu.ask_confirmation)
                dialogs.push_back(&menu.ask_confirmation.value());
            find_dialogs(menu.items);
        }
    };
    find_dialogs(render.menu);

    return dialogs;
}
