#include "menu.hh"

#include "imgui.h"
#include "mappers/in/menu.hh"

static void add_menu_item(luaobj::MenuItem const& menu_item, Engine& engine, luaobj::Render const& render)
{
    if (!menu_item.items.empty()) {
        if (ImGui::BeginMenu(menu_item.text.c_str())) {
            for (auto const& item: menu_item.items)
                add_menu_item(item, engine, render);
            ImGui::EndMenu();
        }
    } else {
        if (ImGui::MenuItem(menu_item.text.c_str())) {
            if (menu_item.ask_confirmation)
                menu_item.ask_confirmation->show = true;
            else if (menu_item.callback)
                engine.call_api_function(*menu_item.callback);
        }
    }
}

void render_menu(luaobj::Render const& render, Engine& engine)
{
    if (ImGui::BeginMainMenuBar()) {
        for (auto const& item: render.menu)
            add_menu_item(item, engine, render);
        ImGui::EndMainMenuBar();
    }
}
