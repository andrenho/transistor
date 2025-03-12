#ifndef ENGINE_HH
#define ENGINE_HH

#include <lua.hpp>
#include <cstdint>
#include <simulation/simulation.hh>

class Engine {
public:
    Engine();
    ~Engine();

private:
    lua_State* L;
    Simulation simulation_;
    int sandbox_ref_ = -1;

    void load(const char* name, uint8_t const* bytecode, size_t sz);
    void execute(bool recompile, const char* fmt, ...)  __attribute__((__format__ (__printf__, 3, 4)));

    void recompile_sandbox();

    void register_native_array_function();

    friend void run_tests();
};

#endif //ENGINE_HH
