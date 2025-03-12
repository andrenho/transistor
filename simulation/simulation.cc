#include "simulation.hh"

#include <chrono>
using namespace std::chrono_literals;

Simulation::Simulation(lua_State* L)
    : L(L), thread_(simulation_thread, this)
{
}

Simulation::~Simulation()
{
    running_ = false;
    thread_.join();
}

void Simulation::simulation_thread(Simulation* simulation)
{
    while (simulation->running_) {

        // pause thread
        std::unique_lock lock(simulation->mutex_);
        simulation->cv_.wait(lock, [&simulation] { return !simulation->paused_; });

        // execute
        simulation_single_step(simulation);

        // unpause
        simulation->paused_ = false;

        // give the CPU a break
        switch (simulation->cpu_usage_) {
            case CpuUsage::Light:      std::this_thread::sleep_for(100ns); break;
            case CpuUsage::Normal:     std::this_thread::yield(); break;
            case CpuUsage::Aggressive: break;
        }

        // TODO - calculate timing
    }
}

void Simulation::simulation_single_step(Simulation* simulation)
{
    // TODO - simulate components (C)

    // TODO - simulate components (Lua)

    // update connection and pin values
    //for (auto& connection: simulation->connections_) {

        // TODO - calculate value from output pins

        // TODO - set connection value

        // TODO - set input pins

    // }

    ++simulation->steps_;
}

void Simulation::update_compilation_result(CompilationResult&& result)
{
    pause();
    result_ = std::move(result);
    resume();
}

void Simulation::pause()
{
    paused_ = true;
}

void Simulation::resume()
{
    paused_ = false;
    cv_.notify_one();
}