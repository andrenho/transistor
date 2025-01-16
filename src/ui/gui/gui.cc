#include "gui.hh"

#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "battery/embed.hpp"
#include "engine/sandbox/sandbox.hh"
#include "ui/ui.hh"

void GUI::init(SDL_Window* window, SDL_Renderer* ren, std::vector<Resource> const& icons)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    setup_theme();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, ren);
    ImGui_ImplSDLRenderer2_Init(ren);

    // Load custom font
    auto ttf = b::embed<"resources/fonts/Pixellari.ttf">();
    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;
    io->Fonts->AddFontFromMemoryTTF((void *) ttf.data(), (int) ttf.size(), 16, &cfg);

    init_toolbox(icons);
}

void GUI::init_toolbox(std::vector<Resource> const& icons)
{
    ui() << U::UpdateToolbox {.toolbox = {
        { .image = icons.at((size_t) CSprite::Arrow) }
    }};
}

void GUI::setup_theme()
{
    // ImGui::StyleColorsDark();
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

void GUI::shutdown()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUI::process_events(SDL_Event* e)
{
    ImGui_ImplSDL2_ProcessEvent(e);
}

bool GUI::render(SDL_Renderer* ren, UIState const& state) const
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window_)
        ImGui::ShowDemoWindow(&show_demo_window_);

    if (!render_main_menu())
        return false;
    render_toolbox(state);
    render_infobox(state);

    if (!render_modal_exception(state))
        return false;

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), ren);

    return true;
}

bool GUI::render_main_menu() const
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
#ifndef NDEBUG
            ImGui::Separator();
            if (ImGui::MenuItem("Serialize sandbox"))
                std::cout << game().serialize().dump(2) << "\n";
            ImGui::MenuItem("Demo Window", "", &show_demo_window_);
#endif
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
                return false;
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    return true;
}

void GUI::render_infobox(UIState const& state) const
{
    constexpr float INFOBOX_WIDTH = 350.f;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 window_pos = { viewport->WorkSize.x - INFOBOX_WIDTH, viewport->WorkPos.y };
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize({ INFOBOX_WIDTH, viewport->WorkSize.y });

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 16, 16 });

    ImGui::SetNextWindowBgAlpha(0.35f);

    if (ImGui::Begin("InfoBox", nullptr, window_flags)) {
        ::render_infobox(state.infobox_contents);
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}

void GUI::render_toolbox(UIState const& state) const
{
    if (ImGui::Begin("Toolbox", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
        for (auto const& button: state.toolbox) {
            if (ImGui::Button("X", { 38, 38 }))
                ;
        }
    }
    ImGui::End();
}

bool GUI::render_modal_exception(UIState const& state) const
{
    ImGui::SetNextWindowSizeConstraints({ 400.f, 0.f }, { 400.f, FLT_MAX });
    if (ImGui::BeginPopupModal("Error!", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (state.exception) {
            ImGui::PushTextWrapPos();
            ImGui::Text("%s", state.exception->text.c_str());
            ImGui::PopTextWrapPos();
        }
        ImGui::Separator();
        ImGui::SetItemDefaultFocus();
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            ui() << U::ClearException {};
            ImGui::EndPopup();
            return state.exception->recoverable;
        }
        ImGui::EndPopup();
    }

    if (state.exception)
        ImGui::OpenPopup("Error!");

    return true;
}
