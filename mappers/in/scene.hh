#ifndef SCENE_HH
#define SCENE_HH

#include <lua.hpp>

namespace luaobj {

struct Scene {
    static Scene from_lua(lua_State* L, int index) {
        return {};
    }
};

}

#endif //SCENE_HH
