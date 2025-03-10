#ifndef ENGINE_HH
#define ENGINE_HH

#include <lua.hpp>
#include <cstdint>

class Engine {
public:
    Engine();
    ~Engine();

private:
    lua_State* L;
    int sandbox_ref_ = -1;

    void load(const char* name, uint8_t const* bytecode, size_t sz);
    void execute(const char* fmt, ...);
};

#endif //ENGINE_HH
