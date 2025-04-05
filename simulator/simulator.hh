#ifndef SIMULATOR_HH
#define SIMULATOR_HH

#include <thread>

#include "luaenv/lua.hh"
#include "mappers/in/compiled_circuit.hh"

class Simulator {
public:
    enum class CpuUsage { Light, Normal, Aggressive };

    explicit Simulator(Lua& lua);
    ~Simulator();

    void main_thread();
    void single_step();  // runs inside main thread

    void update_compiled_circuit(luaobj::CompiledCircuit&& circuit);
    void update_cpu_usage(CpuUsage usage);

    uint64_t steps();

private:
    bool                    running_ = true;
    CpuUsage                cpu_usage_ = CpuUsage::Normal;
    Lua&                    lua_;
    std::thread             thread_;
    std::mutex              mutex_;
    luaobj::CompiledCircuit circuit_;
    int                     simulate_luaref_;
    size_t                  steps_ = 0;
};

#endif //SIMULATOR_HH
