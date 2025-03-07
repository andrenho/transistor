#include "mainmenu.hh"

#include <imgui.h>

extern "C" {
#include <pastel2d.h>
}

extern bool show_demo_window_;
bool about_box_ = false;

void main_menu_render(ts_Transistor* T)
{
    bool open_about_box = false;

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

        if (ImGui::BeginMenu("Simulation")) {

            if (ImGui::BeginMenu("CPU usage")) {
                ts_TransistorConfig config = ts_config(T);
                if (ImGui::MenuItem("Light", "", config.cpu_usage == TS_CPU_LIGHT))
                    ts_update_cpu_usage(T, TS_CPU_LIGHT);
                if (ImGui::MenuItem("Normal", "", config.cpu_usage == TS_CPU_NORMAL))
                    ts_update_cpu_usage(T, TS_CPU_NORMAL);
                if (ImGui::MenuItem("Aggressive", "", config.cpu_usage == TS_CPU_AGGRESSIVE))
                    ts_update_cpu_usage(T, TS_CPU_AGGRESSIVE);
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {

            if (ImGui::MenuItem("About"))
                open_about_box = true;

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (open_about_box)
        ImGui::OpenPopup("AboutTransistor");

    if (ImGui::BeginPopupModal("AboutTransistor", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Transistor " PROJECT_VERSION);
        ImGui::Text("Created by Andre Wagner.");
        ImGui::Text("Distributed as free software under the GPLv3 license.");
        ImGui::Separator();
        if (ImGui::Button("Ok"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}
