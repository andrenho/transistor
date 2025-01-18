#include "infobox.hh"

#include "imgui.h"
#include "ui/ui.hh"
#include "util/visitor.hh"

void Infobox::init()
{

}

void Infobox::render() const
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
        render_contents();
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}

void Infobox::render_contents() const
{
    for (auto const& item: ui().state().infobox_contents.contents) {
        std::visit(overloaded {
            [](std::string const& str) { ImGui::Text("%s", str.c_str()); },
        }, item);
        ImGui::Spacing();
    }

}