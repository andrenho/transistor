#include "menu.hh"

#include "imgui.h"

static void add_menu_item(MenuItem const& menu_item, Transistor& T, Render const& render)
{
    if (!menu_item.items.empty()) {
        if (ImGui::BeginMenu(menu_item.text.c_str())) {
            for (auto const& item: menu_item.items)
                add_menu_item(item, T, render);
            ImGui::EndMenu();
        }
    } else {
        if (ImGui::MenuItem(menu_item.text.c_str())) {
            if (menu_item.confirmation_dialog)
                render.dialogs.at(*menu_item.confirmation_dialog).show = true;
            else if (menu_item.callback)
                T.call_api_function(*menu_item.callback);
        }
    }
}

void render_menu(Render const& render, Transistor& T)
{
    if (ImGui::BeginMainMenuBar()) {
        for (auto const& item: render.menus)
            add_menu_item(item, T, render);
        ImGui::EndMainMenuBar();
    }
}
