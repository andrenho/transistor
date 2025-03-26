#include "dialog.hh"

#include "render.hh"
#include "transistor.hh"
#include <vector>

#include <imgui.h>

void render_dialogs(std::vector<Dialog> const& dialogs, Transistor& T)
{
    for (Dialog const& dialog: dialogs) {
        if (dialog.show)
            ImGui::OpenPopup(dialog.title.c_str());

        if (ImGui::BeginPopupModal(dialog.title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            for (auto const& text: dialog.text)
                ImGui::Text("%s", text.c_str());
            ImGui::Separator();
            for (auto const& button: dialog.buttons) {
                if (ImGui::Button(button.c_str())) {
                    if (*dialog.callback)
                        T.execure_ref_with_sandbox(**dialog.callback);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
            }
            ImGui::EndPopup();
        }
    }
}
