#ifndef SIMULATION_HH
#define SIMULATION_HH

#include <lua.hpp>

#include <condition_variable>
#include <thread>

#include "backend/compilationresult.hh"

enum class CpuUsage { Light, Normal, Aggressive };

class Simulation {
public:
    explicit Simulation(lua_State* L);

    void start();
    void stop();

    void update_compilation_result(CompilationResult&& result);

    void pause();
    void resume();

    void set_cpu_usage(CpuUsage cpu_usage) { cpu_usage_ = cpu_usage; }

    void set_simulate_luaref(int simulate_luaref) { simulate_luaref_ = simulate_luaref; }

private:
    lua_State* L;

    // thread control
    std::thread thread_;
    std::mutex  mutex_;
    std::condition_variable cv_;

    // read-only inside thread
    CompilationResult result_;
    bool              running_ = true;
    CpuUsage          cpu_usage_ = CpuUsage::Normal;
    int               simulate_luaref_ = -1;

    // written inside thread
    uint64_t    steps_ = 0;
    bool        paused_ = false;

    static void simulation_thread(Simulation* simulation);
    static void simulation_single_step(Simulation* simulation);
};

#endif //SIMULATION_HH
