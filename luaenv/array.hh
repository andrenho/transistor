#ifndef ARRAY_HH
#define ARRAY_HH

#include <cstdint>
#include <lua.hpp>

struct Array {
    size_t   sz;
    uint8_t* data;
};

void setup_array(lua_State* L);

#endif //ARRAY_HH
