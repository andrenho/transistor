#ifndef TOOLBOX_HH_
#define TOOLBOX_HH_

#include <string>

namespace luaobj {

struct ToolItem {
    ps::res::idx_t             image;
    std::optional<std::string> key;
    std::string                tooltip;
    bool                       selected;
    bool                       separator = false;

    static ToolItem from_lua(lua_State* L, int index) {
        if (luaw_getfield<std::optional<bool>>(L,index, "separator").value_or(false))
            return { .separator = true };
        return {
            .image = ps::res::idx(luaw_getfield<std::string>(L, index, "image")),
            .key = luaw_getfield<std::optional<std::string>>(L, index, "key"),
            .tooltip = luaw_getfield<std::string>(L, index, "tooltip"),
            .selected = luaw_getfield<std::optional<bool>>(L, index, "selected").value_or(false),
            .separator = false,
        };
    }
};

using ToolBox = std::vector<ToolItem>;

}

#endif //TOOLBOX_HH_
