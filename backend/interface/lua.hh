#ifndef LUA_HH
#define LUA_HH

#include <lua.hpp>
#include <mutex>

class Lua {
public:
    Lua() : L(luaL_newstate()) {}
    ~Lua() { lua_close(L); }

    template <typename F>
    void with_lua(F f) {
        std::lock_guard lock_guard(mutex_);
        f(L);
    }

    template <typename F, typename... Args>
    void with_lua(F f, Args... args) {
        std::lock_guard lock_guard(mutex_);
        f(L, args...);
    }

    template <typename T, typename F>
    T with_lua(F f) {
        std::lock_guard lock_guard(mutex_);
        return f(L);
    }

    template <typename T, typename F, typename... Args>
    T with_lua(F f, Args... args) {
        std::lock_guard lock_guard(mutex_);
        return f(L, args...);
    }

    void restart() {
        lua_close(L);
        L = luaL_newstate();
    }

private:
    lua_State* L;
    std::mutex mutex_;
};

#endif //LUA_HH
