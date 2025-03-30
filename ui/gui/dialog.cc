#include "dialog.hh"

#include <vector>

#include <imgui.h>

void render_dialogs(std::vector<luaobj::Dialog const*> const& dialogs, Engine& engine)
{
    for (auto const& dialog: dialogs) {
        if (dialog->show)
            ImGui::OpenPopup(dialog->title.c_str());

        if (ImGui::BeginPopupModal(dialog->title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            for (auto const& text: dialog->text)
                ImGui::Text("%s", text.c_str());
            ImGui::Separator();
            for (auto const& button: dialog->buttons) {
                if (ImGui::Button(button.text.c_str())) {
                    if (button.callback)
                        engine.call_api_function(*button.callback);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
            }
            ImGui::EndPopup();
        }
    }
}
