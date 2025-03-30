#ifndef SCENEMAPPER_HH
#define SCENEMAPPER_HH

#include <vector>

#include <lua.hpp>
#include <pastel2d.hh>

namespace scene_mapper {

std::vector<ps::Scene> map_to_scene(lua_State* L);

};

#endif //SCENEMAPPER_HH
