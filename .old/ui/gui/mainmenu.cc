#include "mainmenu.hh"

#include <string>
#include <imgui.h>
#include <pl_log.h>

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

std::vector<SDL_DialogFileFilter> file_filters = {
    { "Transistor files", "trn" },
    { "All files", "*" },
};

static std::string adjust_filename(std::string const& filename)
{
    if (!filename.ends_with(".trn"))
        return filename + ".trn";
    return filename;
}

void main_menu_render(ts_Transistor* T)
{
    for (Dialog* dialog: dialogs)
        dialog->reset();

    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("Circuit")) {

            if (ImGui::MenuItem("Clear circuit"))
                clear_dialog.show();
            if (ImGui::MenuItem("Load circuit...")) {
                SDL_ShowOpenFileDialog([](void* T, const char* const *filelist, int filter) {
                    if (!filelist) PL_ERROR("%s", SDL_GetError());
                    if (*filelist) common_load((ts_Transistor *) T, *filelist);
                }, T, ps_graphics_window(), file_filters.data(), (int) file_filters.size(), nullptr, false);
            }
            if (ImGui::MenuItem("Save circuit", "", false, common_savename[0] != '\0'))
                common_save(T);
            if (ImGui::MenuItem("Save circuit as...")) {
                SDL_ShowSaveFileDialog([](void* T, const char* const *filelist, int filter) {
                    if (!filelist) PL_ERROR("%s", SDL_GetError());
                    if (*filelist) common_save_as((ts_Transistor *) T, adjust_filename(*filelist).c_str());
                }, T, ps_graphics_window(), file_filters.data(), (int) file_filters.size(), nullptr);
            }

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
