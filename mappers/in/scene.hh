#ifndef SCENEMAPPER_HH
#define SCENEMAPPER_HH

#include <vector>

#include <lua.hpp>

struct Scene {
};


namespace scene_mapper {

std::vector<Scene> map_to_scene(lua_State* L);

};

#endif //SCENEMAPPER_HH
