#include "dialog.hh"

#include <imgui.h>

void Dialog::render(ts_Transistor* T) const
{
    if (show_)
        ImGui::OpenPopup(title_.c_str());

    if (ImGui::BeginPopupModal(title_.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        for (auto const& text: text_)
            ImGui::Text("%s", text.c_str());
        ImGui::Separator();
        for (auto const& button: buttons_) {
            if (ImGui::Button(button.text.c_str())) {
                button.function(T);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
        }
        ImGui::EndPopup();
    }
}