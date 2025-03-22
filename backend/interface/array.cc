#include <cassert>
#include <cstring>
#include <cstdint>
#include <lua.hpp>

void setup_array(lua_State* L)
{
    struct Array {
        size_t   sz;
        uint8_t* data;
    };

    // metatable
    luaL_newmetatable(L, "Array_mt");
    luaL_setfuncs(L, (luaL_Reg[]) {

        { "__index", [](lua_State* LL) {

            if (lua_isnumber(LL, 2)) {
                Array* array = (Array*) lua_touserdata(LL, 1);
                lua_pushinteger(LL, array->data[lua_tointeger(LL, 2) - 1]);
                return 1;
            }

            if (lua_isstring(LL, 2)) {
                const char* function = lua_tostring(LL, 2);

                if (strcmp(function, "ptr") == 0) {
                    lua_pushcfunction(LL, [](lua_State* LLL) {
                        Array* array = (Array *) luaL_checkudata(LLL, 1, "Array");
                        lua_pushinteger(LLL, (intptr_t) array->data);
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
            Array* array = (Array*) lua_touserdata(LL, 1);
            array->data[luaL_checkinteger(LL, 2) - 1] = luaL_checkinteger(LL, 3) & 0xff;
            return 0;
        } },

        { "__len", [](lua_State* LL) {
            Array* array = (Array*) lua_touserdata(LL, 1);
            lua_pushinteger(LL, (int) array->sz);
            return 1;
        } },

        { "__gc", [](lua_State* LL) {
            Array* array = (Array*) lua_touserdata(LL, 1);
            delete[] array->data;
            return 0;
        } },
        { nullptr, nullptr }
    }, 0);
    lua_pop(L, 1);
    assert(lua_gettop(L) == 0);

    // constructor
    lua_pushcfunction(L, [](lua_State* LL) -> int {
        int sz = luaL_checkinteger(LL, 1);
        Array* array = (Array *) lua_newuserdata(LL, sizeof(Array));
        array->sz = sz;
        array->data = new uint8_t[sz];
        memset(array->data, 0, sz);
        luaL_getmetatable(LL, "Array_mt");
        assert(!lua_isnil(LL, -1));
        lua_setmetatable(LL, -2);
        return 1;
    });
    lua_setglobal(L, "Array");
    assert(lua_gettop(L) == 0);
}
