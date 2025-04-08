#ifndef RENDER_HH
#define RENDER_HH

#include <string>
#include <optional>
#include <vector>

#include <luaw.hh>
#include <pastel2d.hh>

#include "menu.hh"
#include "toolbox.hh"
#include "scenemapper.hh"

namespace luaobj {

struct Render {
    std::vector<ps::Scene>     scenes;
    std::vector<MenuItem>      menu;
    ToolBox                    toolbox;
    std::optional<std::string> infobox;

    static Render from_lua(lua_State* L, int index) {
        Render render = {
            .scenes = {},
            .LUAW_FIELD(menu),
            .LUAW_FIELD(toolbox),
            .LUAW_FIELD(infobox),
        };

        luaw_getfield(L, index, "scenes");
        render.scenes = scene_mapper::map_to_scene(L);
        lua_pop(L, 1);

        return render;
    }
};

}

#endif //RENDER_HH
