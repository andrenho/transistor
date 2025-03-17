#ifndef LUAMUTEX_HH
#define LUAMUTEX_HH

#include <mutex>
#include <lua.hpp>

class LuaMutex {
public:
    LuaMutex() : L(luaL_newstate()) {}
    ~LuaMutex() { lua_close(L); }

    template <typename F>
    void execute(F f)
    {
        std::lock_guard lock(mutex_);
        f(L);
    }

    template <typename T, typename F>
    T execute(F f)
    {
        std::lock_guard lock(mutex_);
        return f(L);
    }

    template <typename F, typename... Args>
    void execute(F f, Args&& ...args)
    {
        std::lock_guard lock(mutex_);
        f(L, args...);
    }

    template <typename T, typename F, typename... Args>
    T execute(F f, Args&& ...args)
    {
        std::lock_guard lock(mutex_);
        return f(L, args...);
    }

private:
    lua_State* L;
    std::mutex mutex_;
};

extern LuaMutex lua;

#endif //LUAMUTEX_HH
