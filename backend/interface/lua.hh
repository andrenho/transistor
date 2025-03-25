#ifndef LUA_HH
#define LUA_HH

#include <lua.hpp>
#include <mutex>

class Lua {
public:
    Lua() : L(luaL_newstate()) {}
    ~Lua() { lua_close(L); }

    template <typename T=void, typename F, typename... Args>
    auto with_lua(F f, Args... args) const {
        std::lock_guard lock_guard(mutex_);
        if constexpr (std::is_same_v<T, void>)
            f(L, args...);
        else
            return f(L, args...);
    }

private:
    lua_State* L;
    mutable std::mutex mutex_;
};

class LuaRef {
public:
    explicit LuaRef(lua_State* L) : L(L), ref_(luaL_ref(L, LUA_REGISTRYINDEX)) {}

    ~LuaRef()
    {
        luaL_unref(L, LUA_REGISTRYINDEX, ref_);
    }

    void get() const
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, ref_);
    }

private:
    lua_State* L;
    int ref_;
};

#endif //LUA_HH
