#ifndef SIMULATION_HH
#define SIMULATION_HH

#include <lua.hpp>

#include <condition_variable>
#include <thread>

#include "compilation.hh"

enum class CpuUsage { Light, Normal, Aggressive };

class Simulation {
public:
    void start();
    void stop();

    void update_compilation_result(CompilationResult&& result);

    void reset_steps();

    [[nodiscard]] CompilationResult const& result() const { return result_; }
    [[nodiscard]] uint64_t steps();

    void set_cpu_usage(CpuUsage cpu_usage) { cpu_usage_ = cpu_usage; }
    void set_simulate_luaref(int simulate_luaref) { simulate_luaref_ = simulate_luaref; }

private:
    // thread control
    std::thread thread_;

    // read-only inside thread
    CompilationResult result_;
    bool              running_ = true;
    CpuUsage          cpu_usage_ = CpuUsage::Normal;
    int               simulate_luaref_ = -1;

    // written inside thread
    uint64_t    steps_ = 0;

    static void simulation_thread(Simulation* simulation);
    static void simulation_single_step(lua_State* L, Simulation* simulation);
};

#endif //SIMULATION_HH
