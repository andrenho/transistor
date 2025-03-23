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

#endif //LUA_HH
