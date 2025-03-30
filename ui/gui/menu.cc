#include "menu.hh"

#include "imgui.h"
#include "mappers/in/menu.hh"

static void add_menu_item(luaobj::MenuItem const& menu_item, std::vector<luaobj::Event>& events, luaobj::Render const& render)
{
    if (!menu_item.items.empty()) {
        if (ImGui::BeginMenu(menu_item.text.c_str())) {
            for (auto const& item: menu_item.items)
                add_menu_item(item, events, render);
            ImGui::EndMenu();
        }
    } else {
        if (ImGui::MenuItem(menu_item.text.c_str())) {
            if (menu_item.ask_confirmation)
                menu_item.ask_confirmation->show = true;
            else if (menu_item.event)
                events.push_back(*menu_item.event);
        }
    }
}

void render_menu(luaobj::Render const& render, std::vector<luaobj::Event>& events)
{
    if (ImGui::BeginMainMenuBar()) {
        for (auto const& item: render.menu)
            add_menu_item(item, events, render);
        ImGui::EndMainMenuBar();
    }
}
