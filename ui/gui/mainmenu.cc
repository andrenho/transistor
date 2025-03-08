#include "mainmenu.hh"

#include <imgui.h>

extern "C" {
#include <pastel2d.h>
#include "ui/common.h"
}

extern bool show_demo_window_;

bool open_about_box_ = false;
bool open_quit_box_ = false;

static void about_box()
{
    if (open_about_box_)
        ImGui::OpenPopup("About Transistor");

    if (ImGui::BeginPopupModal("About Transistor", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Transistor " PROJECT_VERSION);
        ImGui::Text("Created by Andre Wagner.");
        ImGui::Text("Distributed as free software under the GPLv3 license.");
        ImGui::Separator();
        if (ImGui::Button("Ok"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

static void quit_box(ts_Transistor* T)
{
    if (open_quit_box_)
        ImGui::OpenPopup("Quit?");

    if (ImGui::BeginPopupModal("Quit?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to quit?");
        ImGui::Text("(your work will be stashed until you return)");
        ImGui::Separator();
        if (ImGui::Button("Yes"))
            common_quit(T);
        ImGui::SameLine();
        if (ImGui::Button("No"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void main_menu_render(ts_Transistor* T)
{
    open_about_box_ = false;
    open_quit_box_ = false;

    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("Circuit")) {

            if (ImGui::MenuItem("Clear circuit"))
                ;
            if (ImGui::MenuItem("Load circuit..."))
                ;
            if (ImGui::MenuItem("Save circuit", "", false, common_savename[0] != '\0'))
                common_save(T);
            if (ImGui::MenuItem("Save circuit as..."))
                ;

#ifndef NDEBUG
            ImGui::Separator();

            if (ImGui::MenuItem("Serialize to stdout")) {
                ts_serialize_to_file(T, stdout);
                printf("-----------------------\n");
            }

            ImGui::MenuItem("Demo Window", "", &show_demo_window_);
#endif

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
                open_quit_box_ = true;

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
                open_about_box_ = true;

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    about_box();
    quit_box(T);
}
