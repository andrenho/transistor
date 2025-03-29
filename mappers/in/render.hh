#ifndef RENDER_HH
#define RENDER_HH

#include <luaw.hh>

#include "scene.hh"
#include "menu.hh"

namespace luaobj {

struct Render {
    std::vector<Scene> scenes;
    std::vector<MenuItem> menu;

    static Render from_lua(lua_State* L, int index) {
        return {
            .scenes = luaw_getfield<std::vector<Scene>>(L, index, "scenes"),
            .menu = luaw_getfield<std::vector<MenuItem>>(L, index, "menu"),
        };
    }
};

}

#endif //RENDER_HH
