#include "mainmenu.hh"

#include <imgui.h>

extern "C" {
#include <pastel2d.h>
}

extern bool show_demo_window_;

void main_menu_render(ts_Transistor* T)
{
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {

#ifndef NDEBUG
            ImGui::Separator();

            if (ImGui::MenuItem("Serialize sandbox")) {
                ts_serialize_to_file(T, stdout);
                printf("-----------------------\n");
            }

            ImGui::MenuItem("Demo Window", "", &show_demo_window_);
#endif

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
                ps_graphics_quit();

            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();
}
