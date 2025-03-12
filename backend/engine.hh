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

    void load_bytecode(const char* name, uint8_t const* bytecode, size_t sz);
    void execute(std::string const& command, bool recompile=true, std::function<void()> const& and_also_do=nullptr);

    void recompile_sandbox();

    void register_native_array_function();

    friend void run_tests();
};

#endif //ENGINE_HH
