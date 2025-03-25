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

    void restart() {
        lua_close(L);
        L = luaL_newstate();
    }

private:
    lua_State* L;
    mutable std::mutex mutex_;
};

class LuaRef {
public:
    explicit LuaRef(Lua const& lua) : lua(lua) {}

    ~LuaRef()
    {
        if (ref_ != -1)
            lua.with_lua([this](lua_State* L) { luaL_unref(L, LUA_REGISTRYINDEX, ref_); });
    }

    void set()
    {
        lua.with_lua([this](lua_State* L) { this->ref_ = luaL_ref(L, LUA_REGISTRYINDEX); });
    }

    void get() const
    {
        lua.with_lua([this](lua_State* L) { lua_rawgeti(L, LUA_REGISTRYINDEX, ref_); });
    }

    bool has() const { return ref_ != -1; }

private:
    int ref_ = -1;
    Lua const& lua;
};

#endif //LUA_HH
