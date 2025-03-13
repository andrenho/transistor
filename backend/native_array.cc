#include "native_array.hh"

#include <cassert>
#include <cstdint>
#include <cstring>

#include <lauxlib.h>

void register_native_array(lua_State* L)
{
    struct NativeArray {
        size_t   sz;
        uint8_t* data;
    };

    // `NativeArray` metatable

    luaL_newmetatable(L, "NativeArray");
    luaL_setfuncs(L, (luaL_Reg[]) {

        { "__index", [](lua_State* LL) {

            if (lua_isnumber(LL, 2)) {
                NativeArray* native_array = (NativeArray*) lua_touserdata(LL, 1);
                lua_pushinteger(LL, native_array->data[lua_tointeger(LL, 2) - 1]);
                return 1;
            }

            if (lua_isstring(LL, 2)) {
                const char* function = lua_tostring(LL, 2);

                if (strcmp(function, "ptr") == 0) {
                    lua_pushcfunction(LL, [](lua_State* LLL) {
                        NativeArray* native_array = (NativeArray *) luaL_checkudata(LLL, 1, "NativeArray");
                        lua_pushinteger(LLL, (intptr_t) native_array->data);
                        return 1;
                    });
                    return 1;
                }

                luaL_error(LL, "Invalid method '%s'", function);
            }

            luaL_error(LL, "Function called with a invalid type.");
            return 0;
        } },

        { "__newindex", [](lua_State* LL) {
            NativeArray* native_array = (NativeArray*) lua_touserdata(LL, 1);
            native_array->data[luaL_checkinteger(LL, 2) - 1] = luaL_checkinteger(LL, 3) & 0xff;
            return 0;
        } },

        { "__len", [](lua_State* LL) {
            NativeArray* native_array = (NativeArray*) lua_touserdata(LL, 1);
            lua_pushinteger(LL, native_array->sz);
            return 1;
        } },

        { "__gc", [](lua_State* LL) {
            NativeArray* native_array = (NativeArray*) lua_touserdata(LL, 1);
            delete[] native_array->data;
            return 0;
        } },

        { nullptr, nullptr },
    }, 0);
    lua_pop(L, 1);
    assert(lua_gettop(L) == 0);

    // `native_array` function
    lua_pushcfunction(L, [](lua_State* LL) -> int {
        int sz = luaL_checkinteger(LL, 1);
        NativeArray* native_array = (NativeArray *) lua_newuserdata(LL, sizeof(NativeArray));
        native_array->sz = sz;
        native_array->data = new uint8_t[sz];
        memset(native_array->data, 0, sz);
        luaL_getmetatable(LL, "NativeArray");
        assert(!lua_isnil(LL, -1));
        lua_setmetatable(LL, -2);
        return 1;
    });
    lua_setglobal(L, "native_array");
    assert(lua_gettop(L) == 0);
}
