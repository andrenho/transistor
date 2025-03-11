#ifndef ENGINE_HH
#define ENGINE_HH

#include <lua.hpp>
#include <cstdint>
#include <simulation/simulation.hh>

class Engine {
public:
    Engine();
    ~Engine();

    void start();

private:
    lua_State* L;
    Simulation simulation_;
    int sandbox_ref_ = -1;

    void load(const char* name, uint8_t const* bytecode, size_t sz);
    void execute(const char* fmt, ...);

    friend void run_tests();
};

#endif //ENGINE_HH
