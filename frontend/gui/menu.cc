#include "menu.hh"

#include "imgui.h"

static void add_menu_item(MenuItem const& menu_item, Transistor& T)
{
    if (!menu_item.items.empty()) {
        if (ImGui::BeginMenu(menu_item.text.c_str())) {
            for (auto const& item: menu_item.items)
                add_menu_item(item, T);
            ImGui::EndMenu();
        }
    } else {
        if (ImGui::MenuItem(menu_item.text.c_str())) {
            if (menu_item.callback_ref) {
                T.execure_ref_with_sandbox(*menu_item.callback_ref);
            }
        }
    }
}

void render_menu(std::vector<MenuItem> const& menu_items, Transistor& T)
{
    if (ImGui::BeginMainMenuBar()) {
        for (auto const& item: menu_items)
            add_menu_item(item, T);
        ImGui::EndMainMenuBar();
    }
}
