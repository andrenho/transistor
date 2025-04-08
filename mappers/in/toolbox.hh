#ifndef TOOLBOX_HH_
#define TOOLBOX_HH_

#include <string>

namespace luaobj {

struct CategoryDef {
    std::string name;
    std::string key;

    static CategoryDef from_lua(lua_State* L, int index) {
        return {
            .name = luaw_getfield<std::string>(L, index, "name"),
            .key = luaw_getfield<std::string>(L, index, "key"),
        };
    }
};

struct ToolItem {
    ps::res::idx_t             image;
    std::optional<std::string> key;
    std::string                tooltip;
    bool                       selected;
    bool                       separator = false;
    std::optional<std::string> category;
    std::optional<std::vector<std::string>>                                  menus;
    std::optional<std::unordered_map<std::string, std::vector<CategoryDef>>> submenus;

    static ToolItem from_lua(lua_State* L, int index) {
        if (luaw_getfield<std::optional<bool>>(L,index, "separator").value_or(false))
            return { .separator = true };
        return {
            .image = ps::res::idx(luaw_getfield<std::string>(L, index, "image")),
            .LUAW_FIELD(key),
            .LUAW_FIELD(tooltip),
            .selected = luaw_getfield<std::optional<bool>>(L, index, "selected").value_or(false),
            .separator = false,
            .LUAW_FIELD(category),
            .LUAW_FIELD(menus),
            .LUAW_FIELD(submenus)
        };
    }
};

using ToolBox = std::vector<ToolItem>;

}

#endif //TOOLBOX_HH_
