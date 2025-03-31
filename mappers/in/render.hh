#ifndef RENDER_HH
#define RENDER_HH

#include <luaw.hh>
#include <pastel2d.hh>

#include "menu.hh"
#include "scenemapper.hh"

namespace luaobj {

struct Render {
    std::vector<ps::Scene> scenes;
    std::vector<MenuItem>  menu;

    static Render from_lua(lua_State* L, int index) {
        Render render = {
            .scenes = {},
            .menu = luaw_getfield<std::vector<MenuItem>>(L, index, "menu"),
        };

        luaw_getfield(L, index, "scenes");
        render.scenes = scene_mapper::map_to_scene(L);
        lua_pop(L, 1);

        return render;
    }
};

}

#endif //RENDER_HH
