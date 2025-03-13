#ifndef ENGINE_HH
#define ENGINE_HH

#include <functional>
#include <cstdint>
#include <lua.hpp>

#include "snapshot.hh"
#include "simulation/simulation.hh"

class Engine {
public:
    Engine();
    ~Engine();

    void start();

    Snapshot take_snapshot();

private:
    lua_State* L;
    Simulation simulation_;
    int sandbox_ref_ = -1;

    void load_bytecode(const char* name, uint8_t const* bytecode, size_t sz);
    void execute(std::string const& command, bool recompile=true, std::function<void()> const& and_also_do=nullptr);

    void recompile_sandbox();

    friend class Tests;
};

#endif //ENGINE_HH
