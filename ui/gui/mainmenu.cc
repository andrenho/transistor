#include "mainmenu.hh"

#include <string>
#include <imgui.h>

#include "dialog.hh"

extern "C" {
#include <pastel2d.h>
#include "ui/common.h"
}

extern bool show_demo_window_;

static MessageBox about("About Transistor", {
    "Transistor " PROJECT_VERSION,
    "Created by Andre Wagner.",
    "Distributed as free software under the GPLv3 license."
});

static YesNoDialog quit_dialog("Quit?", {
    "Are you sure you want to quit?",
    "(your work will be stashed until you return)"
}, [](ts_Transistor* T) { common_quit(T); });

static YesNoDialog clear_dialog("Clear?", {
    "Are you sure you want to clear the circuit?",
    "(you'll lose any unsaved work!)"
}, [](ts_Transistor* T) { common_clear(T); });

std::vector<Dialog*> dialogs = { &about, &quit_dialog, &clear_dialog };

void main_menu_render(ts_Transistor* T)
{
    for (Dialog* dialog: dialogs)
        dialog->reset();

    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("Circuit")) {

            if (ImGui::MenuItem("Clear circuit"))
                clear_dialog.show();
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
                quit_dialog.show();

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
                about.show();

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    for (Dialog* dialog: dialogs)
        dialog->render(T);
}
